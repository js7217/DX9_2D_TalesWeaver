#include "stdafx.h"
#include "GameObject.h"
#include "ScrollMgr.h"

CGameObject::CGameObject()
	: m_pTimeMgr(CTimeMgr::GetInstance())
	, m_pTextureMgr(CTextureMgr::GetInstance())
	, m_pGraphicDev(CGraphicDev::GetInstance())
	, m_strObjKey(L"NONE")
	, m_strStateKey(L"NONE")
	, m_strDirectionKey(L"NONE")
	, m_eObjType(CGameObject::END)
	, m_eGroupID(GroupID::GAMEOBJECT)
{
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tInfo, sizeof(INFO));
}


CGameObject::~CGameObject()
{
}

void CGameObject::Set_Pos(float fX, float fY)
{
	m_tInfo.vPos.x = fX;
	m_tInfo.vPos.y = fY;
	m_tInfo.vPos.z = 0.f;
}

HRESULT CGameObject::LateInit()
{
	if (!m_bIsInit)
	{
		this->LateInit();
		m_bIsInit = true;
	}

	return S_OK;
}

void CGameObject::UpdateRect(const float& fWidth, const float& fHeight) //인포, 가로, 세로길이, 좌우 이동, 상하 이동
{
	m_tRect.left = LONG(m_tInfo.vPos.x - (m_tInfo.fCX / 2.f) + fWidth);
	m_tRect.top = LONG(m_tInfo.vPos.y - (m_tInfo.fCY / 2.f) + fHeight);
	m_tRect.right = LONG(m_tInfo.vPos.x + (m_tInfo.fCX / 2.f) + fWidth);
	m_tRect.bottom = LONG(m_tInfo.vPos.y + (m_tInfo.fCY / 2.f) + fHeight);
}

void CGameObject::UpdateRect_Scroll(const float & fWidth, const float & fHeight)
{
	m_tRect.left = LONG(m_tInfo.vPos.x - (m_tInfo.fCX / 2.f) + fWidth) - CScrollMgr::GetScroll(0);
	m_tRect.top = LONG(m_tInfo.vPos.y - (m_tInfo.fCY / 2.f) + fHeight) - CScrollMgr::GetScroll(1);
	m_tRect.right = LONG(m_tInfo.vPos.x + (m_tInfo.fCX / 2.f) + fWidth) - CScrollMgr::GetScroll(0);
	m_tRect.bottom = LONG(m_tInfo.vPos.y + (m_tInfo.fCY / 2.f) + fHeight) - CScrollMgr::GetScroll(1);
}