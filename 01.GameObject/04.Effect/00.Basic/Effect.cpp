#include "stdafx.h"
#include "Effect.h"
#include "ScrollMgr.h"

CEffect::CEffect()
{
}

CEffect::CEffect(const D3DXMATRIX& matWorld, const FRAME& tFrame, const float& fSpeed
	, const wstring& wstrStateKey, const wstring& wstrDirectionKey = L"NONE")
{
	m_tInfo.matWorld = matWorld;
	m_tFrame = tFrame;
	m_fSpeed = fSpeed;
	m_strStateKey = wstrStateKey;
	m_strDirectionKey = wstrDirectionKey;
}


CEffect::~CEffect()
{
	Release();
}

int CEffect::Update()
{
	m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDeltaTime() * m_fSpeed;

	if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
		return DEAD_OBJ;

	return NO_EVENT;
}

void CEffect::LateUpdate()
{
}

void CEffect::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_strObjKey,
		int(m_tFrame.fCurFrame),
		m_strStateKey,
		m_strDirectionKey);

	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CEffect::LineRender()
{
}

HRESULT CEffect::Initialize()
{
	m_strObjKey = L"Effect";
	m_eGroupID = GroupID::EFFECT;
	m_eObjType = CGameObject::EFFECT;
	return S_OK;
}

void CEffect::Release()
{
}

HRESULT CEffect::Create(const D3DXMATRIX& matWorld, const FRAME& tFrame, const float& fSpeed
	, const wstring& wstrStateKey, const wstring& wstrDirectionKey)
{
	CEffect* pInstance = new CEffect(matWorld, tFrame, fSpeed, wstrStateKey, wstrDirectionKey);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return E_FAIL;
	}

	HRESULT hr = CObjectMgr::GetInstance()->AddObject(CGameObject::EFFECT, pInstance);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	return S_OK;
}
