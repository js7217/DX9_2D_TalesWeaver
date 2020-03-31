#include "stdafx.h"
#include "StatusBtn.h"
#include "Status.h"
#include "CloseBtn.h"
#include "Player.h"
CStatusBtn::CStatusBtn()
	: m_bIsClick(false)
	, m_bIsRender(false)
{
}


CStatusBtn::~CStatusBtn()
{
	Release();
}

int CStatusBtn::Update()
{

	CGameObject::UpdateRect(m_tInfo.fCX * 0.5f, m_tInfo.fCY * 0.5f);
	if (m_bIsRender)
		m_pStatusWindow->Update();

	if (dynamic_cast<CStatus*>(m_pStatusWindow)->Get_Close())
		m_bIsClick = false; // 만약 윈도우가 닫힌 상태라면 클릭 값을 false로 만들어준다.

	return NO_EVENT;
}

void CStatusBtn::LateUpdate()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tRect, pt))
	{
		m_iFrame = 1;
		m_bIsRender = true;
		dynamic_cast<CStatus*>(m_pStatusWindow)->Set_Open();
		dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->GetObject(PLAYER))->Set_Move(false);
		if (m_pKeyMgr->KeyUp(KEY_LBUTTON))
		{
			if (!m_bIsClick) // 해당 버튼을 클릭했다면 사라지지 말아야함.
				m_bIsClick = true;
			else if (m_bIsClick)
				m_bIsClick = false;
			// 해당 창을 켤지말지만 결정한다.
			CSoundMgr::GetInstance()->MyPlaySound(L"wopen.mp3", CSoundMgr::EFFECT);
			return;
		}
	}
	else
	{
		m_iFrame = 0;
		if (!m_bIsClick)
			m_bIsRender = false;
	}

	if(m_bIsRender)
		m_pStatusWindow->LateUpdate();
}

void CStatusBtn::Render()
{
	D3DXMatrixTranslation(&m_tInfo.matWorld, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	{
		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
			m_strObjKey,
			m_iFrame,
			m_strStateKey);

		NULL_CHECK(pTexInfo);

		m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if (m_bIsRender)
		m_pStatusWindow->Render();
}

void CStatusBtn::LineRender()
{
}

HRESULT CStatusBtn::Initialize()
{
	m_pKeyMgr = CKeyMgr::GetInstance();

	m_tInfo.fCX = 18.f;
	m_tInfo.fCY = 51.f;
	m_tInfo.vPos = { WINCX - m_tInfo.fCX * 1.5f, m_tInfo.fCY, 0.f };

	m_strObjKey = L"Interface";
	m_strStateKey = L"StatusButton";

	m_iFrame = 0;
	m_eObjType = CGameObject::UI;

	m_pStatusWindow = CStatus::Create(m_tInfo.vPos);

	return S_OK;
}

void CStatusBtn::Release()
{
	SafeDelete(m_pStatusWindow);
}

CStatusBtn * CStatusBtn::Create()
{
	CStatusBtn* pInstance = new CStatusBtn;

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}

	return pInstance;
}
