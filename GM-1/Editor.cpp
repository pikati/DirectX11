#include "Editor.h"
#include "Hierarchy.h"
#include "Inspector.h"
#include "EditorController.h"
#include "main.h"
#include "input.h"

bool Editor::m_isDrawEditor = true;
std::unique_ptr<EditorCamera> Editor::m_camera;

using namespace std;

void Editor::Initialize()
{
	Hierarchy::Initialize();
	Inspector::Initialize();
	m_camera = make_unique<EditorCamera>();
	m_camera->Initialize();
}

void Editor::Update()
{
	if (CInput::GetKeyPress(VK_CONTROL))
	{
		if (CInput::GetKeyTrigger('P'))
		{
			m_isDrawEditor = !m_isDrawEditor;
		}
	}
	if (!m_isDrawEditor) return;
	if (!EditorController::IsPlay())
	{
		m_camera->Update();
	}
	Hierarchy::Update();
	Inspector::Update();
	EditorController::Update();
	
}

void Editor::Draw()
{
	if (!m_isDrawEditor) return;
	if (!EditorController::IsPlay())
	{
		m_camera->Draw();
	}
	Hierarchy::Draw();
	Inspector::Draw();
}

void Editor::Finalize()
{
	Inspector::Finalize();
	Hierarchy::Finalize();
}


bool Editor::IsPlay()
{
	return EditorController::IsPlay();
}

D3DXMATRIX Editor::GetEditorCameraViewMatrix()
{
	return m_camera->GetViewMatrix();
}

D3DXMATRIX Editor::GetEditorCameraProjectionMatrix()
{
	return m_camera->GetProjectionMatrix();
}