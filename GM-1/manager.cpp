#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "Scene.h"
#include "ClassDictionary.h"
#include "ImguiManager.h"
#include "AudioManager.h"
#include "ObjectPooler.h"
#include "Editor.h"

Scene* g_scene;

void CManager::Init()
{
	ClassDictionary::Initialize();
	CRenderer::Init();
	ImguiManager::Initialize(GetWindow(), CRenderer::GetDevice(), CRenderer::GetDeviceContext());
	CInput::Init();
	AudioManager::Initialize();
	ObjectPooler::Initialize();

	g_scene = new Scene();
	g_scene->Initialize();
	Editor::Initialize();

}

void CManager::Uninit()
{
	Editor::Finalize();
	g_scene->Finalize();
	delete g_scene;
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
	Editor::Update();
}

void CManager::Draw()
{
	CRenderer::Begin();

	/*LIGHT light;
	light.Enable = false;
	light.Direction = Vector4(1.0f, -1.0f, 1.0f, 0.0f);
	light.Direction.Normalize();
	light.Ambient = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	light.Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetLight(light);*/

	g_scene->Draw();
	Editor::Draw();
	ImguiManager::Draw();
	CRenderer::End();
}

Scene* CManager::GetScene()
{
	return g_scene;
}