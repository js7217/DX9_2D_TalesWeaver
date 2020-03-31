#include "stdafx.h"
#include "MonsterNameBar.h"
#include "ScrollMgr.h"

CMonsterNameBar::CMonsterNameBar()
{
}

CMonsterNameBar::CMonsterNameBar(const wstring & wstrName, CGameObject * pMonster)
{
	m_wstrName = wstrName;
	m_pTarget = pMonster;
}


CMonsterNameBar::~CMonsterNameBar()
{
	Release();
}

int CMonsterNameBar::Update()
{
	return NO_EVENT;
}

void CMonsterNameBar::LateUpdate()
{
	m_tInfo.vPos.x = m_pTarget->Get_Info().vPos.x;
	m_tInfo.vPos.y = m_pTarget->Get_Info().vPos.y +
		m_pTarget->Get_Info().fCY * 0.5f + 5.f;// 이게 적당한 위치. 

}

void CMonsterNameBar::Render()
{
	{
		D3DXMATRIX matScale, matTrans;

		D3DXMatrixScaling(&matScale, m_wstrName.length() * 3.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, 
			m_tInfo.vPos.x - CScrollMgr::GetScroll(0),
			m_tInfo.vPos.y - CScrollMgr::GetScroll(1),
			0.f);

		m_tInfo.matWorld = matScale * matTrans;

		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
			m_strObjKey,
			2,
			m_strStateKey);
		NULL_CHECK(pTexInfo);
		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans,
			m_tInfo.vPos.x - m_tInfo.fCX * 0.5f - CScrollMgr::GetScroll(0) + 7.f,
			m_tInfo.vPos.y - m_tInfo.fCY * 0.5f - CScrollMgr::GetScroll(1) + 6.f,
			0.f);
		m_tInfo.matWorld = matScale * matTrans;

		m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pGraphicDev->GetFont()->DrawTextW(CGraphicDev::GetInstance()->GetSprite(),
			m_wstrName.c_str(), m_wstrName.length(), nullptr, DT_CENTER, D3DCOLOR_ARGB(255, 0, 255, 0));
	}
	{
		D3DXMatrixTranslation(&m_tInfo.matWorld, 
			m_tInfo.vPos.x - m_tInfo.fCX * 0.5f - CScrollMgr::GetScroll(0),
			m_tInfo.vPos.y - CScrollMgr::GetScroll(1),
			0.f);
		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
			m_strObjKey,
			0,
			m_strStateKey);
		NULL_CHECK(pTexInfo);
		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	{
		D3DXMatrixTranslation(&m_tInfo.matWorld, 
			m_tInfo.vPos.x + m_tInfo.fCX * 0.5f - CScrollMgr::GetScroll(0),
			m_tInfo.vPos.y - CScrollMgr::GetScroll(1),
			0.f);
		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
			m_strObjKey,
			1,
			m_strStateKey);
		NULL_CHECK(pTexInfo);
		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CMonsterNameBar::LineRender()
{
}

HRESULT CMonsterNameBar::Initialize()
{
	m_tInfo.vPos.x = m_pTarget->Get_Info().vPos.x;
	m_tInfo.vPos.y = m_pTarget->Get_Info().vPos.y + m_pTarget->Get_Rect().bottom + 5.f; // 이게 적당한 위치. 

	m_strObjKey = L"Interface";
	m_strStateKey = L"MonsterNameBar";

	m_tInfo.fCX = 20.f * m_wstrName.length();
	m_tInfo.fCY = 24.f;

	return S_OK;
}

void CMonsterNameBar::Release()
{
}

CMonsterNameBar * CMonsterNameBar::Create(const wstring & wstrName, CGameObject * pMonster)
{
	CMonsterNameBar* pInstance = new CMonsterNameBar(wstrName, pMonster);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}

	return pInstance;
}
