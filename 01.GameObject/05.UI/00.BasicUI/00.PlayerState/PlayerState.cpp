#include "stdafx.h"
#include "PlayerState.h"

int CPlayerState::m_iIconFrame = 0;

CPlayerState::CPlayerState()
{
}


CPlayerState::~CPlayerState()
{
}

int CPlayerState::Update()
{
	return NO_EVENT;
}

void CPlayerState::LateUpdate()
{
	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x,
		m_tInfo.vPos.y,
		0.f);

	m_tInfo.matWorld = matScale * matTrans;
}

void CPlayerState::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_strObjKey,
		0,
		m_strStateKey);

	NULL_CHECK(pTexInfo);

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	const TEX_INFO* pTexState = m_pTextureMgr->GetTexInfo(
		m_strObjKey,
		1,
		m_strStateKey);

	NULL_CHECK(pTexInfo);

	D3DXMatrixTranslation(&m_tInfo.matWorld,
		m_tInfo.vPos.x,
		m_tInfo.vPos.y + m_tInfo.fCY,
		0.f);

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(pTexState->pTexture, nullptr,
		nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	const TEX_INFO* pTexIcon = m_pTextureMgr->GetTexInfo(
		L"SkillIcon",
		m_iIconFrame);

	NULL_CHECK(pTexIcon);

	D3DXMatrixTranslation(&m_tInfo.matWorld,
		m_tInfo.vPos.x + 68.f,
		m_tInfo.vPos.y + 86.f,
		0.f);

	float fCenterX = pTexIcon->tImgInfo.Width * 0.5f;
	float fCenterY = pTexIcon->tImgInfo.Height * 0.5f;

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(pTexIcon->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CPlayerState::LineRender()
{
}

HRESULT CPlayerState::Initialize()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };

	m_tInfo.fCX = 256.f;
	m_tInfo.fCY = 128.f;

	m_strObjKey = L"Interface";
	m_strStateKey = L"PlayerStatus";

	m_eObjType = CGameObject::UI;
	m_eGroupID = GroupID::UI;
	return S_OK;
}

void CPlayerState::Release()
{
}

HRESULT CPlayerState::Create()
{
	CPlayerState* pInstance = new CPlayerState;

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return E_FAIL;
	}

	HRESULT hr = CObjectMgr::GetInstance()->AddObject(CGameObject::UI, pInstance);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	return S_OK;
}
