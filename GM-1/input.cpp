#include "main.h"
#include "input.h"
#include <string>

BYTE CInput::m_OldKeyState[256];
BYTE CInput::m_KeyState[256];
Vector2 CInput::m_mousePosition;
bool CInput::m_isLTrigger = false;
bool CInput::m_isRTrigger = false;
bool CInput::m_isMTrigger = false;
bool CInput::m_isLDown = false;
bool CInput::m_isRDown = false;
bool CInput::m_isMDown = false;
bool CInput::m_isLUp = false;
bool CInput::m_isRUp = false;
bool CInput::m_isMUp = false;

void CInput::Init()
{

	memset( m_OldKeyState, 0, 256 );
	memset( m_KeyState, 0, 256 );

}

void CInput::Uninit()
{


}

void CInput::Update()
{
	POINT pos;
	GetCursorPos(&pos);
	m_mousePosition.Set(pos.x, pos.y);
	memcpy( m_OldKeyState, m_KeyState, 256 );

	m_isLTrigger = false;
	m_isRTrigger = false;
	m_isMTrigger = false;
	m_isLUp = false;
	m_isRUp = false;
	m_isMUp = false;

	GetKeyboardState( m_KeyState );
	//左クリック関係//
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if(!m_isLDown) m_isLTrigger = true;
		m_isLDown = true;
	}
	else if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
	{
		if (m_isLDown) m_isLUp = true;
		m_isLDown = false;
	}
	
	//右クリック関係//
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		if(!m_isRDown) m_isRTrigger = true;
		m_isRDown = true;
	}
	else if (!(GetAsyncKeyState(VK_RBUTTON) & 0x8000))
	{
		if (m_isRDown) m_isRUp = true;
		m_isRDown = false;
	}

	//中クリック関係//
	if (GetAsyncKeyState(VK_MBUTTON) & 0x8000)
	{
		if(!m_isMDown) m_isMTrigger = true;
		m_isMDown = true;
	}
	else if (!(GetAsyncKeyState(VK_MBUTTON) & 0x8000))
	{
		if (m_isMDown) m_isMUp = true;
		m_isMDown = false;
	}
}

bool CInput::GetKeyPress(BYTE KeyCode)
{
	return (m_KeyState[KeyCode] & 0x80);
}

bool CInput::GetKeyTrigger(BYTE KeyCode)
{
	return ((m_KeyState[KeyCode] & 0x80) && !(m_OldKeyState[KeyCode] & 0x80));
}

bool CInput::GetMouseTrigger(int buttonNum)
{
	if (buttonNum == 0)
	{
		return m_isLTrigger;
	}
	else if (buttonNum == 1)
	{
		return m_isRTrigger;
	}
	else if (buttonNum == 2)
	{
		return m_isMTrigger;
	}
	else
	{
		return false;
	}
}

bool CInput::GetMouseDown(int buttonNum)
{
	if (buttonNum == 0)
	{
		return m_isLDown;
	}
	else if (buttonNum == 1)
	{
		return m_isRDown;
	}
	else if (buttonNum == 2)
	{
		return m_isMDown;
	}
	else
	{
		return false;
	}
}

bool CInput::GetMouseUp(int buttonNum)
{
	if (buttonNum == 0)
	{
		return m_isLUp;
	}
	else if (buttonNum == 1)
	{
		return m_isRUp;
	}
	else if (buttonNum == 2)
	{
		return m_isMUp;
	}
	else
	{
		return false;
	}
}

Vector2 CInput::GetMousePosition()
{
	return m_mousePosition;
}