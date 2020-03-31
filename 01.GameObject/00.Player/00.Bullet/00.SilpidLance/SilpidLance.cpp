#include "stdafx.h"
#include "SilpidLance.h"
#include "ScrollMgr.h"

CSilpidLance::CSilpidLance()
{
}


CSilpidLance::~CSilpidLance()
{
}

int CSilpidLance::Update()
{
	CGameObject::LateInit();

	if (m_bIsDead)
		return DEAD_OBJ;
	CGameObject::UpdateRect_Scroll(0, -40.f);
	return NO_EVENT;
}

void CSilpidLance::LateUpdate()
{
	if (m_fDist > 0)
	{
		m_tInfo.vPos += m_tInfo.vDir * m_fSpeed * m_pTimeMgr->GetDeltaTime();
		m_fDist -= m_fSpeed * m_pTimeMgr->GetDeltaTime();
		m_bIsShake = true;
	}
	else
	{
		m_bIsDead = true;
		m_bIsShake = false;
	}
}

void CSilpidLance::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_strObjKey,
		0,
		m_strStateKey,
		m_strDirectionKey);

	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x - CScrollMgr::GetScroll(0),
		m_tInfo.vPos.y - CScrollMgr::GetScroll(1) - 40.f,
		0.f);

	m_tInfo.matWorld = matScale * matTrans;

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CSilpidLance::LineRender()
{
	CGameObject::UpdateRect_Scroll(0, -40.f);
	D3DXVECTOR2 vec[5] =
	{
		{ (float)m_tRect.left, (float)m_tRect.top },
		{ (float)m_tRect.left, (float)m_tRect.bottom },
		{ (float)m_tRect.right, (float)m_tRect.bottom },
		{ (float)m_tRect.right, (float)m_tRect.top },
		{ (float)m_tRect.left, (float)m_tRect.top }
	};

	m_pGraphicDev->GetLine()->Draw(vec, 5, D3DCOLOR_ARGB(255, 0, 255, 0));
}

HRESULT CSilpidLance::Initialize()
{
	CSoundMgr::GetInstance()->StopSound(CSoundMgr::SILPID);
	m_strObjKey = L"Skill";
	m_strStateKey = L"SilpidLance";

	m_eObjType = CGameObject::PLAYER; // 플레이어 것임.

	m_iAtt = 50.f;

	m_bIsDead = false;
	m_bIsShake = false;
	
	m_iShakeNum = 6;

	m_fDist = 400.f;
	m_fSpeed = 500.f;
	CSoundMgr::GetInstance()->MyPlaySound(L"SilpidLance.wav", CSoundMgr::SILPID);
	return S_OK;
}

HRESULT CSilpidLance::LateInit()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_strObjKey,
		0,
		m_strStateKey,
		m_strDirectionKey);


	m_tInfo.fCX = pTexInfo->tImgInfo.Width * 0.5f;
	m_tInfo.fCY = pTexInfo->tImgInfo.Height * 0.5f;

	return S_OK;
}

void CSilpidLance::Release()
{
}

HRESULT CSilpidLance::Create(const wstring & wstrDirectionKey, const INFO& tInfo)
{
	CSilpidLance* pInstance = new CSilpidLance;

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return E_FAIL;
	}

	pInstance->Set_Info(tInfo);
	pInstance->Set_DirectionKey(wstrDirectionKey);

	HRESULT hr = CObjectMgr::GetInstance()->AddObject(CGameObject::BULLET, pInstance);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	return S_OK;
}