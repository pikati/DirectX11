#include "main.h"
#include "renderer.h"
#include "Field.h"
#include "Transform.h"

Field::Field()
{

}

Field::~Field()
{

}

void Field::Initialize()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(-10.0f, 0.0f, 10.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(10.0f, 0.0f, 10.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-10.0f, 0.0f, -10.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(10.0f, 0.0f, -10.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファの設定
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;//ここかえるとマップアンマップできるらしい
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;//ここかえるとマップアンマップできるらしい

	//頂点バッファを入れる
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_vertexBuffer);

	//テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		"Asset/Texture/koyorichan.jpg",
		NULL,
		NULL,
		&m_texture,
		NULL);

	//m_textureがnullならエラーさせる
	assert(m_texture);

	gameObject->transform->position = Vector3(0.0f, 0.0f, 0.0f);
	gameObject->transform->rotation = Vector3(0.0f, 0.0f, 0.0f);
	gameObject->transform->scale = Vector3(1.0f, 1.0f, 1.0f);
}

void Field::Update()
{

}

void Field::Draw()
{
	//マトリクス設定
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

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);//頂点1個分のサイズ
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetMaterial(material);

	//テクスチャ設定
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_texture);//GetにするとGPUのテクスチャを取得する

	//プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	CRenderer::GetDeviceContext()->Draw(4, 0);
}

void Field::Finalize()
{
	SAFE_RELEASE(m_vertexBuffer);
	SAFE_RELEASE(m_texture);
}