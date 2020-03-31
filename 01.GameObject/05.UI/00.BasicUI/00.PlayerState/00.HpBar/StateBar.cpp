#include "stdafx.h"
#include "StateBar.h"
#include "UIObserver.h"

CStateBar::CStateBar()
	: m_pDataSubject(CDataSubject::GetInstance())
{
}


CStateBar::~CStateBar()
{
	Release();
}

int CStateBar::Update()
{
	CGameObject::LateInit();

	if (nullptr == CObjectMgr::GetInstance()->GetObjectW(CGameObject::PLAYER))
		return DEAD_OBJ;

	m_tData.iMaxHp = dynamic_cast<CUIObserver*>(m_pObserver)->GetMaxHp();
	m_tData.iHp = dynamic_cast<CUIObserver*>(m_pObserver)->GetHp();
	if(m_tData.iMaxHp != 0)
		m_iCHp = (m_iWidth * m_tData.iHp) / m_tData.iMaxHp;

	m_iMaxMp = dynamic_cast<CUIObserver*>(m_pObserver)->GetMaxMp();
	m_tData.iMP = dynamic_cast<CUIObserver*>(m_pObserver)->GetMp();
	if (m_iMaxMp != 0)
	m_iCMp = (m_iWidth * m_tData.iMP) / m_iMaxMp;

	m_fMaxSp = dynamic_cast<CUIObserver*>(m_pObserver)->GetMaxSp();
	m_fSp = dynamic_cast<CUIObserver*>(m_pObserver)->GetSp();
	if (m_fMaxSp != 0.f)
		m_iCSp = (int)((m_iWidth * m_fSp) / m_fMaxSp);

	m_iMaxExp = dynamic_cast<CUIObserver*>(m_pObserver)->GetMaxExp();
	m_iExp = dynamic_cast<CUIObserver*>(m_pObserver)->GetExp();

	return NO_EVENT;
}

void CStateBar::LateUpdate()
{
	m_tRect[0] = { 0, 0, m_iCHp, 14 };
	m_tRect[1] = { 0, 0, m_iCMp, 14 };
	m_tRect[2] = { 0, 0, m_iCSp, 14 };
}

void CStateBar::Render()
{
	for (int i = 0; i < 3; ++i)
	{
		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
			m_strObjKey,
			i,
			m_strStateKey);
		NULL_CHECK(pTexInfo);

		D3DXMatrixTranslation(&m_tInfo.matWorld, m_tInfo.vPos.x, m_tInfo.vPos.y + (i * 15), 0.f);

		m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, &m_tRect[i],
			nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	RenderText();
}

void CStateBar::LineRender()
{
}

HRESULT CStateBar::Initialize()
{
	m_tInfo.vPos = { 98.f, 21.f, 0.f };
	// 98, 36 / 98. 51 -> 15씩 증가
	m_pObserver = CUIObserver::Create();
	m_pDataSubject->Subcribe(m_pObserver);

	m_strObjKey = L"Interface";
	m_strStateKey = L"StateBar";
	m_iFrame = 0;

	m_iCHp = 0;
	m_iCMp = 0;
	m_iCSp = 0;

	m_iWidth = 84;

	m_eObjType = CGameObject::UI;
	m_eGroupID = GroupID::UI;
	return S_OK;
}

HRESULT CStateBar::LateInit()
{
	m_tData.iMaxHp	= dynamic_cast<CUIObserver*>(m_pObserver)->GetMaxHp();
	m_iMaxMp		= dynamic_cast<CUIObserver*>(m_pObserver)->GetMaxMp();
	m_fMaxSp		= dynamic_cast<CUIObserver*>(m_pObserver)->GetMaxSp();
	m_iMaxExp		= dynamic_cast<CUIObserver*>(m_pObserver)->GetMaxExp();
	m_tData.iHp		= dynamic_cast<CUIObserver*>(m_pObserver)->GetHp();
	m_tData.iMP		= dynamic_cast<CUIObserver*>(m_pObserver)->GetMp();
	m_fSp			= dynamic_cast<CUIObserver*>(m_pObserver)->GetSp();
	m_iExp			= dynamic_cast<CUIObserver*>(m_pObserver)->GetExp();
	
	return S_OK;
}

void CStateBar::Release()
{
	m_pDataSubject->UnSubcribe(m_pObserver);
	SafeDelete(m_pObserver);
}

HRESULT CStateBar::Create()
{
	CStateBar* pInstance = new CStateBar;

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return E_FAIL;
	}

	HRESULT hr = CObjectMgr::GetInstance()->AddObject(CGameObject::UI, pInstance);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	return S_OK;
}

void CStateBar::RenderText()
{
	wstring wstrHp = to_wstring(m_tData.iHp) + L" / " + to_wstring(m_tData.iMaxHp);
	wstring wstrMp = to_wstring(m_tData.iMP) + L" / " + to_wstring(m_iMaxMp);
	wstring wstrSp = to_wstring((int)m_fSp) + L" / " + to_wstring((int)m_fMaxSp);
	wstring wstrExp = to_wstring(m_iExp) + L" / " + to_wstring(m_iMaxExp);

	D3DXMatrixTranslation(&m_tInfo.matWorld, // 15간격
		m_tInfo.vPos.x - 50.f,
		m_tInfo.vPos.y + 105.f,
		0.f);

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetFont()->DrawTextW(CGraphicDev::GetInstance()->GetSprite(),
		wstrHp.c_str(), wstrHp.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixTranslation(&m_tInfo.matWorld,
		m_tInfo.vPos.x - 50.f,
		m_tInfo.vPos.y + 120.f,
		0.f);

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetFont()->DrawTextW(CGraphicDev::GetInstance()->GetSprite(),
		wstrMp.c_str(), wstrMp.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixTranslation(&m_tInfo.matWorld,
		m_tInfo.vPos.x - 50.f,
		m_tInfo.vPos.y + 135.f,
		0.f);

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetFont()->DrawTextW(CGraphicDev::GetInstance()->GetSprite(),
		wstrSp.c_str(), wstrSp.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixTranslation(&m_tInfo.matWorld,
		m_tInfo.vPos.x - 50.f,
		m_tInfo.vPos.y + 150.f,
		0.f);

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetFont()->DrawTextW(CGraphicDev::GetInstance()->GetSprite(),
		wstrExp.c_str(), wstrExp.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
}
