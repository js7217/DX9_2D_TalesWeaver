#include "stdafx.h"
#include "SmallBar.h"
#include "UIObserver.h"
#include "ScrollMgr.h"
CSmallBar::CSmallBar()
{
}

CSmallBar::CSmallBar(CGameObject * pTarget)
	: m_pDataSubject(CDataSubject::GetInstance())
{
	m_pTarget = pTarget;
}


CSmallBar::~CSmallBar()
{
	Release();
}

int CSmallBar::Update()
{
	CGameObject::LateInit();

	m_tData.iMaxHp = dynamic_cast<CUIObserver*>(m_pObserver)->GetMaxHp();
	m_tData.iHp = dynamic_cast<CUIObserver*>(m_pObserver)->GetHp();
	if(m_tData.iMaxHp != 0)
		m_iCHp = (m_iWidth * m_tData.iHp) / m_tData.iMaxHp;
	
	
	m_tData.iMP = dynamic_cast<CUIObserver*>(m_pObserver)->GetMp();
	m_iMaxMp = dynamic_cast<CUIObserver*>(m_pObserver)->GetMaxMp();
	if (m_iMaxMp != 0)
		m_iCMp = (m_iWidth * m_tData.iMP) / m_iMaxMp;

	return NO_EVENT;
}

void CSmallBar::LateUpdate()
{
	m_tRect[0] = { 0, 0, m_iCHp, 3 };
	m_tRect[1] = { 0, 0, m_iCMp, 2 };
}

void CSmallBar::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_strObjKey,
		0,
		m_strStateKey);

	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	D3DXMatrixTranslation(&m_tInfo.matWorld, 
		m_pTarget->Get_Info().vPos.x - CScrollMgr::GetScroll(0), 
		m_pTarget->Get_Info().vPos.y - CScrollMgr::GetScroll(1) + 5,
		0.f);

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	for (int i = 0; i < 2; ++i)
	{
		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
			m_strObjKey,
			i + 1,
			m_strStateKey);
		NULL_CHECK(pTexInfo);

		D3DXMatrixTranslation(&m_tInfo.matWorld, 
			m_pTarget->Get_Info().vPos.x - fCenterX - CScrollMgr::GetScroll(0) + 1, 
			m_pTarget->Get_Info().vPos.y - fCenterY + (i * 2) - CScrollMgr::GetScroll(1) + 6,
			0.f);

		m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, &m_tRect[i],
			nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CSmallBar::LineRender()
{
}

HRESULT CSmallBar::Initialize()
{
	m_tInfo.vPos = m_pTarget->Get_Info().vPos;
	m_pObserver = CUIObserver::Create();
	m_pDataSubject->Subcribe(m_pObserver);

	m_strObjKey = L"Interface";
	m_strStateKey = L"SmallBar";
	m_iFrame = 0;
	m_iCHp, m_iCMp = 0;
	m_iWidth = 50;

	m_eObjType = CGameObject::UI;
	m_eGroupID = GroupID::UI;
	return S_OK;
}

HRESULT CSmallBar::LateInit()
{
	m_tData.iMaxHp = dynamic_cast<CUIObserver*>(m_pObserver)->GetMaxHp();
	m_tData.iHp = dynamic_cast<CUIObserver*>(m_pObserver)->GetHp();
	m_tData.iMP = dynamic_cast<CUIObserver*>(m_pObserver)->GetMp();
	m_iMaxMp = dynamic_cast<CUIObserver*>(m_pObserver)->GetMaxMp();

	return S_OK;
}

void CSmallBar::Release()
{
	m_pDataSubject->UnSubcribe(m_pObserver);
	SafeDelete(m_pObserver);
}

HRESULT CSmallBar::Create(CGameObject * pTarget)
{
	CSmallBar* pInstance = new CSmallBar(pTarget);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return E_FAIL;
	}

	HRESULT hr = CObjectMgr::GetInstance()->AddObject(CGameObject::UI, pInstance);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	return S_OK;
}
