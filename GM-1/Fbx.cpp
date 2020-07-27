#include "Fbx.h"
#include "GameObject.h"
#include "Transform.h"

#define FRAME 0.0166666666666667

void Fbx::Initialize()
{
	m_device = CRenderer::GetDevice();
	m_manager = FbxManager::Create();
	m_fbxInfo.uvSetCount = 0;
}

void Fbx::Update()
{

}

void Fbx::Draw()
{

}

void Fbx::Finalize()
{
	for (int i = m_fbxInfo.meshCount - 1; i >= 0; i--)
	{
		delete 	m_meshInfo[i].uvSetName;
	}
	for (int i = m_fbxInfo.meshCount - 1; i >= 0; i--)
	{
		delete m_meshInfo[i].vertex;
	}
	delete m_meshInfo;
	delete m_fbxInfo.uvSetName;
}

void Fbx::Load(const char* fileName)
{
	LoadFBX(fileName);
	InitializeFBX();
	InitializeAnimation();
	SetAnimationVertex();
}

void Fbx::LoadFBX(const char* fileName)
{
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
	MeshTrianglate();
	InitializeUnit();
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

		m_meshInfo[meshIndex].vertex = new VERTEX_3D[m_meshInfo[meshIndex].vertexCount];
		GetVertex(meshIndex);
		m_meshInfo[meshIndex].index = m_fbxInfo.meshes[meshIndex]->GetPolygonVertices();
		GetNormal(meshIndex);
		GetUVSetName(meshIndex);
		GetUV(meshIndex);

		//GetTextureInfo(meshIndex);
		//LoadTexture(meshIndex);
		CreateVertexBuffer(meshIndex);
		CreateIndexBuffer(meshIndex);
		//SetVertexBuffer(meshIndex);
		SetIndexBuffer(meshIndex);
	}
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
	m_fbxInfo.material.reserve(m_fbxInfo.materialCount);
	for (int i = 0; i < m_fbxInfo.materialCount; i++)
	{
		//�}�e���A�����擾
		m_fbxInfo.material.emplace_back(m_scene->GetMaterial(i));
	}
}

int Fbx::GetPolygonCount(int meshIndex)
{
	return m_fbxInfo.meshes[meshIndex]->GetPolygonCount();
}

int Fbx::GetVertexCount(int meshIndex)
{
	return m_fbxInfo.meshes[meshIndex]->GetControlPointsCount();
}

int Fbx::GetIndexCount(int meshIndex)
{
	return m_fbxInfo.meshes[meshIndex]->GetPolygonVertexCount();
}

void Fbx::GetVertex(int meshIndex)
{
	//���b�V���Ɋ܂܂�钸�_���W���擾
	FbxVector4* vtx = m_fbxInfo.meshes[meshIndex]->GetControlPoints();
	for (int vIdx = 0; vIdx < m_meshInfo[meshIndex].vertexCount; vIdx++)
	{
		m_meshInfo[meshIndex].vertex[vIdx].Position.x = (float)vtx[vIdx][0];
		m_meshInfo[meshIndex].vertex[vIdx].Position.y = (float)vtx[vIdx][1];
		m_meshInfo[meshIndex].vertex[vIdx].Position.z = (float)vtx[vIdx][2];
	}
}

void Fbx::GetNormal(int meshIndex)
{
	FbxArray<FbxVector4> normals;
	//�@�����擾
	m_fbxInfo.meshes[meshIndex]->GetPolygonVertexNormals(normals);
	//�@���̐����擾
	int normalCount = normals.Size();
	for (int i = 0; i < normalCount; i++)
	{
		//���_�C���f�b�N�X�ɑΉ��������_�ɒl����
		m_meshInfo[meshIndex].vertex[m_meshInfo[meshIndex].index[i]].Normal.x = normals[i][0];
		m_meshInfo[meshIndex].vertex[m_meshInfo[meshIndex].index[i]].Normal.y = normals[i][1];
		m_meshInfo[meshIndex].vertex[m_meshInfo[meshIndex].index[i]].Normal.z = normals[i][2];
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
	for (int i = 0; i < uvsetCount; i++)
	{
		//���_�C���f�b�N�X�ɑΉ��������_�ɒl����
		m_meshInfo[meshIndex].vertex[m_meshInfo[meshIndex].index[i]].TexCoord.x = uvsets[i][0];
		m_meshInfo[meshIndex].vertex[m_meshInfo[meshIndex].index[i]].TexCoord.y = 1.0f - uvsets[i][1];
	}
}

void Fbx::TextureMemoryAllocate(int meshIndex)
{
	m_meshInfo[meshIndex].uvSetName = new std::string[m_meshInfo[meshIndex].uvSetCount];
	m_meshInfo[meshIndex].texturePath.reserve(m_meshInfo[meshIndex].uvSetCount);
	//m_meshInfo[meshIndex].texture = new ID3D11ShaderResourceView[m_meshInfo[meshIndex].uvSetCount];
}

void Fbx::CreateVertexBuffer(int meshIndex)
{
	//�����o���ł�����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * m_meshInfo[meshIndex].vertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = m_meshInfo[meshIndex].vertex;

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

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = m_meshInfo[meshIndex].index;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_meshInfo[meshIndex].pIB);
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
		MessageBox(NULL, "�A�j���[�V�����Ȃ���ł�����", NULL, MB_OK);
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
}

void Fbx::SetAnimationVertex()
{
	double animationTime = m_start.GetSecondDouble();
	double stopTime = m_stop.GetSecondDouble();
	int count = 0;
	m_timeCount = m_start;
	m_animVertex.resize(m_fbxInfo.meshCount);

	while (animationTime <= stopTime)
	{
		count++;
		animationTime += FRAME;
	}

	m_count = count;

	for (int i = 0; i < m_fbxInfo.meshCount; i++)
	{
		m_animVertex[i].resize(count);
		for (int j = 0; j < count; j++)
		{
			m_animVertex[i][j].resize(m_meshInfo[i].vertexCount);
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

			FbxMatrix* clusterDeformation = new FbxMatrix[m_fbxInfo.meshes[i]->GetControlPointsCount()];//1.9053
			memset(clusterDeformation, 0, sizeof(FbxMatrix) * m_fbxInfo.meshes[i]->GetControlPointsCount());//0.0167
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
					FbxVector4 outVertex = clusterDeformation[cnt].MultNormalize(m_fbxInfo.meshes[i]->GetControlPointAt(cnt));
					float x = (FLOAT)outVertex[0];
					float y = (FLOAT)outVertex[1];
					float z = (FLOAT)outVertex[2];

					m_animVertex[i][animCount][cnt] = (D3DXVECTOR3(x, y, z));
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

	UpdateTime();
	for (int i = 0; i < m_fbxInfo.meshCount; i++)
	{

		SetVertexBuffer(i);
		D3DXMATRIX world, rotation;
		D3DXMatrixRotationYawPitchRoll(&rotation, gameObject->transform->rotation.y, gameObject->transform->rotation.x, gameObject->transform->rotation.z);
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
	}
}

void Fbx::UpdateTime()
{
	if (true)
	{
		//�A�j���[�V�����̎��ԍX�V
		m_timeCount += m_frameTime;
		if (m_timeCount > m_stop)
		{
			//���s�����������ɂȂ���^�C�~���O
			m_timeCount = m_stop * 3 / 28;
		}
		m_frame++;
		if (m_frame >= m_count - 9)
		{
			m_frame = 0;
		}
	}
	else
	{
		m_timeCount = m_start;
		m_frame = 0;
	}
}