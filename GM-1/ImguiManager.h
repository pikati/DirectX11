#pragma once
#include "imGui/imgui.h"
#include "imGui/imgui_impl_win32.h"
#include "imGui/imgui_impl_dx11.h"
class ImguiManager
{
private:
	static bool show_demo_window;
	static bool show_another_window;
public:
	static void Initialize(void*, ID3D11Device*, ID3D11DeviceContext*);
	static void Update();
	static void Draw();
	static void Finalize();
};

