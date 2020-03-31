#include "stdafx.h"
#include "MonsterHpBar.h"
#include "Monster.h"
#include "ScrollMgr.h"
CMonsterHpBar::CMonsterHpBar()
{
}

CMonsterHpBar::CMonsterHpBar(CGameObject * pMonster)
{
	m_pMonster = pMonster;
}


CMonsterHpBar::~CMonsterHpBar()
{
	Release();
}

int CMonsterHpBar::Update()
{
	m_iHp = dynamic_cast<CMonster*>(m_pMonster)->Get_Data().iHp;
	if (m_iMaxHp != 0)
		m_iCHp = (m_iWidth * m_iHp) / m_iMaxHp;
	if (m_iCHp <= 0)
		m_iCHp = 0;
	return NO_EVENT;
}

void CMonsterHpBar::LateUpdate()
{
	m_tInfo.vPos.x = m_pMonster->Get_Info().vPos.x;
	m_tInfo.vPos.y = m_pMonster->Get_Info().vPos.y + m_pMonster->Get_Info().fCY * 0.5f * 0.7f;

	m_tRect = { 0, 0, m_iCHp, 3 };
}

void CMonsterHpBar::Render()
{
	{
		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
			m_strObjKey,
			0,
			m_strStateKey);

		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		D3DXMatrixTranslation(&m_tInfo.matWorld,
			m_tInfo.vPos.x - CScrollMgr::GetScroll(0),
			m_tInfo.vPos.y - CScrollMgr::GetScroll(1),
			0.f);

		m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	{
		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
			m_strObjKey,
			1,
			m_strStateKey);

		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		D3DXMatrixTranslation(&m_tInfo.matWorld,
			m_tInfo.vPos.x - fCenterX - CScrollMgr::GetScroll(0),
			m_tInfo.vPos.y - fCenterY - CScrollMgr::GetScroll(1),
			0.f);

		m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, &m_tRect,
			nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CMonsterHpBar::LineRender()
{
}

HRESULT CMonsterHpBar::Initialize()
{
	m_tInfo.vPos.x = m_pMonster->Get_Info().vPos.x;
	m_tInfo.vPos.y = m_pMonster->Get_Info().vPos.y + m_pMonster->Get_Info().fCY * 0.7f;
	m_strObjKey = L"Interface";
	m_strStateKey = L"MonsterHpBar";

	m_iWidth = 50.f;

	m_iMaxHp = dynamic_cast<CMonster*>(m_pMonster)->Get_Data().iMaxHp;
	m_iHp = dynamic_cast<CMonster*>(m_pMonster)->Get_Data().iHp;
	if(m_iMaxHp != 0)
		m_iCHp = (m_iWidth * m_iHp) / m_iMaxHp;
	return S_OK;
}

void CMonsterHpBar::Release()
{
}

CMonsterHpBar * CMonsterHpBar::Create(CGameObject * pMonster)
{
	CMonsterHpBar* pInstance = new CMonsterHpBar(pMonster);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}

	return pInstance;
}
