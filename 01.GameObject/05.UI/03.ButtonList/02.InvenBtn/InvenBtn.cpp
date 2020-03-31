#include "stdafx.h"
#include "InvenBtn.h"
#include "Inven.h"
#include "Player.h"
CInvenBtn::CInvenBtn()
	: m_bIsClick(false)
	, m_bIsRender(false)
{
}


CInvenBtn::~CInvenBtn()
{
	Release();
}

int CInvenBtn::Update()
{
	CGameObject::UpdateRect(m_tInfo.fCX * 0.5f, m_tInfo.fCY * 0.5f);
	
	m_pInven->Update(); // 인벤은 언제나 돌아야 처리가 깔끔하게된다...

	if (dynamic_cast<CInven*>(m_pInven)->Get_Close())
		m_bIsClick = false; // 만약 윈도우가 닫힌 상태라면 클릭 값을 false로 만들어준다.

	return NO_EVENT;
}

void CInvenBtn::LateUpdate()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tRect, pt))
	{
		m_iFrame = 1;
		m_bIsRender = true;
		dynamic_cast<CInven*>(m_pInven)->Set_Open(); // 버튼에 마우스가 있으면 일단열림
		dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->GetObject(PLAYER))->Set_Move(false);
		if (m_pKeyMgr->KeyUp(KEY_LBUTTON))
		{
			// 인벤토리 창 띄우기.
			if (!m_bIsClick) // 해당 버튼을 클릭했다면 사라지지 말아야함.
				m_bIsClick = true;
			else if (m_bIsClick)
				m_bIsClick = false;

			return;
		}
	}
	else
	{
		m_iFrame = 0;
		if (!m_bIsClick)
			m_bIsRender = false;
	}

	if (m_bIsRender)
		m_pInven->LateUpdate();
}

void CInvenBtn::Render()
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
		m_pInven->Render();
}

void CInvenBtn::LineRender()
{
	if (m_bIsRender)
		m_pInven->LineRender();
}

HRESULT CInvenBtn::Initialize()
{
	m_pKeyMgr = CKeyMgr::GetInstance();

	m_tInfo.fCX = 18.f;
	m_tInfo.fCY = 51.f;
	m_tInfo.vPos = { WINCX - m_tInfo.fCX * 1.5f, m_tInfo.fCY * 3.f, 0.f };

	m_strObjKey = L"Interface";
	m_strStateKey = L"InvenButton";

	m_iFrame = 0;

	m_pInven = CInven::Create(m_tInfo.vPos);

	m_eObjType = CGameObject::UI;
	return S_OK;
}

void CInvenBtn::Release()
{
	SafeDelete(m_pInven);
}

CInvenBtn * CInvenBtn::Create()
{
	CInvenBtn* pInstance = new CInvenBtn;

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}

	return pInstance;
}
