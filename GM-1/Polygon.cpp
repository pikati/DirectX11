#include "main.h"
#include "renderer.h"
#include "Polygon.h"

CPolygon::CPolygon()
{

}

CPolygon::~CPolygon()
{

}

void CPolygon::Initialize() 
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Normal   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse  = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(200.0f, 0.0f, 0.0f);
	vertex[1].Normal   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse  = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(0.0f, 200.0f, 0.0f);
	vertex[2].Normal   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse  = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(200.0f, 200.0f, 0.0f);
	vertex[3].Normal   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse  = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
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
											"Asset/Texture/Penguin.png",
											NULL,
											NULL,
											&m_texture,
											NULL);
}

void CPolygon::Update() 
{

}

void CPolygon::Draw()
{
	//マトリクス設定
	CRenderer::SetWorldViewProjection2D();

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

void CPolygon::Finalize()
{
	SAFE_RELEASE(m_vertexBuffer);
	SAFE_RELEASE(m_texture);
}