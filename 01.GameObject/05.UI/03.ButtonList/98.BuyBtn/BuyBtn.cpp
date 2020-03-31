#include "stdafx.h"
#include "BuyBtn.h"


CBuyBtn::CBuyBtn()
{
}

CBuyBtn::CBuyBtn(const D3DXVECTOR3 & vPos)
	: m_iFrame(0)
{
	m_tInfo.vPos.x = vPos.x - 2.f;
	m_tInfo.vPos.y = vPos.y + 25.f;
}


CBuyBtn::~CBuyBtn()
{
}


int CBuyBtn::Update()
{
	CGameObject::UpdateRect(m_tInfo.fCX * 0.5f, m_tInfo.fCY * 0.5f);

	return NO_EVENT;
}

void CBuyBtn::LateUpdate()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tRect, pt))
	{
		if (CKeyMgr::GetInstance()->KeyPressing(KEY_LBUTTON))
		{
			m_iFrame = 1;
			m_bIsClick = true;
		}
		if (CKeyMgr::GetInstance()->KeyUp(KEY_LBUTTON))
		{
			m_iFrame = 0;
		}
	}
	else
		m_bIsClick = false;
}

void CBuyBtn::Render()
{
	D3DXMatrixTranslation(&m_tInfo.matWorld, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_strObjKey,
		m_iFrame,
		m_strStateKey);

	NULL_CHECK(pTexInfo);

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CBuyBtn::LineRender()
{
}

HRESULT CBuyBtn::Initialize()
{
	m_strObjKey = L"Interface";
	m_strStateKey = L"BuyButton";

	m_tInfo.fCX = 21.f;
	m_tInfo.fCY = 70.f;

	m_eObjType = CGameObject::UI;
	return S_OK;
}

void CBuyBtn::Release()
{
}

CBuyBtn * CBuyBtn::Create(const D3DXVECTOR3 & vPos)
{
	CBuyBtn* pInstance = new CBuyBtn(vPos);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}

	return pInstance;
}
