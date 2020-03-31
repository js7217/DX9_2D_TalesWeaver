#include "stdafx.h"
#include "ScrollMgr.h"

D3DXVECTOR3 CScrollMgr::m_vScroll = {};

CScrollMgr::CScrollMgr()
{
}


CScrollMgr::~CScrollMgr()
{
}

float CScrollMgr::GetScroll(int n)
{
	switch (n)
	{
	case 0:
		return m_vScroll.x;
	case 1:
		return m_vScroll.y;
	}

	return 0.0f;
}

void CScrollMgr::MoveScroll(const D3DXVECTOR3 & vMove)
{
	m_vScroll += vMove;
}

void CScrollMgr::ScrollLock(int fCX, int fCY)
{
	if (m_vScroll.x < 0.f)
		m_vScroll.x = 0.f;
	if (m_vScroll.y < 0.f)
		m_vScroll.y = 0.f;
	// 왼쪽 위쪽
	if (fCX - WINCX < m_vScroll.x)
		m_vScroll.x = fCX - WINCX;
	if (fCY - WINCY < m_vScroll.y)
		m_vScroll.y = fCY - WINCY;
	// 오른쪽 아래쪽
}

