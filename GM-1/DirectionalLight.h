#pragma once
#include "renderer.h"
#include "Component.h"

class DirectionalLight : public Component
{
private:
	LIGHT m_light;
public:
	void Initialize() override;
	void Update() override;

	void DrawInformation() override;

	//void LoadProperties(const rapidjson::Value& inProp) override;
	//void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inProp) override;
};

