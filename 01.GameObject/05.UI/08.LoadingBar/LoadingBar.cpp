#include "stdafx.h"
#include "LoadingBar.h"


CLoadingBar::CLoadingBar()
{
}


CLoadingBar::~CLoadingBar()
{
	Release();
}

int CLoadingBar::Update()
{
	m_iCLoading = (m_iWidth * m_iBar * 2) / m_iMaxBar;

	return NO_EVENT;
}

void CLoadingBar::LateUpdate()
{
	m_tRect = { 0, 0, m_iCLoading, 16 };
}

void CLoadingBar::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_strObjKey,
		0);
	NULL_CHECK(pTexInfo);

	D3DXMatrixTranslation(&m_tInfo.matWorld,
		m_tInfo.vPos.x,
		m_tInfo.vPos.y, 0.f);

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	const TEX_INFO* pTexBar = m_pTextureMgr->GetTexInfo(
		m_strObjKey,
		1);
	NULL_CHECK(pTexBar);

	D3DXMatrixTranslation(&m_tInfo.matWorld, 
		m_tInfo.vPos.x + 3.f, 
		m_tInfo.vPos.y + 54.f, 0.f);

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(pTexBar->pTexture, &m_tRect,
		nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CLoadingBar::LineRender()
{
}

HRESULT CLoadingBar::Initialize()
{
	m_strObjKey = L"Loading";

	m_iWidth = 324;

	m_iMaxBar = 480;
	m_iBar = 0;
	return S_OK;
}

void CLoadingBar::Release()
{
}

CLoadingBar * CLoadingBar::Create(float fX, float fY)
{
	CLoadingBar* pInstance = new CLoadingBar;

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}

	pInstance->Set_Pos(fX, fY);

	return pInstance;
}
