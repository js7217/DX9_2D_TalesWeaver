#include "stdafx.h"
#include "ComboEffect.h"
#include "ScrollMgr.h"
#include "Player.h"
CComboEffect::CComboEffect()
{
}

CComboEffect::CComboEffect(CGameObject* pTarget, const int & iCombo)
{
	m_pTarget = pTarget;
	m_iCombo = iCombo;
}


CComboEffect::~CComboEffect()
{
}

int CComboEffect::Update()
{
	return NO_EVENT;
}

void CComboEffect::LateUpdate()
{
	m_tInfo.vPos.x = m_pTarget->Get_Info().vPos.x;
	m_tInfo.vPos.y = m_pTarget->Get_Info().vPos.y - m_pTarget->Get_Info().fCY;
}

void CComboEffect::Render()
{
	int iCombo = m_iCombo;
	float fX = 0.f;

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		L"Interface",
		0,
		L"Combo");

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

	while (iCombo != 0)
	{
		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
			m_strObjKey,
			int(iCombo % 10),
			m_strStateKey);

		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		D3DXMatrixTranslation(&m_tInfo.matWorld,
			m_tInfo.vPos.x - CScrollMgr::GetScroll(0) + fX,
			m_tInfo.vPos.y - CScrollMgr::GetScroll(1) - 10.f,
			0.f);

		m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		iCombo /= 10;
		fX -= pTexInfo->tImgInfo.Width;
	}
}

void CComboEffect::LineRender()
{
}

HRESULT CComboEffect::Initialize()
{
	m_strObjKey = L"Effect";
	m_strStateKey = L"Combo";

	m_tInfo.vPos = m_pTarget->Get_Info().vPos;

	m_eGroupID = GroupID::EFFECT;
	m_eObjType = CGameObject::EFFECT;

	return S_OK;
}

void CComboEffect::Release()
{
}

HRESULT CComboEffect::Create(CGameObject* pTarget, const int & iCombo)
{
	CComboEffect* pInstance = new CComboEffect(pTarget, iCombo);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return E_FAIL;
	}

	HRESULT hr = CObjectMgr::GetInstance()->AddObject(CGameObject::EFFECT, pInstance);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	return S_OK;
}
