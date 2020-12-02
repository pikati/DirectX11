#include "Animation.h"
#include "LevelLoader.h"
#include "imGui/imgui.h"

Animation::Animation()
{
	m_dataNum = 0;
	m_frame = 0;
	m_state = 0;
}

void Animation::Initialize()
{
	m_defaultState = 0;
	for (int i = 0; i < m_dataNum; i++)
	{
		if (!m_animationData[i]->isLoop)
		{
			m_animationData[i]->loopStartTime = m_animationData[i]->startTime;
			m_animationData[i]->loopEndTime = m_animationData[i]->endTime;
		}
	}
}

void Animation::Update()
{
	UpdateFrame();
}

void Animation::Finalize()
{
	for (AnimationData* d : m_animationData)
	{
		delete d;
		d = nullptr;
	}
	m_animationData.clear();
}

void Animation::UpdateFrame()
{
	m_frame += m_animationData[m_state]->speed;
	if (m_animationData[m_state]->isLoop)
	{
		if (m_frame > m_animationData[m_state]->loopEndTime)
		{
			m_frame = m_animationData[m_state]->loopStartTime;
		}
	}
	else
	{
		if (m_frame > m_animationData[m_state]->endTime)
		{
			m_frame = m_animationData[m_state]->endTime;
		}
	}
}

void Animation::SetAnimationNum(int n)
{
	int oldDataNum = m_dataNum;
	if (oldDataNum > n)
	{
		for (int i = n; i < oldDataNum; i++)
		{
			delete m_animationData[i];
			m_animationData[i] = nullptr;
		}
		m_animationData.erase(m_animationData.begin() + n, m_animationData.end());
	}
	m_dataNum = n;
	m_animationData.resize(m_dataNum);
	if (oldDataNum < n)
	{
		for (int i = oldDataNum; i < n; i++)
		{
			m_animationData[i] = new AnimationData();
		}
	}
}

void Animation::SetAnimationData(std::string name, int state, bool isLoop, int startTime, int endTime, int speed, int loopStartTime, int loopEndTime)
{
	m_animationData[state]->animationName = name;
	m_animationData[state]->isLoop = isLoop;
	m_animationData[state]->startTime = startTime;
	m_animationData[state]->endTime = endTime;
	m_animationData[state]->loopStartTime = loopStartTime;
	m_animationData[state]->loopEndTime = loopEndTime;
	m_animationData[state]->speed = speed;
}

void Animation::SetAnimationData(const char* name, int state, bool isLoop, int startTime, int endTime, int speed, int loopStartTime, int loopEndTime)
{
	m_animationData[state]->animationName = name;
	m_animationData[state]->isLoop = isLoop;
	m_animationData[state]->startTime = startTime;
	m_animationData[state]->endTime = endTime;
	m_animationData[state]->loopStartTime = loopStartTime;
	m_animationData[state]->loopEndTime = loopEndTime;
	m_animationData[state]->speed = speed;
}

int Animation::GetTime()
{
	return m_frame;
}

int Animation::GetState()
{
	return m_state;
}

void Animation::SetState(int state)
{
	if (m_state == state) return;
	if (!m_animationData[m_state]->isLoop)
	{
		if (m_animationData[m_state]->endTime != m_frame)
		{
			return;
		}
	}

	m_state = state;
	m_frame = m_animationData[m_state]->startTime;
}

void Animation::SetDefaultState(int state)
{
	m_defaultState = state;
}

void Animation::SetSpeed(int state, int speed)
{
	m_animationData[state]->speed = speed;
}

void Animation::DrawInformation()
{
	if (m_animationData[0] == nullptr) return;
	std::string name = typeid(*this).name();

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(1000, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Once);
	ImGui::Begin(name.substr(6).c_str());

	ImGui::Text("AnimationNum"); ImGui::SameLine();
	ImGui::InputInt(" ", &m_dataNum, 1, 10);
	if (m_dataNum > 7) m_dataNum = 7;

	ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);
	for (int i = 0; i < m_dataNum; i++)
	{
		if (ImGui::TreeNode(m_animationData[i]->animationName.c_str()))
		{
			ImGui::InputInt("start", &m_animationData[i]->startTime);
			ImGui::InputInt("end", &m_animationData[i]->endTime);
			ImGui::InputInt("speed", &m_animationData[i]->speed);
			ImGui::Checkbox("IsLoop", &m_animationData[i]->isLoop);
			if (m_animationData[i]->isLoop)
			{
				ImGui::InputInt("loopStart", &m_animationData[i]->loopStartTime);
				ImGui::InputInt("loopEnd", &m_animationData[i]->loopEndTime);
			}

			ImGui::TreePop();
		}
	}
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void Animation::LoadProperties(const rapidjson::Value& inProp)
{
	JsonHelper::GetInt(inProp, "dataNum", m_dataNum);
	m_animationData.resize(m_dataNum);

	for (int i = 0; i < m_dataNum; i++)
	{
		m_animationData[i] = new AnimationData();
	}
	JsonHelper::GetString(inProp, "name1", m_animationData[0]->animationName);
	JsonHelper::GetBool(inProp, "loop1", m_animationData[0]->isLoop);
	JsonHelper::GetInt(inProp, "start1", m_animationData[0]->startTime);
	JsonHelper::GetInt(inProp, "end1", m_animationData[0]->endTime);
	JsonHelper::GetInt(inProp, "lstart1", m_animationData[0]->loopStartTime);
	JsonHelper::GetInt(inProp, "lend1", m_animationData[0]->loopEndTime);
	JsonHelper::GetInt(inProp, "speed1", m_animationData[0]->speed);
	if (m_dataNum == 1) return;
	JsonHelper::GetString(inProp, "name2", m_animationData[1]->animationName);
	JsonHelper::GetBool(inProp, "loop2", m_animationData[1]->isLoop);
	JsonHelper::GetInt(inProp, "start2", m_animationData[1]->startTime);
	JsonHelper::GetInt(inProp, "end2", m_animationData[1]->endTime);
	JsonHelper::GetInt(inProp, "lstart2", m_animationData[1]->loopStartTime);
	JsonHelper::GetInt(inProp, "lend2", m_animationData[1]->loopEndTime);
	JsonHelper::GetInt(inProp, "speed2", m_animationData[1]->speed);
	if (m_dataNum == 2) return;
	JsonHelper::GetString(inProp, "name3", m_animationData[2]->animationName);
	JsonHelper::GetBool(inProp, "loop3", m_animationData[2]->isLoop);
	JsonHelper::GetInt(inProp, "start3", m_animationData[2]->startTime);
	JsonHelper::GetInt(inProp, "end3", m_animationData[2]->endTime);
	JsonHelper::GetInt(inProp, "lstart3", m_animationData[2]->loopStartTime);
	JsonHelper::GetInt(inProp, "lend3", m_animationData[2]->loopEndTime);
	JsonHelper::GetInt(inProp, "speed3", m_animationData[2]->speed);
	if (m_dataNum == 3) return;
	JsonHelper::GetString(inProp, "name4", m_animationData[3]->animationName);
	JsonHelper::GetBool(inProp, "loop4", m_animationData[3]->isLoop);
	JsonHelper::GetInt(inProp, "start4", m_animationData[3]->startTime);
	JsonHelper::GetInt(inProp, "end4", m_animationData[3]->endTime);
	JsonHelper::GetInt(inProp, "lstart4", m_animationData[3]->loopStartTime);
	JsonHelper::GetInt(inProp, "lend4", m_animationData[3]->loopEndTime);
	JsonHelper::GetInt(inProp, "speed4", m_animationData[3]->speed);
	if (m_dataNum == 4) return;
	JsonHelper::GetString(inProp, "name5", m_animationData[4]->animationName);
	JsonHelper::GetBool(inProp, "loop5", m_animationData[4]->isLoop);
	JsonHelper::GetInt(inProp, "start5", m_animationData[4]->startTime);
	JsonHelper::GetInt(inProp, "end5", m_animationData[4]->endTime);
	JsonHelper::GetInt(inProp, "lstart5", m_animationData[4]->loopStartTime);
	JsonHelper::GetInt(inProp, "lend5", m_animationData[4]->loopEndTime);
	JsonHelper::GetInt(inProp, "speed5", m_animationData[4]->speed);
	if (m_dataNum == 5) return;
	JsonHelper::GetString(inProp, "name6", m_animationData[5]->animationName);
	JsonHelper::GetBool(inProp, "loop6", m_animationData[5]->isLoop);
	JsonHelper::GetInt(inProp, "start6", m_animationData[5]->startTime);
	JsonHelper::GetInt(inProp, "end6", m_animationData[5]->endTime);
	JsonHelper::GetInt(inProp, "lstart6", m_animationData[5]->loopStartTime);
	JsonHelper::GetInt(inProp, "lend6", m_animationData[5]->loopEndTime);
	JsonHelper::GetInt(inProp, "speed6", m_animationData[5]->speed);
	if (m_dataNum == 6) return;
	JsonHelper::GetString(inProp, "name7", m_animationData[6]->animationName);
	JsonHelper::GetBool(inProp, "loop7", m_animationData[6]->isLoop);
	JsonHelper::GetInt(inProp, "start7", m_animationData[6]->startTime);
	JsonHelper::GetInt(inProp, "end7", m_animationData[6]->endTime);
	JsonHelper::GetInt(inProp, "lstart7", m_animationData[6]->loopStartTime);
	JsonHelper::GetInt(inProp, "lend7", m_animationData[6]->loopEndTime);
	JsonHelper::GetInt(inProp, "speed7", m_animationData[6]->speed);
}

void Animation::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp)
{
	std::string name = typeid(*this).name();
	JsonHelper::AddString(alloc, inProp, "type", name.substr(6).c_str());
	JsonHelper::AddInt(alloc, inProp, "dataNum", m_dataNum);

	JsonHelper::AddString(alloc, inProp, "name1", m_animationData[0]->animationName);
	JsonHelper::AddBool(alloc, inProp, "loop1", m_animationData[0]->isLoop);
	JsonHelper::AddInt(alloc, inProp, "start1", m_animationData[0]->startTime);
	JsonHelper::AddInt(alloc, inProp, "end1", m_animationData[0]->endTime);
	JsonHelper::AddInt(alloc, inProp, "lstart1", m_animationData[0]->loopStartTime);
	JsonHelper::AddInt(alloc, inProp, "lend1", m_animationData[0]->loopEndTime);
	JsonHelper::AddInt(alloc, inProp, "speed1", m_animationData[0]->speed);
	if (m_dataNum == 1) return;
	JsonHelper::AddString(alloc, inProp, "name2", m_animationData[1]->animationName);
	JsonHelper::AddBool(alloc, inProp, "loop2", m_animationData[1]->isLoop);
	JsonHelper::AddInt(alloc, inProp, "start2", m_animationData[1]->startTime);
	JsonHelper::AddInt(alloc, inProp, "end2", m_animationData[1]->endTime);
	JsonHelper::AddInt(alloc, inProp, "lstart2", m_animationData[1]->loopStartTime);
	JsonHelper::AddInt(alloc, inProp, "lend2", m_animationData[1]->loopEndTime);
	JsonHelper::AddInt(alloc, inProp, "speed2", m_animationData[1]->speed);
	if (m_dataNum == 2) return;
	JsonHelper::AddString(alloc, inProp, "name3", m_animationData[2]->animationName);
	JsonHelper::AddBool(alloc, inProp, "loop3", m_animationData[2]->isLoop);
	JsonHelper::AddInt(alloc, inProp, "start3", m_animationData[2]->startTime);
	JsonHelper::AddInt(alloc, inProp, "end3", m_animationData[2]->endTime);
	JsonHelper::AddInt(alloc, inProp, "lstart3", m_animationData[2]->loopStartTime);
	JsonHelper::AddInt(alloc, inProp, "lend3", m_animationData[2]->loopEndTime);
	JsonHelper::AddInt(alloc, inProp, "speed3", m_animationData[2]->speed);
	if (m_dataNum == 3) return;
	JsonHelper::AddString(alloc, inProp, "name4", m_animationData[3]->animationName);
	JsonHelper::AddBool(alloc, inProp, "loop4", m_animationData[3]->isLoop);
	JsonHelper::AddInt(alloc, inProp, "start4", m_animationData[3]->startTime);
	JsonHelper::AddInt(alloc, inProp, "end4", m_animationData[3]->endTime);
	JsonHelper::AddInt(alloc, inProp, "lstart4", m_animationData[3]->loopStartTime);
	JsonHelper::AddInt(alloc, inProp, "lend4", m_animationData[3]->loopEndTime);
	JsonHelper::AddInt(alloc, inProp, "speed4", m_animationData[3]->speed);
	if (m_dataNum == 4) return;
	JsonHelper::AddString(alloc, inProp, "name5", m_animationData[4]->animationName);
	JsonHelper::AddBool(alloc, inProp, "loop5", m_animationData[4]->isLoop);
	JsonHelper::AddInt(alloc, inProp, "start5", m_animationData[4]->startTime);
	JsonHelper::AddInt(alloc, inProp, "end5", m_animationData[4]->endTime);
	JsonHelper::AddInt(alloc, inProp, "lstart5", m_animationData[4]->loopStartTime);
	JsonHelper::AddInt(alloc, inProp, "lend5", m_animationData[4]->loopEndTime);
	JsonHelper::AddInt(alloc, inProp, "speed5", m_animationData[4]->speed);
	if (m_dataNum == 5) return;
	JsonHelper::AddString(alloc, inProp, "name6", m_animationData[5]->animationName);
	JsonHelper::AddBool(alloc, inProp, "loop6", m_animationData[5]->isLoop);
	JsonHelper::AddInt(alloc, inProp, "start6", m_animationData[5]->startTime);
	JsonHelper::AddInt(alloc, inProp, "end6", m_animationData[5]->endTime);
	JsonHelper::AddInt(alloc, inProp, "lstart6", m_animationData[5]->loopStartTime);
	JsonHelper::AddInt(alloc, inProp, "lend6", m_animationData[5]->loopEndTime);
	JsonHelper::AddInt(alloc, inProp, "speed6", m_animationData[5]->speed);
	if (m_dataNum == 6) return;
	JsonHelper::AddString(alloc, inProp, "name7", m_animationData[6]->animationName);
	JsonHelper::AddBool(alloc, inProp, "loop7", m_animationData[6]->isLoop);
	JsonHelper::AddInt(alloc, inProp, "start7", m_animationData[6]->startTime);
	JsonHelper::AddInt(alloc, inProp, "end7", m_animationData[6]->endTime);
	JsonHelper::AddInt(alloc, inProp, "lstart7", m_animationData[6]->loopStartTime);
	JsonHelper::AddInt(alloc, inProp, "lend7", m_animationData[6]->loopEndTime);
	JsonHelper::AddInt(alloc, inProp, "speed7", m_animationData[6]->speed);
}