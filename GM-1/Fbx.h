//CreateBuffer系のbdはメンバにしておくと早くなりそう
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
	VERTEX_3D*					vertex;
	//頂点インデックスの順番
	int*						index;
	//UVSetの数
	int							uvSetCount;
	//UVSetの名前
	std::string*				uvSetName;
	//頂点バッファ
	ID3D11Buffer*				pVB;
	//インデックスバッファ
	ID3D11Buffer*				pIB;
	//テクスチャ
	ID3D11ShaderResourceView*	texture;
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
	std::string*					 uvSetName;
	//マテリアルの色
	Vector4* color;

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

	//各メッシュの各フレームの各頂点配列のVector3だよ
	std::vector<std::vector<std::vector<D3DXVECTOR3>>> m_animVertex;
	//アニメーションの配列作成時に使用
	int m_count;
	//アニメーション管理に使用
	int m_frame;

	void LoadFBX(const char* fileName);
	void InitializeFBX();
	void MeshTrianglate();
	void InitializeUnit();
	void GetMesh();
	void SetInfomation();
	int  GetUVSetNum();
	void GetMaterial();
	int  GetPolygonCount(int meshIndex);
	int  GetVertexCount(int meshIndex);
	int	 GetIndexCount(int meshIndex);
	void GetVertex(int meshIndex);
	void GetNormal(int meshIndex);
	void GetUVSetName(int meshIndex);
	void GetUV(int meshIndex);
	void CreateVertexBuffer(int meshIndex);
	void CreateIndexBuffer(int meshIndex);
	void SetVertexBuffer(int meshIndex);
	void SetIndexBuffer(int meshIndex);
	void TextureMemoryAllocate(int meshIndex);
	void InitializeAnimation();
	void SetAnimationVertex();

	void DrawAnimation();
	void UpdateTime();
public:
	void Initialize();
	void Update();
	void Draw();
	void Finalize();
	void Load(const char* fileName);
};

