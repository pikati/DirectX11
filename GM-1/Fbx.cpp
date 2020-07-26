#include "Fbx.h"

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

}

void Fbx::Load(const char* fileName)
{
	LoadFBX(fileName);
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