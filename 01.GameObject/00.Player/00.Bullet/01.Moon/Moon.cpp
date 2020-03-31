#include "stdafx.h"
#include "Moon.h"
#include "ScrollMgr.h"

CMoon::CMoon()
{
}


CMoon::~CMoon()
{
}

int CMoon::Update()
{
	if (m_bIsDead)
		return DEAD_OBJ;

	m_tInfo.vPos = m_pTarget->Get_Info().vPos;

	return NO_EVENT;
}

void CMoon::LateUpdate()
{
	m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDeltaTime();

	if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
	{
		m_bIsDead = true;
	}
}

void CMoon::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_strObjKey,
		m_tFrame.fCurFrame,
		m_strStateKey);

	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	D3DXMatrixTranslation(&m_tInfo.matWorld,
		m_tInfo.vPos.x - CScrollMgr::GetScroll(0),
		m_tInfo.vPos.y - CScrollMgr::GetScroll(1) - 40.f,
		0.f);

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CMoon::LineRender()
{
}

HRESULT CMoon::Initialize()
{
	m_strObjKey = L"Skill";
	m_strStateKey = L"Moon";

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 12.f;
	m_bIsDead = false;

	m_eObjType = CGameObject::BULLET;
	CSoundMgr::GetInstance()->MyPlaySound(L"Moon.mp3", CSoundMgr::MOON);
	return S_OK;
}

void CMoon::Release()
{
}

HRESULT CMoon::Create(CGameObject * pTarget)
{
	if (nullptr == pTarget)
		return E_FAIL;
	CMoon* pInstance = new CMoon;
	
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return E_FAIL;
	}

	pInstance->Set_Target(pTarget);

	HRESULT hr = CObjectMgr::GetInstance()->AddObject(CGameObject::BULLET, pInstance);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	return S_OK;
}
