#include "MeshField.h"
#include "GameObject.h"
#include "Transform.h"
#include "PerlinNoise.h"
#include <random>

void MeshField::Initialize()
{
	PerlinNoise p;
	p.SettingHash(0);
	std::random_device rnd;
	std::mt19937 mt(rnd());
	float texX = 0;
	float texY = 0;
	float y = 0;
	m_vertices = new VERTEX_3D[(m_x + 1) * (m_z + 1)];

	for (int z = 0; z < m_z + 1; z++)
	{
		for (int x = 0; x < m_x + 1; x++)
		{
			float y = p.OctavePerlinNoise(x, z);
			m_vertices[z * (m_x + 1) + x].Position = D3DXVECTOR3((m_x - x) * 5.0f, /*y * 5.0f*/0, z * 5.0f);
			m_vertices[z * (m_x + 1) + x].Normal = D3DXVECTOR3(0, 1, 0);
			m_vertices[z * (m_x + 1) + x].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			m_vertices[z * (m_x + 1) + x].TexCoord.x = texX;
			m_vertices[z * (m_x + 1) + x].TexCoord.y = texY;
			texX += 1.0f;
		}
		texX = 0;
		texY += 1.0f;
	}

	CalcNormals();

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * (m_x + 1) * (m_z + 1);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = m_vertices;


	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_vertexBuffer);

	int indexNum = ((m_x + 1) * 2) * m_z + (m_z - 1) * 2;
	unsigned int* index = new unsigned int[indexNum];


	int i = 0;
	for (int z = 0; z < m_z; z++)
	{
		for (int x = 0; x < m_x + 1; x++)
		{
			index[i] = m_x + 1 + x + z * (m_x + 1);
			i++;
			index[i] = x + z * (m_x + 1);
			i++;
		}
		if (z == m_z - 1) break;
		index[i] = index[i - 1];
		i++;
		index[i] = m_x + 1 + (z + 1) * (m_x + 1);
		i++;
	}

	D3D11_BUFFER_DESC bd2;
	ZeroMemory(&bd2, sizeof(bd2));
	bd2.Usage = D3D11_USAGE_DEFAULT;
	bd2.ByteWidth = sizeof(unsigned int) * indexNum;
	bd2.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd2.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd2;
	ZeroMemory(&sd2, sizeof(sd2));
	sd2.pSysMem = index;
	CRenderer::GetDevice()->CreateBuffer(&bd2, &sd2, &m_indexBuffer);

	delete index;


	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		m_fileName.c_str(),
		NULL,
		NULL,
		&m_texture,
		NULL);


	assert(m_texture);
}

void MeshField::Draw()
{
	D3DXMATRIX world, rotation;
	D3DXMatrixRotationYawPitchRoll(&rotation, gameObject->transform->rotation.y, gameObject->transform->rotation.x, gameObject->transform->rotation.z);
	world._11 = gameObject->transform->scale.x * rotation._11;
	world._12 = gameObject->transform->scale.x * rotation._12;
	world._13 = gameObject->transform->scale.x * rotation._13;
	world._21 = gameObject->transform->scale.y * rotation._21;
	world._22 = gameObject->transform->scale.y * rotation._22;
	world._23 = gameObject->transform->scale.y * rotation._23;
	world._31 = gameObject->transform->scale.z * rotation._31;
	world._32 = gameObject->transform->scale.z * rotation._32;
	world._33 = gameObject->transform->scale.z * rotation._33;
	world._41 = gameObject->transform->position.x;
	world._42 = gameObject->transform->position.y;
	world._43 = gameObject->transform->position.z;
	world._14 = 0;
	world._24 = 0;
	world._34 = 0;
	world._44 = 1.0f;
	CRenderer::SetWorldMatrix(&world);

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	CRenderer::GetDeviceContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetMaterial(material);

	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_texture);

	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	CRenderer::GetDeviceContext()->DrawIndexed(((m_x + 1) * 2) * m_z + (m_z - 1) * 2, 0, 0);
}

void MeshField::Finalize()
{
	delete m_vertices;
	SAFE_RELEASE(m_vertexBuffer);
	SAFE_RELEASE(m_indexBuffer);
	SAFE_RELEASE(m_texture);
}

void MeshField::SetTexture(std::string fileName)
{
	m_fileName = fileName;
}

void MeshField::SetX(int x)
{
	m_x = x;
}

void MeshField::SetZ(int z)
{
	m_z = z;
}

void MeshField::SetXZ(int x, int z)
{
	m_x = x;
	m_z = z;
}

void MeshField::CalcNormals()
{

	//ここで頂点は右上から左に並んで左下が終わりですよ！
	int count = 0;
	for (int z = 0; z < m_z + 1; z++)
	{
		for (int x = 0; x < m_x + 1; x++)
		{
#pragma region for
			//1番した
			if (z == m_x)
			{
				//左端
				if (x == m_x)
				{
					float x = m_vertices[count].Position.x - m_vertices[count - 1].Position.x;
					float y = m_vertices[count].Position.y - m_vertices[count - 1].Position.y;
					float z = m_vertices[count].Position.z - m_vertices[count - 1].Position.z;
					Vector3 vx = Vector3(x, y, z);
					x = m_vertices[count + m_x - 1].Position.x - m_vertices[count].Position.x;
					y = m_vertices[count + m_x - 1].Position.y - m_vertices[count].Position.y;
					z = m_vertices[count + m_x - 1].Position.z - m_vertices[count].Position.z;
					Vector3 vz = Vector3(x, y, z);
					SetNormal(count, vx, vz);
				}
				//左端以外
				else
				{
					float x = m_vertices[count + 1].Position.x - m_vertices[count].Position.x;
					float y = m_vertices[count + 1].Position.y - m_vertices[count].Position.y;
					float z = m_vertices[count + 1].Position.z - m_vertices[count].Position.z;
					Vector3 vx = Vector3(x, y, z);
					x = m_vertices[count + m_x - 1].Position.x - m_vertices[count].Position.x;
					y = m_vertices[count + m_x - 1].Position.y - m_vertices[count].Position.y;
					z = m_vertices[count + m_x - 1].Position.z - m_vertices[count].Position.z;
					Vector3 vz = Vector3(x, y, z);
					SetNormal(count, vx, vz);
				}
			}
			//1番した以外
			else
			{
				//左端
				if (x == m_x)
				{
					float x = m_vertices[count].Position.x - m_vertices[count - 1].Position.x;
					float y = m_vertices[count].Position.y - m_vertices[count - 1].Position.y;
					float z = m_vertices[count].Position.z - m_vertices[count - 1].Position.z;
					Vector3 vx = Vector3(x, y, z);
					x = m_vertices[count].Position.x - m_vertices[count + m_x + 1].Position.x;
					y = m_vertices[count].Position.y - m_vertices[count + m_x + 1].Position.y;
					z = m_vertices[count].Position.z - m_vertices[count + m_x + 1].Position.z;
					Vector3 vz = Vector3(x, y, z);
					SetNormal(count, vx, vz);
				}
				//左端以外
				else
				{
					float x = m_vertices[count + 1].Position.x - m_vertices[count].Position.x;
					float y = m_vertices[count + 1].Position.y - m_vertices[count].Position.y;
					float z = m_vertices[count + 1].Position.z - m_vertices[count].Position.z;
					Vector3 vx = Vector3(x, y, z);
					x = m_vertices[count].Position.x - m_vertices[count + m_x + 1].Position.x;
					y = m_vertices[count].Position.y - m_vertices[count + m_x + 1].Position.y;
					z = m_vertices[count].Position.z - m_vertices[count + m_x + 1].Position.z;
					Vector3 vz = Vector3(x, y, z);
					SetNormal(count, vx, vz);
				}
			}
			count++;
#pragma endregion
		}
	}
}

void MeshField::SetNormal(int index, Vector3 v1, Vector3 v2)
{
	Vector3 vn = Vector3::Cross(v2, v1);
	vn.Normalize();
	m_vertices[index].Normal = D3DXVECTOR3(vn.x, vn.y, vn.z);
}

VERTEX_3D* MeshField::GetVertices()
{
	return m_vertices;
}