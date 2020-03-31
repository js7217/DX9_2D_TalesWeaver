#include "stdafx.h"
#include "QuickSlot.h"
#include "SkillIcon.h"

CQuickSlot::CQuickSlot()
{
}


CQuickSlot::~CQuickSlot()
{
}

int CQuickSlot::Update()
{
	KeyCheck();
	return S_OK;
}

void CQuickSlot::LateUpdate()
{
	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x,
		m_tInfo.vPos.y,
		0.f);

	m_tInfo.matWorld = matScale * matTrans;
}

void CQuickSlot::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_strObjKey,
		m_iFrame,
		m_strStateKey,
		m_strDirectionKey);

	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CQuickSlot::LineRender()
{
}

HRESULT CQuickSlot::Initialize()
{
	m_tInfo.vPos = { 0.f, WINCY - 101.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };

	m_strObjKey = L"Interface";
	m_strStateKey = L"QuickSlot";

	m_iFrame = 0;

	m_pKeyMgr = CKeyMgr::GetInstance();

	m_eObjType = CGameObject::UI;
	m_eGroupID = GroupID::EFFECT;

	CSkillIcon::Create(
		m_tInfo.vPos.x + 23.f,
		m_tInfo.vPos.y + 41.f,
		0);
	CSkillIcon::Create(
		m_tInfo.vPos.x + 53.f,
		m_tInfo.vPos.y + 41.f,
		1);

	return S_OK;
}

void CQuickSlot::Release()
{
}

void CQuickSlot::KeyCheck()
{
	if (m_pKeyMgr->KeyDown(KEY_TAB))
	{
		if (0 == m_iFrame)
			m_iFrame = 1;
		else if (1 == m_iFrame)
			m_iFrame = 0;
	}
}

HRESULT CQuickSlot::Create()
{
	CQuickSlot* pInstance = new CQuickSlot;

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return E_FAIL;
	}

	HRESULT hr = CObjectMgr::GetInstance()->AddObject(CGameObject::UI, pInstance);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	return S_OK;
}
