#include "stdafx.h"
#include "TargetUI.h"
#include "ScrollMgr.h"

CTargetUI::CTargetUI()
{
}

CTargetUI::CTargetUI(CGameObject * pTarget)
{
	m_pTarget = pTarget;
}


CTargetUI::~CTargetUI()
{
	Release();
}

int CTargetUI::Update()
{
	MoveFrame();

	return NO_EVENT;
}

void CTargetUI::LateUpdate()
{
	m_tInfo.vPos.x = m_pTarget->Get_Info().vPos.x;
	m_tInfo.vPos.y = m_pTarget->Get_Info().vPos.y - m_pTarget->Get_Info().fCY * 0.5f - 5.f;
}

void CTargetUI::Render()
{
	D3DXMatrixTranslation(&m_tInfo.matWorld,
		m_tInfo.vPos.x - CScrollMgr::GetScroll(0),
		m_tInfo.vPos.y - CScrollMgr::GetScroll(1),
		0.f);

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_strObjKey,
		int(m_tFrame.fCurFrame),
		m_strStateKey);

	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CTargetUI::LineRender()
{
}

HRESULT CTargetUI::Initialize()
{
	m_tInfo.vPos.x = m_pTarget->Get_Info().vPos.x;
	m_tInfo.vPos.y = m_pTarget->Get_Info().vPos.y + m_pTarget->Get_Info().fCY;

	m_strObjKey = L"Interface";
	m_strStateKey = L"Target";

	if (CGameObject::MONSTER == m_pTarget->Get_ObjType())
	{
		m_tFrame.fCurFrame = 4.f;
		m_tFrame.fMaxFrame = 7.f;
	}
	else
	{
		m_tFrame.fCurFrame = 0.f;
		m_tFrame.fMaxFrame = 3.f;
	}

	m_fStartFrame = m_tFrame.fCurFrame;
	return S_OK;
}

void CTargetUI::Release()
{
}

void CTargetUI::MoveFrame()
{
	m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDeltaTime();

	if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
		m_tFrame.fCurFrame = m_fStartFrame;
}

CTargetUI * CTargetUI::Create(CGameObject * pTarget)
{
	CTargetUI* pInstance = new CTargetUI(pTarget);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}


	return pInstance;
}
