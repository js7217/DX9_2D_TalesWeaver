#include "stdafx.h"
#include "ChatUI.h"


CChatUI::CChatUI()
{
}


CChatUI::~CChatUI()
{
}

int CChatUI::Update()
{
	return NO_EVENT;
}

void CChatUI::LateUpdate()
{
}

void CChatUI::Render()
{
	D3DXMatrixTranslation(&m_tInfo.matWorld, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_strObjKey,
		0,
		m_strStateKey);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CChatUI::LineRender()
{
}

HRESULT CChatUI::Initialize()
{
	m_tInfo.vPos = { WINCX * 0.5f, WINCY - 50.5f, 0.f };

	m_strObjKey = L"Interface";
	m_strStateKey = L"ChatWnd";

	m_eObjType = CGameObject::UI;
	m_eGroupID = GroupID::UI;

	return S_OK;
}

void CChatUI::Release()
{
}

HRESULT CChatUI::Create()
{
	CChatUI* pInstance = new CChatUI;

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return E_FAIL;
	}

	HRESULT hr = CObjectMgr::GetInstance()->AddObject(CGameObject::UI, pInstance);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	return S_OK;
}
