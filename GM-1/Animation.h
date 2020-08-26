#pragma once
#include "Component.h"

struct AnimationData
{
	int startTime;
	int endTime;
	int loopStartTime;
	int loopEndTime;
	int speed;
	bool isLoop;
};
class Animation : public Component
{
private:
	AnimationData* m_animationData;
	int m_frame;
	int m_state;
	int m_defaultState;
	int m_dataNum;
	void UpdateFrame();
public:
	Animation();
	void Initialize();
	void Update();
	void Draw();
	void Finalize();
	void SetAnimationNum(int num);
	void SetAnimationData(int state, bool isLoop, int startTime, int endTime, int speed = 1, int loopStartTime = -1, int loopEndTime = -1);
	int GetTime();
	int GetState();
	void SetState(int state);
	void SetDefaultState(int state);
	void SetSpeed(int state, int speed);

	void LoadProperties(const rapidjson::Value& inProp) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp) override;
};

