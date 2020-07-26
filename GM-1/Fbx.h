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
	VERTEX_3D* vertex;
	//���_�C���f�b�N�X�̏���
	int* indexBuffer;
	//UVSet�̐�
	int							uvSetCount;
	//UVSet�̖��O
	std::string* uvSetName;
	//���_�o�b�t�@
	ID3D11Buffer*		pVB;
	//�C���f�b�N�X�o�b�t�@
	ID3D11Buffer*		pIB;
	//�e�N�X�`��
	LPDIRECT3DTEXTURE9* texture;
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
	std::string* uvSetName;
	//�}�e���A���̐F
	D3DXCOLOR* color;

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

