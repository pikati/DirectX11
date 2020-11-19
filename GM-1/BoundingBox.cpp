#include "BoundingBox.h"
#include "main.h"
#include "renderer.h"
#include "GameObject.h"
#include "Transform.h"
#include <DirectXMath.h>

void BoundingBox::Draw(Transform* t)
{
	VERTEX_LINE v[8];
	v[0].Position = Vector3(m_min.x, m_max.y, m_min.z);
	v[1].Position = Vector3(m_max.x, m_max.y, m_min.z);
	v[2].Position = Vector3(m_min.x, m_min.y, m_min.z);
	v[3].Position = Vector3(m_max.x, m_min.y, m_min.z);
	v[4].Position = Vector3(m_min.x, m_max.y, m_max.z);
	v[5].Position = Vector3(m_max.x, m_max.y, m_max.z);
	v[6].Position = Vector3(m_min.x, m_min.y, m_max.z);
	v[7].Position = Vector3(m_max.x, m_min.y, m_max.z);

	int* index = new int[24];
	index[0] = 0;
	index[1] = 1;
	index[2] = 0;
	index[3] = 2;
	index[4] = 1;
	index[5] = 3;
	index[6] = 2;
	index[7] = 3;
	index[8] = 0;
	index[9] = 4;
	index[10] = 1;
	index[11] = 5;
	index[12] = 2;
	index[13] = 6;
	index[14] = 3;
	index[15] = 7;
	index[16] = 4;
	index[17] = 5;
	index[18] = 4;
	index[19] = 6;
	index[20] = 5;
	index[21] = 7;
	index[22] = 6;
	index[23] = 7;

	ID3D11Buffer* pVB;
	ID3D11Buffer* pIB;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_LINE) * 8;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = v;
	sd.SysMemPitch = 0;
	sd.SysMemSlicePitch = 0;
	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &pVB);

	D3D11_BUFFER_DESC bd2;
	ZeroMemory(&bd2, sizeof(bd2));
	bd2.Usage = D3D11_USAGE_DEFAULT;
	bd2.ByteWidth = sizeof(unsigned int) * 24;
	bd2.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd2.CPUAccessFlags = 0;
	bd2.MiscFlags = 0;
	bd2.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA sd2;
	ZeroMemory(&sd2, sizeof(sd2));
	sd2.pSysMem = index;
	sd2.SysMemPitch = 0;
	sd2.SysMemSlicePitch = 0;

	CRenderer::GetDevice()->CreateBuffer(&bd2, &sd2, &pIB);

	using namespace DirectX;
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	CRenderer::SetWorldMatrix(&world);

	CRenderer::SetVertexShader(SHADER_TYPE::Color);
	CRenderer::SetPixelShader(SHADER_TYPE::Color);
	CRenderer::SetInputLayout(SHADER_TYPE::Color);

	UINT stride = sizeof(VERTEX_LINE);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &pVB, &stride, &offset);

	// インデックスバッファ設定
	CRenderer::GetDeviceContext()->IASetIndexBuffer(pIB, DXGI_FORMAT_R32_UINT, 0);

	// プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// ポリゴン描画
	CRenderer::GetDeviceContext()->DrawIndexed(24, 0, 0);

	CRenderer::SetShader(SHADER_TYPE::Default);

	SAFE_RELEASE(pVB);
	SAFE_RELEASE(pIB);
}