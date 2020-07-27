//CreateBuffer�n��bd�̓����o�ɂ��Ă����Ƒ����Ȃ肻��
#pragma once
#include "Component.h"
#include "main.h"
#include "renderer.h"
#include <vector>
#include <fbxsdk.h>

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
	//�}�e���A���̐F
	Vector4* color;

};
class Fbx : public Component
{
private:
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

	//�e���b�V���̊e�t���[���̊e���_�z���Vector3����
	std::vector<std::vector<std::vector<D3DXVECTOR3>>> m_animVertex;
	//�A�j���[�V�����̔z��쐬���Ɏg�p
	int m_count;
	//�A�j���[�V�����Ǘ��Ɏg�p
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

