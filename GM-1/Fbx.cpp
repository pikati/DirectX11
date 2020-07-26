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
		//インポーター作成
		FbxImporter* importer = FbxImporter::Create(m_manager, "");

		if (NULL != importer)
		{
			//シーン作成
			m_scene = FbxScene::Create(m_manager, "");

			if (NULL != m_scene)
			{
				//filePathに指定したファイルを読み込む
				bool result = importer->Initialize(fileName);

				if (result)
				{
					//シーンにインポートしたファイルを渡す
					importer->Import(m_scene);
				}
				else
				{
					MessageBox(NULL, "ファイルパス間違ってるかもよ", NULL, MB_OK);
				}
			}
		}
		//インポーターの役目は終わりなので解放する
		importer->Destroy();
	}
}