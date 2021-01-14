#pragma once
#include <memory>
#include "EditorCamera.h"

class Editor
{
private:
	static bool m_isDrawEditor;
	static std::unique_ptr<EditorCamera> m_camera;
public:
	static void Initialize();
	static void Update();
	static void Draw();
	static void Finalize();

	static bool IsPlay();
	static const float& GetEditorCameraDistance();
	static D3DXMATRIX GetEditorCameraViewMatrix();
	static D3DXMATRIX GetEditorCameraProjectionMatrix();
};

