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

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Debug Window");
		for (int i = 0; i < Collider::m_colliders.size(); i++)
		{
			if (Collider::m_colliders[i]->GetGameObject()->tag == "Player")
			{
				std::vector<GameObject*> obj = Collider::m_colliders[i]->GetHitGameObject();
				for (int j = 0; j < obj.size(); j++)
				{
					if (obj[i] != nullptr)
					{
						ImGui::Text("%s", obj[i]->name.c_str());
					}
				}
			}

		}
		/*ImGui::Text("Camera rotation x:%f, y:%f, z:%f", rotation.x, rotation.y, rotation.z);
		ImGui::Text("Camera target   x:%f, y:%f, z:%f", target.x, target.y, target.z);
		
		ImGui::Text("player   pos  x:%f, y:%f, z:%f", cpos.x, cpos.y, cpos.z);*/
		//ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		//ImGui::Checkbox("Another Window", &show_another_window);

		//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		//if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	counter++;
		//ImGui::SameLine();
		//ImGui::Text("counter = %d", counter);
		////ImGui::SameLine();
		//ImGui::Text("fps = %f", FPS::deltaTime);
		//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
	
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