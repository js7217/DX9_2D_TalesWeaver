#include "stdafx.h"
#include "TimeUI.h"
#include "ScrollMgr.h"

CTimeUI::CTimeUI()
{
}


CTimeUI::~CTimeUI()
{
}

int CTimeUI::Update()
{
	m_tInfo.vPos = m_pTarget->Get_Info().vPos;

	return NO_EVENT;
}

void CTimeUI::LateUpdate()
{
	m_fTimer += m_pTimeMgr->GetDeltaTime();

	if (m_fTimer >= m_fChangeTime) // 타임이 넘었다면
	{
		if (!m_bIsNight) // 낮일때
		{
			m_fAlpha += m_pTimeMgr->GetDeltaTime() * 50.f;
			if (m_fAlpha >= 255.f)
			{
				m_fAlpha = 255.f;
				m_fTimer = 0.f;
				m_bIsNight = true;
			}
		}
		else // 밤일때
		{
			m_fAlpha -= m_pTimeMgr->GetDeltaTime() * 50.f;
			if (m_fAlpha <= 0.f)
			{
				m_fAlpha = 0.f;
				m_fTimer = 0.f;
				m_bIsNight = false;
			}
		}
	}
}

void CTimeUI::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_strObjKey,
		0);

	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	D3DXMatrixTranslation(&m_tInfo.matWorld,
		m_tInfo.vPos.x - CScrollMgr::GetScroll(0),
		m_tInfo.vPos.y - CScrollMgr::GetScroll(1),
		0.f);

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB((int)m_fAlpha, 255, 255, 255));
}

void CTimeUI::LineRender()
{
}

HRESULT CTimeUI::Initialize()
{
	m_fTimer = 0.f;
	m_fChangeTime = 60.f; // 2분
	m_fAlpha = 0.f;
	m_strObjKey = L"Night";
	m_eGroupID = GroupID::EFFECT;
	m_bIsNight = false;
	return S_OK;
}

void CTimeUI::Release()
{
}

HRESULT CTimeUI::Create(CGameObject* pTarget)
{
	CTimeUI* pInstance = new CTimeUI;

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return E_FAIL;
	}

	pInstance->Set_Target(pTarget);

	HRESULT hr = CObjectMgr::GetInstance()->AddObject(CGameObject::UI, pInstance);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	return S_OK;
}
