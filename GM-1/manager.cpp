#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "Scene.h"
#include "ClassDictionary.h"
#include "ImguiManager.h"
#include "AudioManager.h"

Scene* g_scene;

void CManager::Init()
{
	ClassDictionary::Initialize();
	CRenderer::Init();
	CInput::Init();
	AudioManager::Initialize();

	g_scene = new Scene();
	g_scene->Initialize();
}

void CManager::Uninit()
{
	ImguiManager::Finalize();
	g_scene->Finalize();
	delete g_scene;
	AudioManager::Finalize();
	CInput::Uninit();
	CRenderer::Uninit();
}

void CManager::Update()
{
	CInput::Update();
	ImguiManager::Update();
	g_scene->Update();
}

void CManager::Draw()
{
	CRenderer::Begin();

	LIGHT light;
	light.Enable = false;
	light.Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetLight(light);

	g_scene->Draw();

	ImguiManager::Draw();
	CRenderer::End();
}

Scene* CManager::GetScene()
{
	return g_scene;
}