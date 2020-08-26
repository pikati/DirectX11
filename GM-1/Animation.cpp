#include "Animation.h"
#include "LevelLoader.h"

Animation::Animation()
{
	m_animationData = nullptr;
	m_dataNum = 0;
	m_frame = 0;
	m_state = 0;
}

void Animation::Initialize()
{
	m_defaultState = 0;
}

void Animation::Update()
{
	UpdateFrame();
}

void Animation::Draw()
{

}

void Animation::Finalize()
{
	if (m_animationData != nullptr)
	{
		delete m_animationData;
	}
}

void Animation::UpdateFrame()
{
	m_frame += m_animationData[m_state].speed;
	if (m_animationData[m_state].isLoop)
	{
		if (m_frame > m_animationData[m_state].loopEndTime)
		{
			m_frame = m_animationData[m_state].loopStartTime;
		}
	}
	else
	{
		if (m_frame > m_animationData[m_state].endTime)
		{
			m_frame = m_animationData[m_state].endTime;
		}
	}
}

void Animation::SetAnimationNum(int n)
{
	m_dataNum = n;
	m_animationData = new AnimationData[m_dataNum];
}

void Animation::SetAnimationData(int state, bool isLoop, int startTime, int endTime, int speed, int loopStartTime, int loopEndTime)
{
	m_animationData[state].isLoop = isLoop;
	m_animationData[state].startTime = startTime;
	m_animationData[state].endTime = endTime;
	m_animationData[state].loopStartTime = loopStartTime;
	m_animationData[state].loopEndTime = loopEndTime;
	m_animationData[state].speed = speed;
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
	if (!m_animationData[m_state].isLoop)
	{
		if (m_animationData[m_state].endTime != m_frame)
		{
			return;
		}
	}

	m_state = state;
	m_frame = m_animationData[m_state].startTime;
}

void Animation::SetDefaultState(int state)
{
	m_defaultState = state;
}

void Animation::SetSpeed(int state, int speed)
{
	m_animationData[state].speed = speed;
}

void Animation::LoadProperties(const rapidjson::Value& inProp)
{
	JsonHelper::GetInt(inProp, "dataNum", m_dataNum);
	m_animationData = new AnimationData[m_dataNum];

	JsonHelper::GetBool(inProp, "loop1", m_animationData[0].isLoop);
	JsonHelper::GetInt(inProp, "start1", m_animationData[0].startTime);
	JsonHelper::GetInt(inProp, "end1", m_animationData[0].endTime);
	JsonHelper::GetInt(inProp, "lstart1", m_animationData[0].loopStartTime);
	JsonHelper::GetInt(inProp, "lend1", m_animationData[0].loopEndTime);
	JsonHelper::GetInt(inProp, "speed1", m_animationData[0].speed);
	if (m_dataNum == 1) return;
	JsonHelper::GetBool(inProp, "loop2", m_animationData[1].isLoop);
	JsonHelper::GetInt(inProp, "start2", m_animationData[1].startTime);
	JsonHelper::GetInt(inProp, "end2", m_animationData[1].endTime);
	JsonHelper::GetInt(inProp, "lstart2", m_animationData[1].loopStartTime);
	JsonHelper::GetInt(inProp, "lend2", m_animationData[1].loopEndTime);
	JsonHelper::GetInt(inProp, "speed2", m_animationData[1].speed);
	if (m_dataNum == 2) return;
	JsonHelper::GetBool(inProp, "loop3", m_animationData[2].isLoop);
	JsonHelper::GetInt(inProp, "start3", m_animationData[2].startTime);
	JsonHelper::GetInt(inProp, "end3", m_animationData[2].endTime);
	JsonHelper::GetInt(inProp, "lstart3", m_animationData[2].loopStartTime);
	JsonHelper::GetInt(inProp, "lend3", m_animationData[2].loopEndTime);
	JsonHelper::GetInt(inProp, "speed3", m_animationData[2].speed);
	if (m_dataNum == 3) return;
	JsonHelper::GetBool(inProp, "loop4", m_animationData[3].isLoop);
	JsonHelper::GetInt(inProp, "start4", m_animationData[3].startTime);
	JsonHelper::GetInt(inProp, "end4", m_animationData[3].endTime);
	JsonHelper::GetInt(inProp, "lstart4", m_animationData[3].loopStartTime);
	JsonHelper::GetInt(inProp, "lend4", m_animationData[3].loopEndTime);
	JsonHelper::GetInt(inProp, "speed4", m_animationData[3].speed);
	if (m_dataNum == 4) return;
	JsonHelper::GetBool(inProp, "loop5", m_animationData[4].isLoop);
	JsonHelper::GetInt(inProp, "start5", m_animationData[4].startTime);
	JsonHelper::GetInt(inProp, "end5", m_animationData[4].endTime);
	JsonHelper::GetInt(inProp, "lstart5", m_animationData[4].loopStartTime);
	JsonHelper::GetInt(inProp, "lend5", m_animationData[4].loopEndTime);
	JsonHelper::GetInt(inProp, "speed5", m_animationData[4].speed);
	if (m_dataNum == 5) return;
	JsonHelper::GetBool(inProp, "loop6", m_animationData[5].isLoop);
	JsonHelper::GetInt(inProp, "start6", m_animationData[5].startTime);
	JsonHelper::GetInt(inProp, "end6", m_animationData[5].endTime);
	JsonHelper::GetInt(inProp, "lstart6", m_animationData[5].loopStartTime);
	JsonHelper::GetInt(inProp, "lend6", m_animationData[5].loopEndTime);
	JsonHelper::GetInt(inProp, "speed6", m_animationData[5].speed);
	if (m_dataNum == 6) return;
	JsonHelper::GetBool(inProp, "loop7", m_animationData[6].isLoop);
	JsonHelper::GetInt(inProp, "start7", m_animationData[6].startTime);
	JsonHelper::GetInt(inProp, "end7", m_animationData[6].endTime);
	JsonHelper::GetInt(inProp, "lstart7", m_animationData[6].loopStartTime);
	JsonHelper::GetInt(inProp, "lend7", m_animationData[6].loopEndTime);
	JsonHelper::GetInt(inProp, "speed7", m_animationData[6].speed);
}

void Animation::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp)
{
	std::string name = typeid(*this).name();
	JsonHelper::AddString(alloc, inProp, "type", name.substr(6).c_str());
	JsonHelper::AddInt(alloc, inProp, "dataNum", m_dataNum);

	JsonHelper::AddBool(alloc, inProp, "loop1", m_animationData[0].isLoop);
	JsonHelper::AddInt(alloc, inProp, "start1", m_animationData[0].startTime);
	JsonHelper::AddInt(alloc, inProp, "end1", m_animationData[0].endTime);
	JsonHelper::AddInt(alloc, inProp, "lstart1", m_animationData[0].loopStartTime);
	JsonHelper::AddInt(alloc, inProp, "lend1", m_animationData[0].loopEndTime);
	JsonHelper::AddInt(alloc, inProp, "speed1", m_animationData[0].speed);
	if (m_dataNum == 1) return;
	JsonHelper::AddBool(alloc, inProp, "loop2", m_animationData[1].isLoop);
	JsonHelper::AddInt(alloc, inProp, "start2", m_animationData[1].startTime);
	JsonHelper::AddInt(alloc, inProp, "end2", m_animationData[1].endTime);
	JsonHelper::AddInt(alloc, inProp, "lstart2", m_animationData[1].loopStartTime);
	JsonHelper::AddInt(alloc, inProp, "lend2", m_animationData[1].loopEndTime);
	JsonHelper::AddInt(alloc, inProp, "speed2", m_animationData[1].speed);
	if (m_dataNum == 2) return;
	JsonHelper::AddBool(alloc, inProp, "loop3", m_animationData[2].isLoop);
	JsonHelper::AddInt(alloc, inProp, "start3", m_animationData[2].startTime);
	JsonHelper::AddInt(alloc, inProp, "end3", m_animationData[2].endTime);
	JsonHelper::AddInt(alloc, inProp, "lstart3", m_animationData[2].loopStartTime);
	JsonHelper::AddInt(alloc, inProp, "lend3", m_animationData[2].loopEndTime);
	JsonHelper::AddInt(alloc, inProp, "speed3", m_animationData[2].speed);
	if (m_dataNum == 3) return;
	JsonHelper::AddBool(alloc, inProp, "loop4", m_animationData[3].isLoop);
	JsonHelper::AddInt(alloc, inProp, "start4", m_animationData[3].startTime);
	JsonHelper::AddInt(alloc, inProp, "end4", m_animationData[3].endTime);
	JsonHelper::AddInt(alloc, inProp, "lstart4", m_animationData[3].loopStartTime);
	JsonHelper::AddInt(alloc, inProp, "lend4", m_animationData[3].loopEndTime);
	JsonHelper::AddInt(alloc, inProp, "speed4", m_animationData[3].speed);
	if (m_dataNum == 4) return;
	JsonHelper::AddBool(alloc, inProp, "loop5", m_animationData[4].isLoop);
	JsonHelper::AddInt(alloc, inProp, "start5", m_animationData[4].startTime);
	JsonHelper::AddInt(alloc, inProp, "end5", m_animationData[4].endTime);
	JsonHelper::AddInt(alloc, inProp, "lstart5", m_animationData[4].loopStartTime);
	JsonHelper::AddInt(alloc, inProp, "lend5", m_animationData[4].loopEndTime);
	JsonHelper::AddInt(alloc, inProp, "speed5", m_animationData[4].speed);
	if (m_dataNum == 5) return;
	JsonHelper::AddBool(alloc, inProp, "loop6", m_animationData[5].isLoop);
	JsonHelper::AddInt(alloc, inProp, "start6", m_animationData[5].startTime);
	JsonHelper::AddInt(alloc, inProp, "end6", m_animationData[5].endTime);
	JsonHelper::AddInt(alloc, inProp, "lstart6", m_animationData[5].loopStartTime);
	JsonHelper::AddInt(alloc, inProp, "lend6", m_animationData[5].loopEndTime);
	JsonHelper::AddInt(alloc, inProp, "speed6", m_animationData[6].speed);
	if (m_dataNum == 6) return;
	JsonHelper::AddBool(alloc, inProp, "loop7", m_animationData[6].isLoop);
	JsonHelper::AddInt(alloc, inProp, "start7", m_animationData[6].startTime);
	JsonHelper::AddInt(alloc, inProp, "end7", m_animationData[6].endTime);
	JsonHelper::AddInt(alloc, inProp, "lstart7", m_animationData[6].loopStartTime);
	JsonHelper::AddInt(alloc, inProp, "lend7", m_animationData[6].loopEndTime);
	JsonHelper::AddInt(alloc, inProp, "speed7", m_animationData[6].speed);
}