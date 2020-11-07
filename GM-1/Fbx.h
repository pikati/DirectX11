//CreateBuffer�n��bd�̓����o�ɂ��Ă����Ƒ����Ȃ肻��
#pragma once
#include "Component.h"
#include "main.h"
#include "renderer.h"
#include <vector>
#include <fbxsdk.h>
#include <memory>

class Animation;

struct MeshInfo
{
	//�|���S���̐�
	int							polygonCount;
	//���_�̐�
	int							vertexCount;
	//���_�C���f�b�N�X�̐�
	int							indexCount;
	//���_
	VERTEX_3D*					vertex;
	//���_�C���f�b�N�X�̏���
	int*						index;
	//UVSet�̐�
	int							uvSetCount;
	//UVSet�̖��O
	std::string*				uvSetName;
	//���_�o�b�t�@
	ID3D11Buffer*				pVB;
	//�C���f�b�N�X�o�b�t�@
	ID3D11Buffer*				pIB;
	//�e�N�X�`��
	ID3D11ShaderResourceView*	texture;
	//�e�N�X�`���p�X
	std::vector<std::string>	texturePath;
};

struct FbxInfo
{
	//���b�V��
	std::vector<FbxMesh*>			 meshes;
	//���b�V���̐�
	int								 meshCount;
	//�}�e���A��
	std::vector<FbxSurfaceMaterial*> material;
	//�}�e���A���̐�
	int								 materialCount;
	//UVSet�̐�
	int								 uvSetCount;
	//UVSet�̖��O
	std::string*					 uvSetName;
	//�}�e���A�����
	MATERIAL*						 materialInfo;

};
class Fbx : public Component
{
private:
	Animation* m_animation;
	//!fbx�̃}�l�[�W��
	FbxManager* m_manager = NULL;
	//!fbx�̃V�[��
	FbxScene* m_scene = NULL;
	//!fbx�̏��
	FbxInfo		m_fbxInfo;
	//!���b�V���̏��
	MeshInfo* m_meshInfo;

	//!�A�j���[�V�������邩
	bool		m_isAnim;
	//!�A�j���[�V�����̐�
	int				m_animationStackNumber;
	//!���낢��
	FbxTime			m_frameTime, m_timeCount, m_start, m_stop;

	std::vector<FbxSkin*>	m_skinDeformer;
	std::vector<FbxNode*>	m_link;

	ID3D11Device* m_device;

	//�e���b�V���̊e�t���[���̒��_�z���Vector3����
	//std::vector<std::vector<std::vector<D3DXVECTOR3>>> m_animVertex;
	Vector3*** m_animationVertex;
	//�A�j���[�V�����̔z��쐬���Ɏg�p
	int m_count;
	//�A�j���[�V�����Ǘ��Ɏg�p
	int m_frame;
	bool isPlay = false;
	static int m_maxID;

	std::string m_textureName;
	std::string m_fileName;
	ID3D11ShaderResourceView* texture;
	bool m_isCopy = false;

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
	void GetColor(int meshIndex);
	void GetUVSetName(int meshIndex);
	void GetUV(int meshIndex);
	void GetTextureInfo(int meshindex);
	void LoadTexture(int meshIndex);
	void LoadTextureSimple();
	void CreateVertexBuffer(int meshIndex);
	void CreateIndexBuffer(int meshIndex);
	void SetVertexBuffer(int meshIndex);//bd�����o��������Ӗ����邵�Ȃ�������Create�Ɠ���
	void TextureMemoryAllocate(int meshIndex);
	void InitializeAnimation();
	void SetAnimationVertex();
	void UpdateAnimationVertex();

	void DrawAnimation();
	void UpdateTime();

	/*�f�o�b�O�p�̃t�@�C���o��*/
	void PrintFile();
	void DrawAnimationFrame();
public:
	void Initialize();
	void Update();
	void Draw();
	void Finalize();
	void Load(const char* fileName);
	void Load();
	void SetTextureName(const char* fileName);
	void UpFrame();
	void DownFrame();
	void PlayAnimation();
	void SetFileName(const char* fileName);

	void LoadProperties(const rapidjson::Value& inProp) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp) override;
};

