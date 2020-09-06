#pragma once
#include "Component.h"
class Image;

class Fade : public Component
{
private:
	Image* m_image;
	float m_alpha = 0;
	float m_fadeTime = 0.5f;
	bool m_isFadeIn = false;
	bool m_isFadeOut = false;

	void UpdateFadeIn();
	void UpdateFadeOut();
public:
	void Initialize();
	void Update();

	void SetFadeTime(float time);
	void FadeIn();
	void FadeOut();

	bool IsFading();
};

