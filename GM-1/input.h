#pragma once
#include "Engine.h"

class CInput
{
private:
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];
	//static Vector2 m_oldMousePosition;
	static Vector2 m_mousePosition;
	static bool m_isLTrigger;
	static bool m_isRTrigger;
	static bool m_isMTrigger;
	static bool m_isLDown;
	static bool m_isRDown;
	static bool m_isMDown;
	static bool m_isLUp;
	static bool m_isRUp;
	static bool m_isMUp;

public:
	static void Init();
	static void Uninit();
	static void Update();

	static bool GetKeyPress( BYTE KeyCode );
	static bool GetKeyTrigger( BYTE KeyCode );
	static bool GetMouseTrigger(int buttonNum);
	static bool GetMouseDown(int buttonNum);
	static bool GetMouseUp(int buttonNum);
	static Vector2 GetMousePosition();
};
