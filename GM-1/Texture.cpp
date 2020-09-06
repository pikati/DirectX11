#include "Texture.h"
#include <string>
#include "renderer.h"
#include "Transform.h"
#include "LevelLoader.h"
#include "ObjectPooler.h"

int Texture::m_maxID = -1;

void Texture::Initialize()
{
	m_maxID++;

	//テクスチャをプールしているところから同じIDのテクスチャが見つかったらそれ持ってきて代入する
	Texture* t = ObjectPooler::GetTexture(m_id);
	if (t != nullptr)
	{
		if (t->m_id == m_id)
		{
			m_fileName = t->m_fileName;
			m_texture = t->m_texture;
			m_sortingOrder = t->m_sortingOrder;
			m_maxID--;
			return;
		}
	}

	m_id = m_maxID;
	LoadTexture(m_fileName);
	ObjectPooler::SetComponent(this, m_fileName);
}

void Texture::Finalize()
{
	SAFE_RELEASE(m_texture);
}

void Texture::LoadTexture(std::string fileName)
{
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		m_fileName.c_str(),
		NULL,
		NULL,
		&m_texture,
		NULL);
}

void Texture::SetTextureName(std::string textureName)
{
	m_fileName = textureName;
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return m_texture;
}

void Texture::LoadProperties(const rapidjson::Value& inProp)
{
	JsonHelper::GetString(inProp, "fileName", m_fileName);
	JsonHelper::GetInt(inProp, "id", m_id);
}

void Texture::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp)
{
	std::string name = typeid(*this).name();
	JsonHelper::AddString(alloc, inProp, "type", name.substr(6).c_str());
	JsonHelper::AddString(alloc, inProp, "fileName", m_fileName);
	JsonHelper::AddInt(alloc, inProp, "id", m_id);
}

void Texture::SetID(int id)
{
	m_id = id;
}

int Texture::GetTextureID()
{
	return m_id;
}