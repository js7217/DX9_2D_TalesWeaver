#pragma once
class CScrollMgr
{
public:
	CScrollMgr();
	~CScrollMgr();

public:
	static float GetScroll(int n);

public:
	static void MoveScroll(const D3DXVECTOR3& vMove);
	static void ScrollLock(int fCX, int fCY);
public:
	static void Set_Scroll(const D3DXVECTOR3& vScroll) { m_vScroll = vScroll; }
private:
	static D3DXVECTOR3 m_vScroll;
};

