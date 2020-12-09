#include "Editor.h"
#include "Hierarchy.h"
#include "Inspector.h"
#include "EditorController.h"
#include "main.h"
#include "input.h"
bool Editor::m_isDrawEditor = true;

void Editor::Initialize()
{
	Hierarchy::Initialize();
	Inspector::Initialize();
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
	Hierarchy::Update();
	Inspector::Update();
	EditorController::Update();
	
}

void Editor::Draw()
{
	if (!m_isDrawEditor) return;
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