#include "ImguiManager.h"
#include "FPS.h"
#include "manager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "Engine.h"
#include "Transform.h"
#include "Player.h"
#include "AABB.h"
#include "Collider.h"
#include "imgui/ImGuizmo.h"

bool ImguiManager::show_demo_window = false;
bool ImguiManager::show_another_window = false;

void ImguiManager::Initialize(void* hwnd, ID3D11Device* device, ID3D11DeviceContext* context)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device, context);
}

void ImguiManager::Update()
{
	//Vector3 rotation = CManager::GetScene()->Find<Camera>()->transform->position;
	////bool forward = CManager::GetScene()->Find<Player>()->GetComponent<Player>()->m_isGrounded;
	//Vector3 cpos = CManager::GetScene()->Find<Player>()->transform->position;
	//Camera* camera = CManager::GetScene()->Find<Camera>()->GetComponent<Camera>();
	//Vector3 target = camera->GetTarget();
	//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
	
}

void ImguiManager::Draw()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImguiManager::Finalize()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}