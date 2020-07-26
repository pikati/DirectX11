#pragma once
#include "Component.h"
#include "main.h"
#include "renderer.h"
#include <vector>
#include <fbxsdk.h>

struct MeshInfo
{
	//ポリゴンの数
	int							polygonCount;
	//頂点の数
	int							vertexCount;
	//頂点インデックスの数
	int							indexCount;
	//頂点
	VERTEX_3D* vertex;
	//頂点インデックスの順番
	int* indexBuffer;
	//UVSetの数
	int							uvSetCount;
	//UVSetの名前
	std::string* uvSetName;
	//頂点バッファ
	ID3D11Buffer*		pVB;
	//インデックスバッファ
	ID3D11Buffer*		pIB;
	//テクスチャ
	LPDIRECT3DTEXTURE9* texture;
	//テクスチャパス
	std::vector<std::string>	texturePath;
};

struct FbxInfo
{
	//メッシュ
	std::vector<FbxMesh*>			 meshes;
	//メッシュの数
	int								 meshCount;
	//マテリアル
	std::vector<FbxSurfaceMaterial*> material;
	//マテリアルの数
	int								 materialCount;
	//UVSetの数
	int								 uvSetCount;
	//UVSetの名前
	std::string* uvSetName;
	//マテリアルの色
	D3DXCOLOR* color;

};
class Fbx : public Component
{
private:
	//!fbxのマネージャ
	FbxManager* m_manager = NULL;
	//!fbxのシーン
	FbxScene* m_scene = NULL;
	//!fbxの情報
	FbxInfo		m_fbxInfo;
	//!メッシュの情報
	MeshInfo* m_meshInfo;

	//!アニメーションするか
	bool		m_isAnim;
	//!アニメーションの数
	int				m_animationStackNumber;
	//!いろいろ
	FbxTime			m_frameTime, m_timeCount, m_start, m_stop;

	std::vector<FbxSkin*>	m_skinDeformer;
	std::vector<FbxNode*>	m_link;

	ID3D11Device* m_device;

	void LoadFBX(const char* fileName);
	void InitializeFBX();
	void MeshTrianglate();
	std::vector<FbxMesh*> GetMesh();
public:
	void Initialize();
	void Update();
	void Draw();
	void Finalize();
	void Load(const char* fileName);
};

