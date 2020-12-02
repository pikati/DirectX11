#include "Image.h"
#include "renderer.h"
#include "Texture.h"
#include "ObjectPooler.h"
#include "LevelLoader.h"
#include "Transform.h"
#include "imGui/imgui.h"

void Image::Initialize()
{
	VERTEX_3D vertex[4];
	float width = m_width * 0.5f;
	float height = m_height * 0.5f;
	float x = gameObject->transform->position.x;
	float y = gameObject->transform->position.y;

	vertex[0].Position = D3DXVECTOR3(0 + x, 0 + y, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(m_width + x, 0 + y, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(0 + x, m_height + y, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(m_width + x, m_height + y, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_vertexBuffer);
	/*Texture* tex = ObjectPooler::GetTexture(m_textureName);
	if (tex == nullptr)
	{
		tex = new Texture();
		tex->SetTextureName(m_textureName);
		tex->Initialize();
		m_texture = tex->GetTexture();
	}*/
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		m_textureName.c_str(),
		NULL,
		NULL,
		&m_texture,
		NULL);
}

void Image::Update()
{
	VERTEX_3D vertex[4];
	float width = m_width * 0.5f;
	float height = m_height * 0.5f;
	float x = gameObject->transform->position.x;
	float y = gameObject->transform->position.y;

	vertex[0].Position = D3DXVECTOR3(0 + x, 0 + y, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(m_width + x, 0 + y, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(0 + x, m_height + y, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(m_width + x, m_height + y, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	D3D11_MAPPED_SUBRESOURCE pdata;
	CRenderer::GetDeviceContext()->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)vertex, sizeof(VERTEX_3D) * 4);
	CRenderer::GetDeviceContext()->Unmap(m_vertexBuffer, 0);
}

void Image::Draw()
{
	CRenderer::SetShader(SHADER_TYPE::Default);
	//マトリクス設定
	CRenderer::SetWorldViewProjection2D();

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);//頂点1個分のサイズ
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = m_color;
	CRenderer::SetMaterial(material);

	//テクスチャ設定
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_texture);//GetにするとGPUのテクスチャを取得する

	//プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	CRenderer::GetDeviceContext()->Draw(4, 0);
}

void Image::Finalize()
{
	SAFE_RELEASE(m_texture);
}

void Image::SetTextureName(std::string textureName)
{
	m_textureName = textureName;
}

void Image::SetSize(float width, float height)
{
	m_width = width;
	m_height = height;
}

void Image::SetColor(float r, float g, float b, float a)
{
	m_color = D3DXCOLOR(r, g, b, a);
}

void Image::LoadProperties(const rapidjson::Value& inProp)
{
	JsonHelper::GetString(inProp, "fileName", m_textureName);
	JsonHelper::GetFloat(inProp, "width", m_width);
	JsonHelper::GetFloat(inProp, "height", m_height);
	JsonHelper::GetInt(inProp, "id", m_id);
}

void Image::DrawInformation()
{
	std::string name = typeid(*this).name();

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(1000, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Once);
	ImGui::Begin(name.substr(6).c_str());
	char fname[256];
	strcpy_s(fname, m_textureName.c_str());
	ImGui::InputText("fileName", fname, sizeof(fname));
	m_textureName = fname;
	if (ImGui::Button("Reload"))
	{
		Finalize();
		Initialize();
	}
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void Image::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp)
{
	std::string name = typeid(*this).name();
	JsonHelper::AddString(alloc, inProp, "type", name.substr(6).c_str());
	JsonHelper::AddString(alloc, inProp, "fileName", m_textureName);
	JsonHelper::AddFloat(alloc, inProp, "width", m_width);
	JsonHelper::AddFloat(alloc, inProp, "height", m_height);
	JsonHelper::AddInt(alloc, inProp, "id", m_id);
}