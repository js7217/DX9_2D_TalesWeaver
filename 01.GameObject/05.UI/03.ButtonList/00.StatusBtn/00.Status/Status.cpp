#include "stdafx.h"
#include "Status.h"
#include "UIObserver.h"
#include "CloseBtn.h"
#include "Player.h"
CStatus::CStatus()
{
}

CStatus::CStatus(const D3DXVECTOR3 & vPos)
	: m_pDataSubject(CDataSubject::GetInstance())
	, m_bIsClose(false)
{
	m_tInfo.fCX = 345.f;
	m_tInfo.fCY = 305.f;

	m_tInfo.vPos.x = vPos.x - m_tInfo.fCX; // 이때 vPos는 Right Top
	m_tInfo.vPos.y = vPos.y;
	m_tInfo.vPos.z = 0.f;

	m_pCloseBtn = CCloseBtn::Create(vPos);
}


CStatus::~CStatus()
{
	Release();
}

int CStatus::Update()
{
	if (m_bIsClose)
		return NO_EVENT;

	CGameObject::LateInit();
	CGameObject::UpdateRect(m_tInfo.fCX * 0.5f, m_tInfo.fCY * 0.5f);
	m_pCloseBtn->Update();


	m_tData.iMaxHp = dynamic_cast<CUIObserver*>(m_pObserver)->GetMaxHp();
	m_iMaxMp = dynamic_cast<CUIObserver*>(m_pObserver)->GetMaxMp();
	m_fMaxSp = dynamic_cast<CUIObserver*>(m_pObserver)->GetMaxSp();

	m_tData.iHp = dynamic_cast<CUIObserver*>(m_pObserver)->GetHp();
	m_tData.iMP = dynamic_cast<CUIObserver*>(m_pObserver)->GetMp();
	m_fSp = dynamic_cast<CUIObserver*>(m_pObserver)->GetSp();
	m_iSeed = dynamic_cast<CUIObserver*>(m_pObserver)->GetSeed();
	m_iLevel = dynamic_cast<CUIObserver*>(m_pObserver)->GetLevel();
	m_iExp = dynamic_cast<CUIObserver*>(m_pObserver)->GetExp();
	m_iMaxExp = dynamic_cast<CUIObserver*>(m_pObserver)->GetMaxExp();

	m_iCExp = (m_iWidth * m_iExp) / m_iMaxExp;


	return NO_EVENT;
}

void CStatus::LateUpdate()
{
	if (m_bIsClose)
		return;
	CGameObject::UpdateRect(m_tInfo.fCX * 0.5f, m_tInfo.fCY * 0.5f);
	POINT pt = {};

	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tRect, pt))
	{
		dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->GetObject(PLAYER))->Set_Move(false);
		m_bIsClose = false;
		
	}
	else
	{
		m_bIsClose = true;
	}
	
	m_pCloseBtn->LateUpdate();

	m_bIsClose = dynamic_cast<CCloseBtn*>(m_pCloseBtn)->Get_Click();

	m_tRectExp = { 0, 0, m_iCExp, 2 };
}

void CStatus::Render()
{
	if (m_bIsClose)
		return;
	CGameObject::UpdateRect(m_tInfo.fCX * 0.5f, m_tInfo.fCY * 0.5f);

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_strObjKey,
		0,
		m_strStateKey);
	NULL_CHECK(pTexInfo);

	D3DXMatrixTranslation(&m_tInfo.matWorld, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pCloseBtn->Render();

	RenderText();

	const TEX_INFO* pTexExp = m_pTextureMgr->GetTexInfo( // 경험치바
		L"Interface",
		3,
		L"StateBar");
	NULL_CHECK(pTexExp);

	D3DXMatrixTranslation(&m_tInfo.matWorld,
		m_tInfo.vPos.x + 35.f,
		m_tInfo.vPos.y + 147.f,
		0.f);

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(pTexExp->pTexture, &m_tRectExp,
		nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CStatus::LineRender()
{
}

HRESULT CStatus::Initialize()
{
	m_pObserver = CUIObserver::Create();
	m_pDataSubject->Subcribe(m_pObserver);

	m_strObjKey = L"Interface";
	m_strStateKey = L"StateWindow";

	m_tInfo.fCX = 345.f;
	m_tInfo.fCY = 305.f;

	m_iWidth = 300;
	m_iCExp = 0;

	m_eObjType = CGameObject::UI;

	return S_OK;
}

HRESULT CStatus::LateInit()
{
	m_tData.iMaxHp = dynamic_cast<CUIObserver*>(m_pObserver)->GetMaxHp();
	m_iMaxMp = dynamic_cast<CUIObserver*>(m_pObserver)->GetMaxMp();
	m_fMaxSp = dynamic_cast<CUIObserver*>(m_pObserver)->GetMaxSp();

	m_tData.iHp = dynamic_cast<CUIObserver*>(m_pObserver)->GetHp();
	m_tData.iMP = dynamic_cast<CUIObserver*>(m_pObserver)->GetMp();
	m_fSp = dynamic_cast<CUIObserver*>(m_pObserver)->GetSp();
	m_iSeed = dynamic_cast<CUIObserver*>(m_pObserver)->GetSeed();
	m_iLevel = dynamic_cast<CUIObserver*>(m_pObserver)->GetLevel();
	m_iExp = dynamic_cast<CUIObserver*>(m_pObserver)->GetExp();
	m_iMaxExp = dynamic_cast<CUIObserver*>(m_pObserver)->GetMaxExp();

	return S_OK;
}

void CStatus::Release()
{
	SafeDelete(m_pCloseBtn);
	m_pDataSubject->UnSubcribe(m_pObserver);
	SafeDelete(m_pObserver);
}

CStatus* CStatus::Create(const D3DXVECTOR3& vPos)
{
	CStatus* pInstance = new CStatus(vPos);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}

	return pInstance;
}

void CStatus::RenderText()
{
	wstring wstrName = L"막시민";
	wstring wstrType = L"베기";
	wstring wstrHp = to_wstring(m_tData.iHp) + L" / " + to_wstring(m_tData.iMaxHp);
	wstring wstrMp = to_wstring(m_tData.iMP) + L" / " + to_wstring(m_iMaxMp);
	wstring wstrSp = to_wstring((int)m_fSp) + L" / " + to_wstring((int)m_fMaxSp);
	wstring wstrSeed = to_wstring(m_iSeed);
	wstring wstrLevel = to_wstring(m_iLevel);
	wstring wstrExp = to_wstring(m_iExp);
	wstring wstrMaxExp = to_wstring(m_iMaxExp);

	D3DXMatrixTranslation(&m_tInfo.matWorld, // 이름
		m_tInfo.vPos.x + 250.f,
		m_tInfo.vPos.y + 57.f,
		0.f);

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetFont()->DrawTextW(CGraphicDev::GetInstance()->GetSprite(),
		wstrName.c_str(), wstrName.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));


	D3DXMatrixTranslation(&m_tInfo.matWorld, // 타입
		m_tInfo.vPos.x + 255.f,
		m_tInfo.vPos.y + 76.f,
		0.f);
	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetFont()->DrawTextW(CGraphicDev::GetInstance()->GetSprite(),
		wstrType.c_str(), wstrType.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));


	D3DXMatrixTranslation(&m_tInfo.matWorld, // HP
		m_tInfo.vPos.x + 240.f,
		m_tInfo.vPos.y + 182.f,
		0.f);
	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetFont()->DrawTextW(CGraphicDev::GetInstance()->GetSprite(),
		wstrHp.c_str(), wstrHp.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixTranslation(&m_tInfo.matWorld, // MP
		m_tInfo.vPos.x + 240.f,
		m_tInfo.vPos.y + 197.f,
		0.f);
	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetFont()->DrawTextW(CGraphicDev::GetInstance()->GetSprite(),
		wstrMp.c_str(), wstrMp.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixTranslation(&m_tInfo.matWorld, // SP
		m_tInfo.vPos.x + 240.f,
		m_tInfo.vPos.y + 212.f,
		0.f);
	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetFont()->DrawTextW(CGraphicDev::GetInstance()->GetSprite(),
		wstrSp.c_str(), wstrSp.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixTranslation(&m_tInfo.matWorld, // SEED
		m_tInfo.vPos.x + 240.f,
		m_tInfo.vPos.y + 227.f,
		0.f);
	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetFont()->DrawTextW(CGraphicDev::GetInstance()->GetSprite(),
		wstrSeed.c_str(), wstrSeed.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixTranslation(&m_tInfo.matWorld, // Level
		m_tInfo.vPos.x + 220.f,
		m_tInfo.vPos.y + 40.f,
		0.f);
	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetFont()->DrawTextW(CGraphicDev::GetInstance()->GetSprite(),
		wstrLevel.c_str(), wstrLevel.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixTranslation(&m_tInfo.matWorld, // exp
		m_tInfo.vPos.x + 100.f,
		m_tInfo.vPos.y + 135.f,
		0.f);
	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetFont()->DrawTextW(CGraphicDev::GetInstance()->GetSprite(),
		wstrExp.c_str(), wstrExp.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixTranslation(&m_tInfo.matWorld, // Maxexp
		m_tInfo.vPos.x + 200.f,
		m_tInfo.vPos.y + 135.f,
		0.f);
	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetFont()->DrawTextW(CGraphicDev::GetInstance()->GetSprite(),
		wstrMaxExp.c_str(), wstrMaxExp.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
}
