#include "stdafx.h"
#include "DmgEffect.h"
#include "ScrollMgr.h"

CDmgEffect::CDmgEffect()
{
}

CDmgEffect::CDmgEffect(const D3DXVECTOR3 & vPos, const wstring & wstrStateKey, const int & iDmg)
{
	m_tInfo.vPos = vPos;
	m_strStateKey = wstrStateKey;
	m_iDmg = iDmg;
}


CDmgEffect::~CDmgEffect()
{
}

int CDmgEffect::Update()
{
	if (m_fTimer <= 0.f)
		return DEAD_OBJ;

	return NO_EVENT;
}

void CDmgEffect::LateUpdate()
{
	m_fTimer -= m_pTimeMgr->GetDeltaTime() * 2.f;

	m_tInfo.vPos.x +=  m_fSpeed * 0.5f * m_pTimeMgr->GetDeltaTime();

	m_tInfo.vPos.y -= 11.f * m_fJumpAccel - 9.8f * m_fJumpAccel * m_fJumpAccel * 0.5f;

	m_fJumpAccel += 0.1f;
}

void CDmgEffect::Render()
{
	int iDmg = m_iDmg;
	float fX = -20.f;

	while (iDmg != 0)
	{
		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
			m_strObjKey,
			int(iDmg % 10),
			m_strStateKey);

		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		D3DXMatrixTranslation(&m_tInfo.matWorld,
			m_tInfo.vPos.x - CScrollMgr::GetScroll(0) + fX,
			m_tInfo.vPos.y - CScrollMgr::GetScroll(1),
			0.f);

		m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	
		iDmg /= 10;
		fX -= pTexInfo->tImgInfo.Width;
	}
}

void CDmgEffect::LineRender()
{
}

HRESULT CDmgEffect::Initialize()
{
	m_strObjKey = L"Font";

	m_eGroupID = GroupID::EFFECT;
	m_eObjType = CGameObject::EFFECT;

	m_fSpeed = 150.f;

	m_fTimer = 1.f;
	m_fJumpAccel = 0.f;
	return S_OK;
}

void CDmgEffect::Release()
{
}

HRESULT CDmgEffect::Create(const D3DXVECTOR3 & vPos, const wstring & wstrStateKey, const int & iDmg)
{
	CDmgEffect* pInstance = new CDmgEffect(vPos, wstrStateKey, iDmg);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return E_FAIL;
	}

	HRESULT hr = CObjectMgr::GetInstance()->AddObject(CGameObject::EFFECT, pInstance);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	return S_OK;
}
