#include "Hierarchy.h"
#include "imGui/imgui.h"
#include "imGui/imgui_impl_win32.h"
#include "imGui/imgui_impl_dx11.h"
#include "manager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Inspector.h"
#include "Plane.h"
#include <list>

bool Hierarchy::m_isLoadMenu = false;
bool Hierarchy::m_isSaveMenu = false;
int* Hierarchy::m_layerMax = nullptr;
std::string Hierarchy::m_loadPath = "";
std::string Hierarchy::m_savePath = "";

void Hierarchy::Initialize()
{
    m_layerMax = new int[LAYER_MAX];
    std::list<GameObject*>* objs = CManager::GetScene()->GetAllGameObject();
    for (int i = 0; i < LAYER_MAX; i++)
    {
        m_layerMax[i] = 0;
        for (GameObject* obj : objs[i])
        {
            m_layerMax[i]++;
        }
    }
}

void Hierarchy::Update()
{
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
    ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_Once);

    ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_MenuBar);

	std::list<GameObject*>* objects = CManager::GetScene()->GetAllGameObject();

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Scene"))
            {
                CManager::GetScene()->CreateScene();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Save"))
            {
                m_isSaveMenu = !m_isSaveMenu;
            }
            if (ImGui::MenuItem("Load"))
            {
                m_isLoadMenu = !m_isLoadMenu;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("GameObject"))
        {
            if(ImGui::MenuItem("Empty"))
            {
                GameObject* obj = CManager::GetScene()->CreateGameObject();
                Inspector::SetGameObject(obj, 1, m_layerMax[1]);
            }
            if (ImGui::MenuItem("Plane"))
            {
                GameObject* obj = CManager::GetScene()->CreateGameObject();
                obj->AddComponent<Plane>();
                obj->SystemInitialize();
                obj->Initialize();
                Inspector::SetGameObject(obj, 1, m_layerMax[1]);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    for (int i = 0; i < LAYER_MAX; i++)
    {
        int count = 0;
        for (GameObject* object : objects[i])
        {
            if (ImGui::Button(object->name.c_str()))
            {
                Inspector::SetGameObject(object, i, count);
            }
            count++;
        }
    }

    ImGui::End();

    if (m_isLoadMenu)
    {
        DispLoadMenu();
    }
    if (m_isSaveMenu)
    {
        DispSaveMenu();
    }

    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
}

void Hierarchy::Draw()
{

}

void Hierarchy::Finalize()
{

}

void Hierarchy::DispLoadMenu()
{
    bool batsu = true;
    char path[256] = "";
    strcpy_s(path, m_loadPath.c_str());
    ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_Once);

    ImGui::Begin("Load", &batsu, ImGuiWindowFlags_MenuBar);
    ImGui::InputText("loadFile", path, sizeof(path));
    m_loadPath = path;
    if (ImGui::Button("Load"))
    {
        CManager::GetScene()->LoadScene(path);
    }
    ImGui::End();
    
}

void Hierarchy::DispSaveMenu()
{
    bool batsu = true;
    char path[256] = "";
    strcpy_s(path, m_savePath.c_str());

    ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_Once);

    ImGui::Begin("Save", &batsu, ImGuiWindowFlags_MenuBar);
    ImGui::InputText("saveFile", path, sizeof(path));
    m_savePath = path;
    if (ImGui::Button("Save"))
    {
        CManager::GetScene()->SaveScene(path);
    }
    ImGui::End();
}

void Hierarchy::SetDefaultPath(std::string path)
{
    m_loadPath = path;
    m_savePath = path;
}