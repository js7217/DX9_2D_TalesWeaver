#include "stdafx.h"
#include "Gate.h"

#include "ScrollMgr.h"
CGate::CGate()
{
}


CGate::~CGate()
{
}

int CGate::Update()
{
	return NO_EVENT;
}

void CGate::LateUpdate()
{
	m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDeltaTime();

	if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
		m_tFrame.fCurFrame = 0;
}

void CGate::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_strObjKey,
		int(m_tFrame.fCurFrame));

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

void CGate::LineRender()
{
}

HRESULT CGate::Initialize()
{
	m_strObjKey = L"WarpGate";
	
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 23.f;

	return S_OK;
}

void CGate::Release()
{
}

HRESULT CGate::Create(float fX, float fy)
{
	CGate* pInstance = new CGate;

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return E_FAIL;
	}

	pInstance->Set_Pos(fX, fy);

	HRESULT hr = CObjectMgr::GetInstance()->AddObject(CGameObject::EFFECT, pInstance);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	return S_OK;
}
