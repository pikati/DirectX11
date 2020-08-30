#include "Fade.h"
#include "FPS.h"
#include "GameObject.h"
#include "Image.h"

void Fade::Initialize()
{
	m_image = gameObject->GetComponent<Image>();
}

void Fade::Update()
{
	if (m_isFadeIn) UpdateFadeIn();
	else if (m_isFadeOut) UpdateFadeOut();
}

void Fade::Draw()
{

}

void Fade::Finalize()
{

}

void Fade::SetFadeTime(float time)
{
	m_fadeTime = time;
}

void Fade::FadeIn()
{
	gameObject->activeSelf = true;
	m_isFadeIn = true;
	m_alpha = 0.0f;
}

void Fade::FadeOut()
{
	gameObject->activeSelf = true;
	m_isFadeOut = true;
	m_alpha = 1.0f;
}

void Fade::UpdateFadeIn()
{
	m_alpha += FPS::deltaTime * m_fadeTime;
	
	if (m_alpha >= 1.0f)
	{
		m_isFadeIn = false;
		m_alpha = 1.0f;
	}
	
	m_image->SetColor(1.0f, 1.0f, 1.0f, m_alpha);
}

void Fade::UpdateFadeOut()
{
	m_alpha -= FPS::deltaTime * m_fadeTime;

	if (m_alpha <= 0)
	{
		m_isFadeOut = false;
		m_alpha = 0;
		gameObject->activeSelf = false;
	}

	m_image->SetColor(1.0f, 1.0f, 1.0f, m_alpha);
}

bool Fade::IsFading()
{
	return m_isFadeIn || m_isFadeOut;
}