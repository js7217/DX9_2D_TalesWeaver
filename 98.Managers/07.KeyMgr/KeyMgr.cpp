#include "stdafx.h"
#include "KeyMgr.h"

IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr()
	: m_dwCurKey(0), m_dwKeyDowned(0), m_dwKeyPressed(0)
{

}

CKeyMgr::~CKeyMgr()
{

}


void CKeyMgr::CheckKeyInput()
{
	m_dwCurKey = 0;

	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_dwCurKey |= KEY_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_dwCurKey |= KEY_DOWN;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_dwCurKey |= KEY_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_dwCurKey |= KEY_RIGHT;
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		m_dwCurKey |= KEY_RETURN;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_dwCurKey |= KEY_SPACE;
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwCurKey |= KEY_LBUTTON;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwCurKey |= KEY_RBUTTON;
	if (GetAsyncKeyState('R') & 0x8000)
		m_dwCurKey |= KEY_R;
	if (GetAsyncKeyState(VK_DELETE) & 0x8000)
		m_dwCurKey |= KEY_DELETE;
	if (GetAsyncKeyState(VK_INSERT) & 0x8000)
		m_dwCurKey |= KEY_INSERT;
	if (GetAsyncKeyState(VK_TAB) & 0x8000)
		m_dwCurKey |= KEY_TAB;
	if (GetAsyncKeyState(VK_F1) & 0x8000)
		m_dwCurKey |= KEY_F1;
	if (GetAsyncKeyState(VK_F2) & 0x8000)
		m_dwCurKey |= KEY_F2;
	if (GetAsyncKeyState(VK_F3) & 0x8000)
		m_dwCurKey |= KEY_F3;
	if (GetAsyncKeyState(VK_F4) & 0x8000)
		m_dwCurKey |= KEY_F4;
}

bool CKeyMgr::KeyDown(DWORD dwKey)
{
	// ���� ������ ���� ���� ������ �� true
	if (!(m_dwKeyDowned & dwKey) && (m_dwCurKey & dwKey))
	{
		m_dwKeyDowned |= dwKey;
		return true;
	}
	// ���� ������ �ְ� ���� ������ �ʾ��� �� false
	else if ((m_dwKeyDowned & dwKey) && !(m_dwCurKey & dwKey))
	{
		m_dwKeyDowned ^= dwKey;
		return false;
	}

	return false;
}

bool CKeyMgr::KeyUp(DWORD dwKey)
{
	// ���� ������ �ְ� ���� �� ������ �� true
	if ((m_dwKeyPressed & dwKey) && !(m_dwCurKey & dwKey))
	{
		m_dwKeyPressed ^= dwKey;
		return true;
	}
	// ���� ������ ���� ���� ������ �� false
	else if (!(m_dwKeyPressed & dwKey) && (m_dwCurKey & dwKey))
	{
		m_dwKeyPressed |= dwKey;
		return false;
	}

	return false;
}

bool CKeyMgr::KeyPressing(DWORD dwKey)
{
	if (m_dwCurKey & dwKey)
		return true;

	return false;
}

// Ctrl + C �Ǵ� Ctrl + V�� ���� ���.
bool CKeyMgr::KeyCombine(DWORD dwFirstKey, DWORD dwSecondKey)
{
	if (KeyDown(dwSecondKey) && KeyPressing(dwFirstKey))
		return true;

	return false;
}