#include "stdafx.h"
#include "MenuButton.h"


CMenuButton::CMenuButton()
{
}

CMenuButton::CMenuButton(const wstring & wstrObjKey, const wstring & wstrStateKey, const D3DXVECTOR3 & vPos)
	: m_iFrame(0)
	, m_pKeyMgr(CKeyMgr::GetInstance())
{
	m_strObjKey = wstrObjKey;
	m_strStateKey = wstrStateKey;
	m_tInfo.vPos = vPos;
}


CMenuButton::~CMenuButton()
{
	Release();
}

int CMenuButton::Update()
{
	CGameObject::UpdateRect();
	return NO_EVENT;
}

void CMenuButton::LateUpdate()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tRect, pt))
	{
		m_iFrame = 1;
		
		if (m_pKeyMgr->KeyDown(KEY_LBUTTON))
		{
			CSoundMgr::GetInstance()->MyPlaySound(L"MenuSelect.wav", CSoundMgr::EFFECT);
			if (L"StartBtn" == m_strStateKey)
				CSceneMgr::GetInstance()->SceneChange(CScene::CHAPTER1);
			else if (L"ExitBtn" == m_strStateKey)
				DestroyWindow(g_hWnd);
			return;
		}
	}
	else
		m_iFrame = 0;
}

void CMenuButton::Render()
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

void CMenuButton::LineRender()
{
}

HRESULT CMenuButton::Initialize()
{
	m_tInfo.fCX = 115.f;
	m_tInfo.fCY = 24.f;

	m_eObjType = CGameObject::UI;
	m_eGroupID = GroupID::UI;

	return S_OK;
}

void CMenuButton::Release()
{
}

HRESULT CMenuButton::Create(const wstring & wstrObjKey, const wstring & wstrStateKey, const D3DXVECTOR3 & vPos)
{
	CMenuButton* pInstance = new CMenuButton(wstrObjKey, wstrStateKey, vPos);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return E_FAIL;
	}

	HRESULT hr = CObjectMgr::GetInstance()->AddObject(CGameObject::UI, pInstance);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	return S_OK;
}
