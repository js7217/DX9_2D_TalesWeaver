#include "stdafx.h"
#include "Minimap.h"


CMinimap::CMinimap()
{
}


CMinimap::~CMinimap()
{
}

int CMinimap::Update()
{
	return 0;
}

void CMinimap::LateUpdate()
{
	// 미니맵 이미지 바꾸기.
}

void CMinimap::Render()
{
	D3DXMatrixTranslation(&m_tInfo.matWorld, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	{
		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
			m_strObjKey,
			0,
			m_strStateKey);

		NULL_CHECK(pTexInfo);

		m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	{
		D3DXMatrixTranslation(&m_tInfo.matWorld, m_tInfo.vPos.x, m_tInfo.vPos.y - 23.f, 0.f);
		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
			m_strObjKey,
			m_iFrame,
			m_strStateKey);

		NULL_CHECK(pTexInfo);

		m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

}

void CMinimap::LineRender()
{
}

HRESULT CMinimap::Initialize()
{
	m_tInfo.fCX = 132.f;
	m_tInfo.fCY = 100.f;
	m_tInfo.vPos = { WINCX - m_tInfo.fCX, WINCY - m_tInfo.fCY, 0.f };

	m_strObjKey = L"Interface";
	m_strStateKey = L"Minimap";

	m_iFrame = 1;

	m_eObjType = CGameObject::UI;
	m_eGroupID = GroupID::UI;

	return S_OK;
}

void CMinimap::Release()
{
}

HRESULT CMinimap::Create()
{
	CMinimap* pInstance = new CMinimap;

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return E_FAIL;
	}

	HRESULT hr = CObjectMgr::GetInstance()->AddObject(CGameObject::UI, pInstance);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	return S_OK;
}
