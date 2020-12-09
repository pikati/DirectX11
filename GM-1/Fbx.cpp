#include "Fbx.h"
#include "GameObject.h"
#include "Transform.h"
#include <iostream>
#include <fstream>
#include <string>
#include "ImguiManager.h"
#include "Animation.h"
#include "LevelLoader.h"
#include <DirectXMath.h>
#include "ObjectPooler.h"
#include "BoundingBox.h"

#define FRAME 0.0166666666666667

int Fbx::m_maxID = -1;


void Fbx::Initialize()
{

	m_device = CRenderer::GetDevice();
	m_fbxInfo.uvSetCount = 0;
	m_sortingOrder = 0;
	if (m_bb == nullptr)
	{
		m_bb = new BoundingBox();
	}
	Load();
}

void Fbx::Update()
{
	if (gameObject->transform->rotation.y >= 360)
	{
		gameObject->transform->rotation.y -= 360;
	}
	if (m_animationStackNumber != 0)
	{
		if (m_animation == nullptr)
		{
			m_animation = gameObject->GetComponent<Animation>();
		}
		m_frame = m_animation->GetTime();
		UpdateAnimationVertex();
	}
}

void Fbx::Draw()
{
	DrawAnimationFrame();
	DrawAnimation();
}

void Fbx::Finalize()
{
	if (m_isCopy)
	{
		if (m_animationStackNumber != 0)
		{
			if (m_animationVertex != nullptr)
			{
				for (int i = 0; i < m_fbxInfo.meshCount; i++)
				{
					if (m_animationVertex[i] != nullptr)
						for (int j = 0; j < m_count; j++)
						{
							if (m_animationVertex[i][j] != nullptr)
							{
								m_animationVertex[i][j] = nullptr;
							}
						}
					m_animationVertex[i] = nullptr;
				}
				m_animationVertex = nullptr;
			}

		}


		for (int i = m_fbxInfo.meshCount - 1; i >= 0; i--)
		{
			if (m_meshInfo[i].vertex != nullptr)
			{
				m_meshInfo[i].vertex = nullptr;
			}

		}
		for (int i = m_fbxInfo.meshCount - 1; i >= 0; i--)
		{
			if (m_meshInfo[i].index != nullptr)
			{
				m_meshInfo[i].index = nullptr;
			}
		}
		for (int i = m_fbxInfo.meshCount - 1; i >= 0; i--)
		{;
			m_meshInfo[i].pVB = nullptr;
			m_meshInfo[i].pIB = nullptr;
		}
		/*if (m_meshInfo != nullptr)
			delete m_meshInfo;
		if (m_fbxInfo.uvSetName != nullptr)
			delete m_fbxInfo.uvSetName;*/
		if (m_bb != nullptr)
		{
			m_bb = nullptr;
		}
		return;
	}
	/*for (int i = m_fbxInfo.meshCount - 1; i >= 0; i--)
	{
		if(m_meshInfo[i].uvSetName != nullptr)
		delete 	m_meshInfo[i].uvSetName;
	}*/
	if (m_animationStackNumber != 0)
	{
		if (m_animationVertex != nullptr)
		{
			for (int i = 0; i < m_fbxInfo.meshCount; i++)
			{
				if (m_animationVertex[i] != nullptr)
					for (int j = 0; j < m_count; j++)
					{
						if (m_animationVertex[i][j] != nullptr)
						{
							delete[] m_animationVertex[i][j];
							m_animationVertex[i][j] = nullptr;
						}
					}
				delete[] m_animationVertex[i];
				m_animationVertex[i] = nullptr;
			}
			delete[] m_animationVertex;
			m_animationVertex = nullptr;
		}

	}


	for (int i = m_fbxInfo.meshCount - 1; i >= 0; i--)
	{
		if (m_meshInfo[i].vertex != nullptr)
		{
			delete m_meshInfo[i].vertex;
			m_meshInfo[i].vertex = nullptr;
		}

	}
	for (int i = m_fbxInfo.meshCount - 1; i >= 0; i--)
	{
		if (m_meshInfo[i].index != nullptr)
		{
			delete m_meshInfo[i].index;
			m_meshInfo[i].index = nullptr;
		}
	}
	for (int i = m_fbxInfo.meshCount - 1; i >= 0; i--)
	{
		SAFE_RELEASE(m_meshInfo[i].pVB);
		m_meshInfo[i].pVB = nullptr;
		SAFE_RELEASE(m_meshInfo[i].pIB);
		m_meshInfo[i].pIB = nullptr;
	}
	/*if (m_meshInfo != nullptr)
		delete m_meshInfo;
	if (m_fbxInfo.uvSetName != nullptr)
		delete m_fbxInfo.uvSetName;*/
	if (m_bb != nullptr)
	{
		delete m_bb;
		m_bb = nullptr;
	}
}

void Fbx::Load(const char* fileName)
{
	m_fileName = fileName;
	LoadFBX(fileName);
	InitializeFBX();
	InitializeAnimation();
	SetAnimationVertex();
}

void Fbx::Load()
{
	m_maxID++;
	Fbx* t = new Fbx();
	t = ObjectPooler::GetComponent(m_id, t);
	if (t != nullptr)
	{
		if (t->m_id == m_id)
		{
			m_animationStackNumber = t->m_animationStackNumber;
			if (m_animationStackNumber != 0)
			{
				m_animation = t->m_animation;
				m_animation->SetState(5);
				m_animationVertex = t->m_animationVertex;
			}
			m_count = t->m_count;
			m_fbxInfo = t->m_fbxInfo;
			m_fileName = t->m_fileName;
			m_frame = 0;
			m_frameTime = t->m_frameTime;
			m_isAnim = t->m_isAnim;
			m_link = t->m_link;
			m_meshInfo = t->m_meshInfo;
			m_sortingOrder = t->m_sortingOrder;
			m_start = t->m_start;
			m_stop = t->m_stop;
			m_textureName = t->m_textureName;
			m_timeCount = t->m_timeCount;
			texture = t->texture;
			m_bb = t->m_bb;
			gameObject->SetBoundingBox(m_bb);
			m_isCopy = true;
			m_maxID--;
			return;
		}
	}
	m_frame = 0;

	LoadFBX(m_fileName.c_str());
	InitializeFBX();
	InitializeAnimation();
	SetAnimationVertex();
	//m_id = m_maxID;
	ObjectPooler::SetComponent(this);
}

void Fbx::SetTextureName(const char* fileName)
{
	m_textureName = fileName;
}

void Fbx::LoadFBX(const char* fileName)
{
	m_manager = FbxManager::Create();

	if (NULL != m_manager)
	{
		//�C���|�[�^�[�쐬
		FbxImporter* importer = FbxImporter::Create(m_manager, "");

		if (NULL != importer)
		{
			//�V�[���쐬
			m_scene = FbxScene::Create(m_manager, "");

			if (NULL != m_scene)
			{
				//filePath�Ɏw�肵���t�@�C����ǂݍ���
				bool result = importer->Initialize(fileName);

				if (result)
				{
					//�V�[���ɃC���|�[�g�����t�@�C����n��
					importer->Import(m_scene);
				}
				else
				{
					MessageBox(NULL, "�t�@�C���p�X�Ԉ���Ă邩����", NULL, MB_OK);
				}
			}
		}
		//�C���|�[�^�[�̖�ڂ͏I���Ȃ̂ŉ������
		importer->Destroy();
	}
}

void Fbx::InitializeFBX()
{
	//MeshTrianglate();
	//InitializeUnit();
	GetMesh();
	SetInfomation();
}

void Fbx::MeshTrianglate()
{
	FbxGeometryConverter geometryConverter(m_manager);
	geometryConverter.Triangulate(m_scene, true);
}

void Fbx::InitializeUnit()
{
	FbxSystemUnit SceneSystemUnit = m_scene->GetGlobalSettings().GetSystemUnit();
	if (SceneSystemUnit.GetScaleFactor() != 1.0f)
	{
		// �Z���`���[�^�[�P�ʂɃR���o�[�g����B
		FbxSystemUnit::cm.ConvertScene(m_scene);
	}
}

void Fbx::GetMesh()
{
	//���b�V���̐����擾
	m_fbxInfo.meshCount = m_scene->GetSrcObjectCount<FbxMesh>();
	for (int i = 0; i < m_fbxInfo.meshCount; ++i)
	{
		//i�Ԗڂ̃��b�V�����擾
		m_fbxInfo.meshes.emplace_back(m_scene->GetSrcObject<FbxMesh>(i));
	}
}

void Fbx::SetInfomation()
{
	//�������m��
	m_fbxInfo.uvSetName = new std::string[GetUVSetNum()];
	m_meshInfo = new MeshInfo[m_fbxInfo.meshCount];
	GetMaterial();
	//���ꂼ��̒l�擾
	for (int meshIndex = 0; meshIndex < m_fbxInfo.meshCount; meshIndex++)
	{
		m_meshInfo[meshIndex].polygonCount = GetPolygonCount(meshIndex);
		m_meshInfo[meshIndex].vertexCount = GetVertexCount(meshIndex);
		m_meshInfo[meshIndex].indexCount = GetIndexCount(meshIndex);

		m_meshInfo[meshIndex].index = new int[m_meshInfo[meshIndex].indexCount];
		// �|���S�����̎擾
		int polygon_count = m_fbxInfo.meshes[meshIndex]->GetPolygonCount();
		// �|���S���̐������A�ԂƂ��ĕۑ�����
		for (int i = 0; i < polygon_count; i++)
		{
			m_meshInfo[meshIndex].index[i * 3] = i * 3 + 2;
			m_meshInfo[meshIndex].index[i * 3 + 1] = i * 3 + 1;
			m_meshInfo[meshIndex].index[i * 3 + 2] = i * 3;
		}

		/*int* idx = m_fbxInfo.meshes[meshIndex]->GetPolygonVertices();
		for (int i = 0; i < m_meshInfo[meshIndex].indexCount; i++) {
			m_meshInfo[meshIndex].index[i] = idx[i];
		}*/
		m_meshInfo[meshIndex].vertex = new VERTEX_3D[m_meshInfo[meshIndex].vertexCount];
		GetVertex(meshIndex);
		gameObject->SetBoundingBox(m_bb);
		GetNormal(meshIndex);
		GetColor(meshIndex);
		GetUVSetName(meshIndex);
		GetUV(meshIndex);

		//GetTextureInfo(meshIndex);
		//LoadTexture(meshIndex);
		CreateVertexBuffer(meshIndex);
		CreateIndexBuffer(meshIndex);
	}
	//PrintFile();
	LoadTextureSimple();
}

int Fbx::GetUVSetNum()
{
	int n = 0;
	for (int i = 0; i < m_fbxInfo.meshCount; i++)
	{
		n += m_fbxInfo.meshes[i]->GetUVLayerCount();

	}
	return n;
}

void Fbx::GetMaterial()
{
	//�}�e���A���̐����擾
	m_fbxInfo.materialCount = m_scene->GetMaterialCount();
	m_fbxInfo.materialInfo = new MATERIAL[m_fbxInfo.materialCount];
	m_fbxInfo.material.reserve(m_fbxInfo.materialCount);
	for (int i = 0; i < m_fbxInfo.materialCount; i++)
	{
		//�}�e���A�����擾
		m_fbxInfo.material.emplace_back(m_scene->GetMaterial(i));
		FbxProperty prop = m_fbxInfo.material[i]->FindProperty(FbxSurfaceMaterial::sAmbient);
		if (prop.IsValid())
		{
			FbxDouble3 color = prop.Get<FbxDouble3>();
			m_fbxInfo.materialInfo[i].Diffuse.r = 1.0f;
			m_fbxInfo.materialInfo[i].Diffuse.g = 1.0f;
			m_fbxInfo.materialInfo[i].Diffuse.b = 1.0f;
			m_fbxInfo.materialInfo[i].Diffuse.a = 1.0f;
			m_fbxInfo.materialInfo[i].Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			m_fbxInfo.materialInfo[i].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			m_fbxInfo.materialInfo[i].Emission = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			m_fbxInfo.materialInfo[i].Shininess = 0.0f;
			m_fbxInfo.materialInfo[i].Dummy[0] = 0.0f;
			m_fbxInfo.materialInfo[i].Dummy[1] = 0.0f;
			m_fbxInfo.materialInfo[i].Dummy[2] = 0.0f;
		}
	}
}

int Fbx::GetPolygonCount(int meshIndex)
{
	return m_fbxInfo.meshes[meshIndex]->GetPolygonCount();
}

int Fbx::GetVertexCount(int meshIndex)
{
	//vertexCounts
	int a = m_fbxInfo.meshes[meshIndex]->GetControlPointsCount();
	int b = m_fbxInfo.meshes[meshIndex]->GetPolygonVertexCount();
	//indexCounts
	int c = m_fbxInfo.meshes[meshIndex]->GetPolygonCount() * 3;
	int d = m_fbxInfo.meshes[meshIndex]->GetPolygonVertexCount();
	return /*m_fbxInfo.meshes[meshIndex]->GetControlPointsCount()*/b;
}

int Fbx::GetIndexCount(int meshIndex)
{
	return m_fbxInfo.meshes[meshIndex]->GetPolygonCount() * 3;
}

void Fbx::GetVertex(int meshIndex)
{
	int i = 0;
	//���b�V���Ɋ܂܂�钸�_���W���擾�s�J�^�񗴂������̕��@���ƒ��_���W�͂���������UV�������
	/*FbxVector4* vtx = m_fbxInfo.meshes[meshIndex]->GetControlPoints();
	for (; i < m_meshInfo[meshIndex].vertexCount; i++)
	{
		FbxVector4 point = m_fbxInfo.meshes[meshIndex]->GetControlPointAt(i);
		m_meshInfo[meshIndex].vertex[i].Position.x = -(float)point[0];
		m_meshInfo[meshIndex].vertex[i].Position.y = (float)point[1];
		m_meshInfo[meshIndex].vertex[i].Position.z = (float)point[2];
	}*/
	//�������̕��@���ƒ��_���W�͕���邯��UV�͂���
	// ���_�o�b�t�@�̎擾
	FbxVector4* vertices = m_fbxInfo.meshes[meshIndex]->GetControlPoints();
	// �C���f�b�N�X�o�b�t�@�̎擾
	int* indices = m_fbxInfo.meshes[meshIndex]->GetPolygonVertices();
	// ���_���W�̐��̎擾
	int polygon_vertex_count = m_fbxInfo.meshes[meshIndex]->GetPolygonVertexCount();
	// GetPolygonVertexCount => ���_��
	for (; i < polygon_vertex_count; i++)
	{
		// �C���f�b�N�X�o�b�t�@���璸�_�ԍ����擾
		int index = indices[i];

		Vector3 v = { (float)-vertices[index][0], (float)vertices[index][1], (float)vertices[index][2] };
		// ���_���W���X�g������W���擾����
		m_meshInfo[meshIndex].vertex[i].Position.x = v.x;
		m_meshInfo[meshIndex].vertex[i].Position.y = v.y;
		m_meshInfo[meshIndex].vertex[i].Position.z = v.z;

		if (m_bb->m_max.x < v.x)
		{
			m_bb->m_max.x = v.x;
		}
		if (m_bb->m_max.y < v.y)
		{
			m_bb->m_max.y = v.y;
		}
		if (m_bb->m_max.z < v.z)
		{
			m_bb->m_max.z = v.z;
		}

		if (m_bb->m_min.x > v.x)
		{
			m_bb->m_min.x = v.x;
		}
		if (m_bb->m_min.y > v.y)
		{
			m_bb->m_min.y = v.y;
		}
		if (m_bb->m_min.z > v.z)
		{
			m_bb->m_min.z = v.z;
		}
	}

	using namespace DirectX;
	D3DXMATRIX mat, rotation;
	D3DXMatrixRotationYawPitchRoll(&rotation, XMConvertToRadians(gameObject->transform->rotation.y), XMConvertToRadians(gameObject->transform->rotation.x), XMConvertToRadians(gameObject->transform->rotation.z));
	mat._11 = gameObject->transform->scale.x * rotation._11;
	mat._12 = gameObject->transform->scale.x * rotation._12;
	mat._13 = gameObject->transform->scale.x * rotation._13;
	mat._21 = gameObject->transform->scale.y * rotation._21;
	mat._22 = gameObject->transform->scale.y * rotation._22;
	mat._23 = gameObject->transform->scale.y * rotation._23;
	mat._31 = gameObject->transform->scale.z * rotation._31;
	mat._32 = gameObject->transform->scale.z * rotation._32;
	mat._33 = gameObject->transform->scale.z * rotation._33;
	mat._41 = gameObject->transform->position.x;
	mat._42 = gameObject->transform->position.y;
	mat._43 = gameObject->transform->position.z;
	mat._14 = 0;
	mat._24 = 0;
	mat._34 = 0;
	mat._44 = 1.0f;
	Vector3 a = m_bb->m_max;
	Vector3 b = m_bb->m_min;
	m_bb->m_max.x = mat._11 * a.x + mat._12 * a.y + mat._13 * a.z + mat._41;
	m_bb->m_max.y = mat._21 * a.x + mat._22 * a.y + mat._23 * a.z + mat._42;
	m_bb->m_max.z = mat._31 * a.x + mat._32 * a.y + mat._33 * a.z + mat._43;
	m_bb->m_min.x = mat._11 * b.x + mat._12 * b.y + mat._13 * b.z + mat._41;
	m_bb->m_min.y = mat._21 * b.x + mat._22 * b.y + mat._23 * b.z + mat._42;
	m_bb->m_min.z = mat._31 * b.x + mat._32 * b.y + mat._33 * b.z + mat._43;
}

void Fbx::GetNormal(int meshIndex)
{
	////�s�J�^��
	//FbxArray<FbxVector4> normals;
	////�@�����擾
	//m_fbxInfo.meshes[meshIndex]->GetPolygonVertexNormals(normals);
	////�@���̐����擾
	//int normalCount = normals.Size();
	//for (int i = 0; i < normalCount; i++)
	//{
	//	//���_�C���f�b�N�X�ɑΉ��������_�ɒl����
	//	m_meshInfo[meshIndex].vertex[i].Normal.x = -normals[i][0];
	//	m_meshInfo[meshIndex].vertex[i].Normal.y = normals[i][1];
	//	m_meshInfo[meshIndex].vertex[i].Normal.z = normals[i][2];
	//}
	//--- �@���Z�b�g�����擾 ---//
	int normalLayerCount = m_fbxInfo.meshes[meshIndex]->GetElementNormalCount();

	//--- ���C���[��������� ---//
	for (int i = 0; normalLayerCount > i; i++)
	{
		//--- �@���Z�b�g���擾 ---//
		FbxGeometryElementNormal* normal = m_fbxInfo.meshes[meshIndex]->GetElementNormal(i);

		//--- �}�b�s���O���[�h�̎擾
		FbxGeometryElement::EMappingMode mapping = normal->GetMappingMode();
		//--- ���t�@�����X���[�h�̎擾 ---//
		FbxGeometryElement::EReferenceMode reference = normal->GetReferenceMode();

		//--- �}�b�s���O���[�h�̔��� ---//
		switch (mapping)
		{
		case FbxGeometryElement::eByControlPoint:
			//--- ���t�@�����X���[�h�̔��� ---//
			switch (reference)
			{
			case FbxGeometryElement::eDirect:
			{
				//--- �@�������擾 ---//
				int normalCount = normal->GetDirectArray().GetCount();

				//-----------------------------------------------------------------------
				// eDirect �̏ꍇ�f�[�^�͏��ԂɊi�[����Ă���̂ł��̂܂ܕێ�
				//-----------------------------------------------------------------------
				for (int i = 0; normalCount > i; i++)
				{
					//--- �@���̎擾 ---//
					m_meshInfo[meshIndex].vertex[i].Normal.x = -(float)normal->GetDirectArray().GetAt(i)[0];
					m_meshInfo[meshIndex].vertex[i].Normal.y = (float)normal->GetDirectArray().GetAt(i)[1];
					m_meshInfo[meshIndex].vertex[i].Normal.z = (float)normal->GetDirectArray().GetAt(i)[2];
				}
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
				break;

			default:
				break;
			}

			break;
		case FbxGeometryElement::eByPolygonVertex:
			switch (reference)
			{
			case FbxGeometryElement::eDirect:
			{
				//--- �@�������擾 ---//
				int normalCount = normal->GetDirectArray().GetCount();

				//-----------------------------------------------------------------------
				// eDirect �̏ꍇ�f�[�^�͏��ԂɊi�[����Ă���̂ł��̂܂ܕێ�
				//-----------------------------------------------------------------------
				for (int i = 0; normalCount > i; i++)
				{
					//--- �@���̎擾 ---//
					m_meshInfo[meshIndex].vertex[i].Normal.x = -(float)normal->GetDirectArray().GetAt(i)[0];
					m_meshInfo[meshIndex].vertex[i].Normal.y = (float)normal->GetDirectArray().GetAt(i)[1];
					m_meshInfo[meshIndex].vertex[i].Normal.z = (float)normal->GetDirectArray().GetAt(i)[2];
				}
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
				break;

			default:
				break;
			}

			break;
		default:
			break;
		}
	}
}

void Fbx::GetColor(int meshIndex)
{
	for (int vIdx = 0; vIdx < m_meshInfo[meshIndex].vertexCount; vIdx++)
	{
		m_meshInfo[meshIndex].vertex[vIdx].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

void Fbx::GetUVSetName(int meshIndex)
{
	FbxStringList uvsetName;
	//���b�V���Ɋ܂܂��UVSet�������ׂĎ擾
	m_fbxInfo.meshes[meshIndex]->GetUVSetNames(uvsetName);
	//UVSet�̐����擾
	m_meshInfo[meshIndex].uvSetCount = m_fbxInfo.meshes[meshIndex]->GetUVLayerCount();
	/*�e�N�X�`���֌W�̃������m��*/
	TextureMemoryAllocate(meshIndex);
	for (int i = 0; i < m_meshInfo[meshIndex].uvSetCount; i++)
	{
		//UVSet�����擾
		m_meshInfo[meshIndex].uvSetName[i] = uvsetName[i];
	}
}

void Fbx::GetUV(int meshIndex)
{
	FbxArray<FbxVector2> uvsets;
	FbxStringList uvsetName;
	m_fbxInfo.meshes[meshIndex]->GetUVSetNames(uvsetName);
	//UV���擾
	m_fbxInfo.meshes[meshIndex]->GetPolygonVertexUVs(uvsetName.GetStringAt(0), uvsets);
	//UV�̐����擾
	int uvsetCount = uvsets.Size();
	for (int i = 0; i < m_meshInfo[meshIndex].vertexCount; i++)
	{
		//���_�C���f�b�N�X�ɑΉ��������_�ɒl����
		m_meshInfo[meshIndex].vertex[/*m_meshInfo[meshIndex].index[i]*/i].TexCoord.x = uvsets[i][0];
		m_meshInfo[meshIndex].vertex[/*m_meshInfo[meshIndex].index[i]*/i].TexCoord.y = 1.0f - uvsets[i][1];
	}
}

void Fbx::GetTextureInfo(int meshIndex)
{
	int uvIndex = 0;
	int currentIndex = m_fbxInfo.uvSetCount;
	for (int matIndex = 0; matIndex < m_fbxInfo.materialCount; matIndex++)
	{
		//diffuse�̏����擾
		FbxProperty prop = m_fbxInfo.material[matIndex]->FindProperty(FbxSurfaceMaterial::sDiffuse);
		//���C���e�N�X�`���̐����擾����
		int layeredTextureCount = prop.GetSrcObjectCount<FbxLayeredTexture>();

		//���C���e�N�X�`���𗘗p���Ă���ꍇ
		if (0 < layeredTextureCount)
		{
			for (int j = 0; layeredTextureCount > j; j++)
			{

				//���C���e�N�X�`�����擾����
				FbxLayeredTexture* layeredTexture = prop.GetSrcObject<FbxLayeredTexture>(j);
				//�e�N�X�`���̐����擾����
				int textureCount = layeredTexture->GetSrcObjectCount<FbxFileTexture>();

				for (int k = 0; textureCount > k; k++)
				{
					//�e�N�X�`�����擾����
					FbxFileTexture* texture = prop.GetSrcObject<FbxFileTexture>(k);

					if (texture)
					{
						//�e�N�X�`�������擾����
						std::string textureName = texture->GetRelativeFileName();

						//UVSet�����擾����
						std::string UVSetName = texture->UVSet.Get().Buffer();

						//UVSet�����r���Ή����Ă���e�N�X�`���Ȃ�ێ�����
						for (int i = 0; i < m_meshInfo[meshIndex].uvSetCount; i++)
						{
							if (m_meshInfo[meshIndex].uvSetName[i] == UVSetName)
							{
								//�����Ɛݒ肵�Ă��Ȃ��̂Ńt�@�C���܂ł̃p�X��ǉ����Ă��܂�
								std::string a = "Models/test/";
								m_meshInfo[meshIndex].texturePath.emplace_back(a + textureName);
								//�e�N�X�`����UVSet�����擾����
								m_fbxInfo.uvSetName[uvIndex + currentIndex] = UVSetName;
								uvIndex++;
							}
						}

					}
				}
			}
		}
		//���C���e�N�X�`���𗘗p���Ă��Ȃ��ꍇ
		else
		{
			//�e�N�X�`�������擾����
			int fileTextureCount = prop.GetSrcObjectCount<FbxFileTexture>();

			if (0 < fileTextureCount)
			{
				for (int j = 0; fileTextureCount > j; j++)
				{
					//�e�N�X�`�����擾����
					FbxFileTexture* texture = prop.GetSrcObject<FbxFileTexture>(j);
					if (texture)
					{
						//�e�N�X�`�������擾����
						std::string textureName = texture->GetFileName();
						textureName = textureName.substr(textureName.find_last_of('/') + 1);
						textureName = "Asset/Texture/" + textureName;
						//std::string textureName = texture->GetRelativeFileName();

						//UVSet�����擾����
						std::string UVSetName = texture->UVSet.Get().Buffer();

						//UVSet�����r���Ή����Ă���e�N�X�`���Ȃ�ێ�����
						for (int i = 0; i < m_meshInfo[meshIndex].uvSetCount; i++)
						{
							int count = 0;
							if (m_meshInfo[meshIndex].uvSetName[i] == UVSetName)
							{
								count++;
								int j;
								for (j = 0; j < currentIndex + count; j++)
								{
									if (m_fbxInfo.uvSetName[j] == UVSetName)
									{
										break;
									}
								}
								if (j == currentIndex + count)
								{
									m_meshInfo[meshIndex].texturePath.emplace_back(textureName);
									m_fbxInfo.uvSetName[uvIndex + currentIndex] = UVSetName;
									uvIndex++;
								}
							}
						}
					}
				}
			}
		}
	}
	//UVSet�̐����擾����
	m_fbxInfo.uvSetCount += uvIndex;
}

void Fbx::LoadTexture(int meshIndex)
{
	for (int i = 0; i < m_meshInfo[meshIndex].uvSetCount; i++)
	{
		if (m_meshInfo[meshIndex].texturePath.size() > 0)
		{
			D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
				m_meshInfo[meshIndex].texturePath[i].c_str(),
				NULL,
				NULL,
				&m_meshInfo[meshIndex].texture,
				NULL);

			assert(m_meshInfo[meshIndex].texture);
		}
	}
}

void Fbx::LoadTextureSimple()
{
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		m_textureName.c_str(),
		NULL,
		NULL,
		&texture,
		NULL);
}

void Fbx::TextureMemoryAllocate(int meshIndex)
{
	m_meshInfo[meshIndex].uvSetName = new std::string[m_meshInfo[meshIndex].uvSetCount];
	m_meshInfo[meshIndex].texturePath.reserve(m_meshInfo[meshIndex].uvSetCount);
	//m_meshInfo[meshIndex].texture = new ID3D11ShaderResourceView[m_meshInfo[meshIndex].uvSetCount];
}

void Fbx::CreateVertexBuffer(int meshIndex)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * m_meshInfo[meshIndex].vertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = m_meshInfo[meshIndex].vertex;
	sd.SysMemPitch = 0;
	sd.SysMemSlicePitch = 0;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_meshInfo[meshIndex].pVB);
}

void Fbx::CreateIndexBuffer(int meshIndex)
{
	//�����o���ł�����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned int) * m_meshInfo[meshIndex].indexCount;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = m_meshInfo[meshIndex].index;
	sd.SysMemPitch = 0;
	sd.SysMemSlicePitch = 0;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_meshInfo[meshIndex].pIB);
}

void Fbx::SetVertexBuffer(int meshIndex)
{
	D3D11_MAPPED_SUBRESOURCE pdata;
	CRenderer::GetDeviceContext()->Map(m_meshInfo[meshIndex].pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(m_meshInfo[meshIndex].vertex), sizeof(VERTEX_3D) * m_meshInfo[meshIndex].vertexCount);
	CRenderer::GetDeviceContext()->Unmap(m_meshInfo[meshIndex].pVB, 0);
}

void Fbx::InitializeAnimation()
{
	/*�A�j���[�V�����̑I������ѐݒ�*/
	FbxArray<FbxString*> AnimStackNameArray;
	//�A�j���[�V�����̖��O�ꗗ���擾
	m_scene->FillAnimStackNameArray(AnimStackNameArray);
	m_animationStackNumber = m_scene->GetSrcObjectCount<FbxAnimStack>();
	//m_animationStackNumber�Ԗڂ̃A�j���[�V���������擾
	if (m_animationStackNumber == 0)
	{
		//MessageBox(NULL, "�A�j���[�V�����Ȃ���ł�����", NULL, MB_OK);
		return;
	}
	FbxAnimStack* AnimationStack = m_scene->FindMember<FbxAnimStack>(AnimStackNameArray[m_animationStackNumber - 1]->Buffer());
	//�A�j���[�V������ݒ�
	m_scene->SetCurrentAnimationStack(AnimationStack);

	/*�A�j���[�V�������s����ŕK�v�Ȏ��Ԋ֌W�̒l���擾*/
	//�A�j���[�V�������̎擾
	FbxTakeInfo* takeInfo = m_scene->GetTakeInfo(*(AnimStackNameArray[m_animationStackNumber - 1]));
	//�J�n����
	m_start = takeInfo->mLocalTimeSpan.GetStart();
	//�I������
	m_stop = takeInfo->mLocalTimeSpan.GetStop();
	//�A�j���[�V�����P�R�}�����s����Ă������ԏ���ێ����� ���Ԃ̃J�E���g���@��ݒ�
	m_frameTime.SetTime(0, 0, 0, 1, 0, m_scene->GetGlobalSettings().GetTimeMode());
	m_timeCount = m_start;

	m_animation = gameObject->GetComponent<Animation>();
}

void Fbx::SetAnimationVertex()
{
	if (m_animationStackNumber == 0)
	{
		return;
	}
	double animationTime = m_start.GetSecondDouble();
	double stopTime = m_stop.GetSecondDouble();
	int count = 0;
	m_timeCount = m_start;
	//m_animVertex.resize(m_fbxInfo.meshCount);
	m_animationVertex = new Vector3 * *[m_fbxInfo.meshCount];
	while (animationTime <= stopTime)
	{
		count++;
		animationTime += FRAME;
	}

	m_count = count;

	for (int i = 0; i < m_fbxInfo.meshCount; i++)
	{
		//m_animVertex[i].resize(count);
		m_animationVertex[i] = new Vector3 * [count];
		for (int j = 0; j < count; j++)
		{
			//m_animVertex[i][j].resize(m_meshInfo[i].vertexCount);
			m_animationVertex[i][j] = new Vector3[m_meshInfo[i].vertexCount];
		}
	}

	int animCount = 0;

	while (animCount < count)
	{
		FbxNode* pNode = m_scene->GetRootNode();//0.0029
	// <�ړ��A��]�A�g��̂��߂̍s����쐬>
		FbxMatrix globalPosition = pNode->EvaluateGlobalTransform(m_timeCount);//0.6313
		FbxVector4 t0 = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);//0.0024
		FbxVector4 r0 = pNode->GetGeometricRotation(FbxNode::eSourcePivot);//0.0020
		FbxVector4 s0 = pNode->GetGeometricScaling(FbxNode::eSourcePivot);//0.0017
		FbxAMatrix geometryOffset = FbxAMatrix(t0, r0, s0);//0.0081
		for (int i = 0; i < m_fbxInfo.meshCount; i++)
		{

			// <�e���_�Ɋ|���邽�߂̍ŏI�I�ȍs��̔z��>

			FbxMatrix* clusterDeformation = new FbxMatrix[m_meshInfo[i].vertexCount];//1.9053
			memset(clusterDeformation, 0, sizeof(FbxMatrix) * m_meshInfo[i].vertexCount);//0.0167
			//�S�ẴN���X�^��񂪓���
			FbxSkin* skinDeformer = (FbxSkin*)m_fbxInfo.meshes[i]->GetDeformer(0, FbxDeformer::eSkin);//0.0220
			if (skinDeformer)
			{
				int clusterCount = skinDeformer->GetClusterCount();//0.0492
				// <�e�N���X�^����e���_�ɉe����^���邽�߂̍s��쐬>
				for (int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++)
				{
					// <�N���X�^(�{�[��)�̎��o��>
					FbxCluster* cluster = skinDeformer->GetCluster(clusterIndex);//0.0321
					FbxMatrix vertexTransformMatrix;//0.0063
					FbxAMatrix referenceGlobalInitPosition;//0.0042
					FbxAMatrix clusterGlobalInitPosition;//0.0042
					FbxMatrix clusterGlobalCurrentPosition;//0.0063
					FbxMatrix clusterRelativeInitPosition;//0.0039
					FbxMatrix clusterRelativeCurrentPositionInverse;//0.0095
					cluster->GetTransformMatrix(referenceGlobalInitPosition);//0.0595
					referenceGlobalInitPosition *= geometryOffset;//0.0035
					cluster->GetTransformLinkMatrix(clusterGlobalInitPosition);//0.0566
					clusterGlobalCurrentPosition = cluster->GetLink()->EvaluateGlobalTransform(m_timeCount);//2.0852
					clusterRelativeInitPosition = clusterGlobalInitPosition.Inverse() * referenceGlobalInitPosition;//0.0378
					clusterRelativeCurrentPositionInverse = globalPosition.Inverse() * clusterGlobalCurrentPosition;//0.0705
					vertexTransformMatrix = clusterRelativeCurrentPositionInverse * clusterRelativeInitPosition;//0.0203
					// <��ō�����s��Ɋe���_���̉e���x(�d��)���|���Ă��ꂼ��ɉ��Z>
					for (int cnt = 0; cnt < cluster->GetControlPointIndicesCount(); cnt++)
					{
						int index = cluster->GetControlPointIndices()[cnt];//0.0019
						double weight = cluster->GetControlPointWeights()[cnt];//0.0013
						FbxMatrix influence = vertexTransformMatrix * weight;//0.0058
						clusterDeformation[index] += influence;//0.0020
					}
				}


				// <�ŏI�I�Ȓ��_���W���v�Z��VERTEX�ɕϊ�>
				for (int cnt = 0; cnt < m_meshInfo[i].vertexCount; cnt++)
				{
					//�����l�����ɂ��ƕ��ѕς���O�̒��_���W�ɑ΂��ĕϊ�����̂ŁA���ёւ�����̒��_���W�ɑΉ������邽�߂̏���
					for (int n = 0; n < m_fbxInfo.meshes[i]->GetControlPointsCount(); n++)
					{
						FbxVector4 pos = m_fbxInfo.meshes[i]->GetControlPointAt(n);
						Vector3 a;
						a.Set(float(pos[0]), float(pos[1]), float(pos[2]));
						Vector3 vPos;
						vPos.Set(m_meshInfo[i].vertex[cnt].Position.x, m_meshInfo[i].vertex[cnt].Position.y, m_meshInfo[i].vertex[cnt].Position.z);
						if (fabs(float(pos[0]) + vPos.x) < 0.01f && fabs(float(pos[1]) - vPos.y) < 0.01f && fabs(float(pos[2]) - vPos.z) < 0.01f)
						{
							FbxVector4 v;
							v.Set(-m_meshInfo[i].vertex[cnt].Position.x, m_meshInfo[i].vertex[cnt].Position.y, m_meshInfo[i].vertex[cnt].Position.z);
							FbxVector4 outVertex = clusterDeformation[n].MultNormalize(v);
							float x = (FLOAT)outVertex[0];
							float y = (FLOAT)outVertex[1];
							float z = (FLOAT)outVertex[2];

							m_animationVertex[i][animCount][cnt] = (Vector3(-x, y, z));
							break;
						}

					}

				}
				delete[] clusterDeformation;
			}
		}
		double timeCount = m_timeCount.GetSecondDouble();
		timeCount += FRAME;
		m_timeCount.SetSecondDouble(timeCount);
		animCount++;
	}
}

void Fbx::DrawAnimation()
{

	//UpdateTime();
	using namespace DirectX;
	D3DXMATRIX world, rotation;
	D3DXMatrixRotationYawPitchRoll(&rotation, XMConvertToRadians(gameObject->transform->rotation.y), XMConvertToRadians(gameObject->transform->rotation.x), XMConvertToRadians(gameObject->transform->rotation.z));
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

	CRenderer::SetShader(SHADER_TYPE::Default);

	for (int i = 0; i < m_fbxInfo.meshCount; i++)
	{
		SetVertexBuffer(i);
		// ���_�o�b�t�@�ݒ�
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_meshInfo[i].pVB, &stride, &offset);

		// �C���f�b�N�X�o�b�t�@�ݒ�
		CRenderer::GetDeviceContext()->IASetIndexBuffer(m_meshInfo[i].pIB, DXGI_FORMAT_R32_UINT, 0);

		// �v���~�e�B�u�g�|���W�ݒ�
		CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = m_fbxInfo.materialInfo[i].Diffuse;
		material.Ambient = m_fbxInfo.materialInfo[i].Ambient;
		CRenderer::SetMaterial(material);
		// �}�e���A���ݒ�
		//CRenderer::SetMaterial(m_fbxInfo.materialInfo[i]);

		// �e�N�X�`���ݒ�
		CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);

		// �|���S���`��
		CRenderer::GetDeviceContext()->DrawIndexed(m_meshInfo[i].indexCount, 0, 0);
	}
	//DrawNormal();
}

void Fbx::DrawNormal()
{
	int vCount = 0;
	for (int i = 0; i < m_fbxInfo.meshCount; i++)
	{
		vCount += m_meshInfo->vertexCount;
	}

	VERTEX_LINE* v = new VERTEX_LINE[vCount * 2];
	ID3D11Buffer* b;
	int count = 0;

	for (int i = 0; i < m_fbxInfo.meshCount; i++)
	{
		for (int j = 0; j < m_meshInfo[i].vertexCount; j++)
		{
			Vector3 n = { m_meshInfo->vertex[j].Normal.x, m_meshInfo->vertex[j].Normal.y, m_meshInfo->vertex[j].Normal.z };
			v[count].Position = { m_meshInfo->vertex[j].Position.x, m_meshInfo->vertex[j].Position.y, m_meshInfo->vertex[j].Position.z };
			v[count++].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
			v[count].Position = v[count - 1].Position + n;
			v[count++].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
		}
	}

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_LINE) * vCount * 2;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = v;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &b);

	UINT stride = sizeof(VERTEX_LINE);
	UINT offset = 0;
	CRenderer::SetShader(SHADER_TYPE::Color);
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &b, &stride, &offset);

	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	CRenderer::GetDeviceContext()->Draw(vCount * 2, 0);
	CRenderer::SetShader(SHADER_TYPE::Default);
	SAFE_RELEASE(b);
	delete[] v;
}

void Fbx::UpdateTime()
{
	if (m_animationStackNumber == 0)
	{
		return;
	}
	if (isPlay)
	{
		//�A�j���[�V�����̎��ԍX�V
		m_timeCount += m_frameTime;
		if (m_timeCount > m_stop)
		{
			//���s�����������ɂȂ���^�C�~���O
			m_timeCount = m_stop * 3 / 28;
		}
		m_frame++;
		if (m_frame >= m_count)
		{
			m_frame = 0;
		}
	}
	/*else
	{
		m_timeCount = m_start;
		m_frame = 0;
	}*/

}

void Fbx::UpdateAnimationVertex()
{
	m_bb->m_max = { 0,0,0 };
	m_bb->m_min = { 0,0,0 };
	for (int i = 0; i < m_fbxInfo.meshCount; i++)
	{
		for (int j = 0; j < m_meshInfo[i].vertexCount; j++)
		{
			float x = m_animationVertex[i][m_frame][j].x;
			float y = m_animationVertex[i][m_frame][j].y;
			float z = m_animationVertex[i][m_frame][j].z;
			m_meshInfo[i].vertex[j].Position = D3DXVECTOR3(x, y, z);
			if (m_bb->m_max.x < x)
			{
				m_bb->m_max.x = x;
			}
			if (m_bb->m_max.y < y)
			{
				m_bb->m_max.y = y;
			}
			if (m_bb->m_max.z < z)
			{
				m_bb->m_max.z = z;
			}

			if (m_bb->m_min.x > x)
			{
				m_bb->m_min.x = x;
			}
			if (m_bb->m_min.y > y)
			{
				m_bb->m_min.y = y;
			}
			if (m_bb->m_min.z > z)
			{
				m_bb->m_min.z = z;
			}
		}
	}
	using namespace DirectX;
	D3DXMATRIX mat, rotation;
	D3DXMatrixRotationYawPitchRoll(&rotation, XMConvertToRadians(-gameObject->transform->rotation.y), XMConvertToRadians(gameObject->transform->rotation.x), XMConvertToRadians(gameObject->transform->rotation.z));
	mat._11 = gameObject->transform->scale.x * rotation._11;
	mat._12 = gameObject->transform->scale.x * rotation._12;
	mat._13 = gameObject->transform->scale.x * rotation._13;
	mat._21 = gameObject->transform->scale.y * rotation._21;
	mat._22 = gameObject->transform->scale.y * rotation._22;
	mat._23 = gameObject->transform->scale.y * rotation._23;
	mat._31 = gameObject->transform->scale.z * rotation._31;
	mat._32 = gameObject->transform->scale.z * rotation._32;
	mat._33 = gameObject->transform->scale.z * rotation._33;
	mat._41 = gameObject->transform->position.x;
	mat._42 = gameObject->transform->position.y;
	mat._43 = gameObject->transform->position.z;
	mat._14 = 0;
	mat._24 = 0;
	mat._34 = 0;
	mat._44 = 1.0f;
	Vector3 a = m_bb->m_max;
	Vector3 b = m_bb->m_min;
	m_bb->m_max.x = mat._11 * a.x + mat._12 * a.y + mat._13 * a.z + mat._41;
	m_bb->m_max.y = mat._21 * a.x + mat._22 * a.y + mat._23 * a.z + mat._42;
	m_bb->m_max.z = mat._31 * a.x + mat._32 * a.y + mat._33 * a.z + mat._43;
	m_bb->m_min.x = mat._11 * b.x + mat._12 * b.y + mat._13 * b.z + mat._41;
	m_bb->m_min.y = mat._21 * b.x + mat._22 * b.y + mat._23 * b.z + mat._42;
	m_bb->m_min.z = mat._31 * b.x + mat._32 * b.y + mat._33 * b.z + mat._43;
	gameObject->SetBoundingBox(m_bb);
}

void Fbx::PrintFile()
{
	std::string filename = "vertex.txt";
	std::ofstream writing_file;
	writing_file.open(filename, std::ios::out);
	int count = 0;

	writing_file << "���X�����ق�" << std::endl;
	for (int i = 0; i < m_meshInfo[0].vertexCount; i++)
	{
		writing_file << "x:" << m_meshInfo[0].vertex[i].Position.x << " y:" << m_meshInfo[0].vertex[i].Position.y << " z:" << m_meshInfo[0].vertex[i].Position.z << std::endl;
		//writing_file << "x:" << m_meshInfo[0].index[i] << std::endl;
	}
	writing_file << "���_���Ȃ��ق�" << std::endl;
	for (int i = 0; i < m_fbxInfo.meshes[0]->GetControlPointsCount(); i++)
	{
		FbxVector4 v = m_fbxInfo.meshes[0]->GetControlPointAt(i);
		float x = float(v[0]);
		float y = float(v[1]);
		float z = float(v[2]);
		writing_file << "x:" << x << " y:" << y << " z:" << z << std::endl;
	}
}


void Fbx::UpFrame()
{
	m_frame++;
	if (m_frame >= m_count)
	{
		m_frame = 0;
	}
}

void Fbx::DownFrame()
{
	m_frame--;
	if (m_frame < 0)
	{
		m_frame = m_count - 1;
	}
}

void Fbx::DrawAnimationFrame()
{
	/*ImGui::Begin("Animation Window");
	ImGui::Text("Animation Frame :%d", m_frame);
	ImGui::Text("Animation Max Frame :%d", m_count);
	ImGui::End();*/
}

void Fbx::PlayAnimation()
{
	isPlay = !isPlay;
}

void Fbx::SetFileName(const char* fileName)
{
	m_fileName = fileName;
}

VERTEX_3D* Fbx::GetVertices(int meshNum)
{
	return m_meshInfo[meshNum].vertex;
}

int Fbx::GetVerticesCount(int meshNum)
{
	return m_meshInfo[meshNum].vertexCount;
}

int* Fbx::GetIndeces(int meshNum)
{
	return m_meshInfo[meshNum].index;
}

int Fbx::GetIndecesCount(int meshNum)
{
	return m_meshInfo[meshNum].indexCount;
}

ID3D11ShaderResourceView* Fbx::GetTexture(int meshNum)
{
	return m_meshInfo[meshNum].texture;
}

ID3D11ShaderResourceView* Fbx::GetTexture()
{
	return texture;
}

void Fbx::DrawInformation()
{
	std::string name = typeid(*this).name();

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(1000, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Once);
	ImGui::Begin(name.substr(6).c_str());
	
	char fname[256];
	strcpy_s(fname, m_fileName.c_str());
	ImGui::InputText("fileName", fname, sizeof(fname));
	m_fileName = fname;

	char tname[256];
	strcpy_s(tname, m_textureName.c_str());
	ImGui::InputText("textureName", tname, sizeof(tname));
	m_textureName = tname;
	
	if (ImGui::Button("Reload"))
	{
		Finalize();
		Initialize();
	}
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}


void Fbx::LoadProperties(const rapidjson::Value& inProp)
{
	JsonHelper::GetString(inProp, "fileName", m_fileName);
	JsonHelper::GetString(inProp, "textureName", m_textureName);
	JsonHelper::GetInt(inProp, "id", m_id);
	//Initialize();
}

void Fbx::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp)
{
	std::string name = typeid(*this).name();
	JsonHelper::AddString(alloc, inProp, "type", name.substr(6).c_str());
	JsonHelper::AddString(alloc, inProp, "fileName", m_fileName);
	JsonHelper::AddString(alloc, inProp, "textureName", m_textureName);
	JsonHelper::AddInt(alloc, inProp, "id", m_id);
}