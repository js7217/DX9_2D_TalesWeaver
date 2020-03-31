#include "stdafx.h"
#include "CloseBtn.h"


CCloseBtn::CCloseBtn()
{
}

CCloseBtn::CCloseBtn(const D3DXVECTOR3 & vPos)
	: m_pKeyMgr(CKeyMgr::GetInstance())
	, m_bIsClose(false)
{
	m_tInfo.vPos.x = vPos.x - 18.f;
	m_tInfo.vPos.y = vPos.y + 12.f;
}


CCloseBtn::~CCloseBtn()
{
}

int CCloseBtn::Update()
{
	CGameObject::UpdateRect();
	return NO_EVENT;
}

void CCloseBtn::LateUpdate()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tRect, pt))
	{
		if (m_pKeyMgr->KeyPressing(KEY_LBUTTON))
		{
			m_iFrame = 1;
		}
		if(m_pKeyMgr->KeyUp(KEY_LBUTTON))
		{
			m_iFrame = 0;
			m_bIsClose = true;
		}
	}
	else
	{
		m_iFrame = 0;
		m_bIsClose = false;
	}
}

void CCloseBtn::Render()
{
	CGameObject::UpdateRect();
	D3DXMatrixTranslation(&m_tInfo.matWorld, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_strObjKey,
		m_iFrame,
		m_strStateKey);

	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CCloseBtn::LineRender()
{
}

HRESULT CCloseBtn::Initialize()
{
	m_strObjKey = L"Interface";
	m_strStateKey = L"CloseButton";

	m_tInfo.fCX = 20.f;
	m_tInfo.fCY = 17.f;

	m_eObjType = CGameObject::UI;
	return S_OK;
}

void CCloseBtn::Release()
{
}

CCloseBtn* CCloseBtn::Create(const D3DXVECTOR3 & vPos)
{
	CCloseBtn* pInstance = new CCloseBtn(vPos);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}
