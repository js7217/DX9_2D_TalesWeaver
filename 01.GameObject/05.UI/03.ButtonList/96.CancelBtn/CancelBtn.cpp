#include "stdafx.h"
#include "CancelBtn.h"


CCancelBtn::CCancelBtn()
{
}

CCancelBtn::CCancelBtn(const D3DXVECTOR3 & vPos)
{
	m_tInfo.vPos.x = vPos.x + 159.5f;
	m_tInfo.vPos.y = vPos.y + 179.5f;
	m_tInfo.vPos.z = 0.f;
}


CCancelBtn::~CCancelBtn()
{
}

int CCancelBtn::Update()
{
	CGameObject::UpdateRect();

	return NO_EVENT;
}

void CCancelBtn::LateUpdate()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tRect, pt))
	{
		if (CKeyMgr::GetInstance()->KeyPressing(KEY_LBUTTON))
		{
			m_iFrame = 1;
		}
		if (CKeyMgr::GetInstance()->KeyUp(KEY_LBUTTON))
		{
			m_iFrame = 0;
			m_bIsClick = true;
		}
	}
	else
		m_bIsClick = false;
}

void CCancelBtn::Render()
{
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

void CCancelBtn::LineRender()
{
}

HRESULT CCancelBtn::Initialize()
{
	m_strObjKey = L"Interface";
	m_strStateKey = L"CancelButton";

	m_tInfo.fCX = 32.f;
	m_tInfo.fCY = 14.f;

	m_iFrame = 0;

	return S_OK;
}

void CCancelBtn::Release()
{
}

CCancelBtn * CCancelBtn::Create(const D3DXVECTOR3 & vPos)
{
	CCancelBtn* pInstance = new CCancelBtn(vPos);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}

	return pInstance;
}
