#include "stdafx.h"
#include "OKBtn.h"


COKBtn::COKBtn()
{
}

COKBtn::COKBtn(const D3DXVECTOR3 & vPos)
{
	m_tInfo.vPos.x = vPos.x + 120.5f;
	m_tInfo.vPos.y = vPos.y + 179.5f;
	m_tInfo.vPos.z = 0.f;
}


COKBtn::~COKBtn()
{
}

int COKBtn::Update()
{
	CGameObject::UpdateRect();

	return NO_EVENT;
}

void COKBtn::LateUpdate()
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

void COKBtn::Render()
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

void COKBtn::LineRender()
{
}

HRESULT COKBtn::Initialize()
{
	m_strObjKey = L"Interface";
	m_strStateKey = L"OKButton";

	m_tInfo.fCX = 32.f;
	m_tInfo.fCY = 14.f;

	m_iFrame = 0;

	return S_OK;
}

void COKBtn::Release()
{
}

COKBtn * COKBtn::Create(const D3DXVECTOR3 & vPos)
{
	COKBtn* pInstance = new COKBtn(vPos);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}

	return pInstance;
}
