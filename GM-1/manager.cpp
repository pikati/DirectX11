#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "Scene.h"
#include "ClassDictionary.h"
#include "ImguiManager.h"
#include "AudioManager.h"
#include "ObjectPooler.h"
#include "Editer.h"

Scene* g_scene;

void CManager::Init()
{
	ClassDictionary::Initialize();
	CRenderer::Init();
	ImguiManager::Initialize(GetWindow(), CRenderer::GetDevice(), CRenderer::GetDeviceContext());
	CInput::Init();
	AudioManager::Initialize();
	ObjectPooler::Initialize();
	Editer::Initialize();

	g_scene = new Scene();
	g_scene->Initialize();
}

void CManager::Uninit()
{
	g_scene->Finalize();
	delete g_scene;
	Editer::Finalize();
	ObjectPooler::Finalize();
	AudioManager::Finalize();
	CInput::Uninit();
	ImguiManager::Finalize();
	CRenderer::Uninit();

}

void CManager::Update()
{
	CInput::Update();
	ImguiManager::Update();
	g_scene->Update();
	Editer::Update();
}

void CManager::Draw()
{
	CRenderer::Begin();

	LIGHT light;
	light.Enable = true;
	light.Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetLight(light);

	g_scene->Draw();
	Editer::Draw();
	ImguiManager::Draw();
	CRenderer::End();
}

Scene* CManager::GetScene()
{
	return g_scene;
}