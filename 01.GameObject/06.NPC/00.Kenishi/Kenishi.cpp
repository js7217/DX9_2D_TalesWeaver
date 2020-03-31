#include "stdafx.h"
#include "Kenishi.h"
#include "ScrollMgr.h"
#include "Mouse.h"
#include "MonsterNameBar.h"
#include "TargetUI.h"
#include "ShopWindow.h"
CKenishi::CKenishi()
	: m_pKeyMgr(CKeyMgr::GetInstance())
	, m_bIsTarget(false)
	, m_bIsClick(false)
	, m_bIsRender(false)
{
}


CKenishi::~CKenishi()
{
	Release();
}

int CKenishi::Update()
{
	CGameObject::UpdateRect_Scroll();
	MoveFrame();

	if (m_bIsTarget)
	{
		m_pNameBar->Update();
		m_pTargetUI->Update();
	}
	if(m_bIsRender)
		m_pShop->Update();

	if (dynamic_cast<CShopWindow*>(m_pShop)->Get_Close())
		m_bIsRender = false; // 만약 윈도우가 닫힌 상태라면 클릭 값을 false로 만들어준다.


	return NO_EVENT;
}

void CKenishi::LateUpdate()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tRect, pt))
	{
		m_bIsTarget = true;
		dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->GetObject(PLAYER))->Set_Move(false);
		dynamic_cast<CMouse*>(CObjectMgr::GetInstance()->GetObjectW(MOUSE))->Set_MouseState(CMouse::CHAT);
		if (m_pKeyMgr->KeyUp(KEY_LBUTTON))
		{
			dynamic_cast<CShopWindow*>(m_pShop)->Set_Open();
			m_bIsRender = true;
		}
	}
	else
		m_bIsTarget = false;

	if (m_bIsTarget)
	{
		m_pNameBar->LateUpdate();
		m_pTargetUI->LateUpdate();
	}
	if (m_bIsRender)
		m_pShop->LateUpdate();
}

void CKenishi::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_strObjKey,
		int(m_tFrame.fCurFrame),
		m_strStateKey);

	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	D3DXMatrixTranslation(&m_tInfo.matWorld, 
		m_tInfo.vPos.x - CScrollMgr::GetScroll(0), 
		m_tInfo.vPos.y - CScrollMgr::GetScroll(1),
		0.f);

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	if (m_bIsTarget)
	{
		m_pNameBar->Render();
		m_pTargetUI->Render();
	}

	if (m_bIsRender)
		m_pShop->Render();
}

void CKenishi::LineRender()
{
	CGameObject::UpdateRect_Scroll();
	D3DXVECTOR2 vecRect[5] =
	{
		{ (float)m_tRect.left, (float)m_tRect.top },
		{ (float)m_tRect.left, (float)m_tRect.bottom },
		{ (float)m_tRect.right, (float)m_tRect.bottom },
		{ (float)m_tRect.right, (float)m_tRect.top },
		{ (float)m_tRect.left, (float)m_tRect.top }
	};

	m_pGraphicDev->GetLine()->Draw(vecRect, 5, D3DCOLOR_ARGB(255, 0, 255, 0));
}

HRESULT CKenishi::Initialize()
{
	m_strObjKey = L"NPC";
	m_strStateKey = L"Kenishi";
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 6.f;

	m_tInfo.fCX = 61.f;
	m_tInfo.fCY = 92.f;

	m_eObjType = CGameObject::NPC;
	m_eGroupID = GroupID::SHOP;

	m_pNameBar = CMonsterNameBar::Create(L"케니시", this); // 네임 바 만들어줌.
	m_pTargetUI = CTargetUI::Create(this);

	m_pShop = CShopWindow::Create(1);
	return S_OK;
}

void CKenishi::Release()
{
	SafeDelete(m_pNameBar);
	SafeDelete(m_pTargetUI);
	SafeDelete(m_pShop);
}

void CKenishi::MoveFrame()
{
	m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDeltaTime();

	if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
		m_tFrame.fCurFrame = 0.f;
}

HRESULT CKenishi::Create(float fX, float fY)
{
	CKenishi* pInstance = new CKenishi;

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return E_FAIL;
	}

	pInstance->Set_Pos(fX, fY);

	HRESULT hr = CObjectMgr::GetInstance()->AddObject(CGameObject::NPC, pInstance);
	FAILED_CHECK_RETURN(hr, E_FAIL);
	
	return S_OK;
}
