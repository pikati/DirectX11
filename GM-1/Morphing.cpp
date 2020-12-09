#include "Morphing.h"
#include "manager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "renderer.h"
#include "Fbx.h"
#include "FPS.h"
#include <DirectXMath.h>

void Morphing::Initialize()
{
	Scene* s = CManager::GetScene();
	Fbx* f1 = s->Find("BackDog1")->GetComponent<Fbx>();
	Fbx* f2 = s->Find("BackRabbit1")->GetComponent<Fbx>();
	m_f1 = f1->GetVertices(0);
	m_f2 = f2->GetVertices(0);
	m_vCount = f1->GetVerticesCount(0);
	m_iCount = f1->GetIndecesCount(0);
	m_tex1 = f1->GetTexture();
	m_tex2 = f2->GetTexture();

	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * m_vCount;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_f1;
		sd.SysMemPitch = 0;
		sd.SysMemSlicePitch = 0;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &pVB);
	}
	
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * m_iCount;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = f1->GetIndeces(0);
		sd.SysMemPitch = 0;
		sd.SysMemSlicePitch = 0;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &pIB);
	}
}

void Morphing::Update()
{
	if (m_isUp)
	{
		m_raito += FPS::deltaTime * 0.2f;
		if (m_raito >= 1.0f)
		{
			m_raito = 1.0f;
			m_isUp = false;
		}
	}
	else
	{
		m_raito -= FPS::deltaTime * 0.2f;
		if (m_raito <= 0.0f)
		{
			m_raito = 0.0f;
			m_isUp = true;
		}
	}
}

void Morphing::Draw()
{
	VERTEX_3D* vtx = new VERTEX_3D[m_vCount];
	for (int i = 0; i < m_vCount; i++)
	{
		vtx[i].Position.x = m_f1[i].Position.x * m_raito + m_f2[i].Position.x * (1 - m_raito);
		vtx[i].Position.y = m_f1[i].Position.y * m_raito + m_f2[i].Position.y * (1 - m_raito);
		vtx[i].Position.z = m_f1[i].Position.z * m_raito + m_f2[i].Position.z * (1 - m_raito);
		vtx[i].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
		vtx[i].Normal.x = m_f1[i].Normal.x * m_raito + m_f2[i].Normal.x * (1 - m_raito);
		vtx[i].Normal.y = m_f1[i].Normal.y * m_raito + m_f2[i].Normal.y * (1 - m_raito);
		vtx[i].Normal.z = m_f1[i].Normal.z * m_raito + m_f2[i].Normal.z * (1 - m_raito);
		vtx[i].TexCoord.x = m_f1[i].TexCoord.x * m_raito + m_f2[i].TexCoord.x * (1 - m_raito);
		vtx[i].TexCoord.y = m_f1[i].TexCoord.y * m_raito + m_f2[i].TexCoord.y * (1 - m_raito);
	}
	D3D11_MAPPED_SUBRESOURCE pdata;
	CRenderer::GetDeviceContext()->Map(pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(vtx), sizeof(VERTEX_3D) *m_vCount);
	CRenderer::GetDeviceContext()->Unmap(pVB, 0);
	using namespace DirectX;
	D3DXMATRIX world, rotation;
	D3DXMatrixRotationYawPitchRoll(&rotation, XMConvertToRadians(gameObject->transform->rotation.y), XMConvertToRadians(gameObject->transform->rotation.x), XMConvertToRadians(gameObject->transform->rotation.z));
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

	CRenderer::SetShader(SHADER_TYPE::Morphing);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &pVB, &stride, &offset);

	// インデックスバッファ設定
	CRenderer::GetDeviceContext()->IASetIndexBuffer(pIB, DXGI_FORMAT_R32_UINT, 0);

	// プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	material.Ambient = { 1.0f,1.0f,1.0f,1.0f };
	CRenderer::SetMaterial(material);
	// マテリアル設定
	//CRenderer::SetMaterial(m_fbxInfo.materialInfo[i]);

	// テクスチャ設定
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_tex1);
	CRenderer::GetDeviceContext()->PSSetShaderResources(1, 1, &m_tex2);
	CRenderer::SetParameter(Vector4(m_raito, 0, 0, 0));

	// ポリゴン描画
	CRenderer::GetDeviceContext()->DrawIndexed(m_iCount, 0, 0);
	delete[] vtx;
	vtx = nullptr;
}

void Morphing::Finalize()
{
	SAFE_RELEASE(pIB);
	SAFE_RELEASE(pVB);
	m_tex1 = nullptr;
	m_tex2 = nullptr;
	m_f1 = nullptr;
	m_f2 = nullptr;
}