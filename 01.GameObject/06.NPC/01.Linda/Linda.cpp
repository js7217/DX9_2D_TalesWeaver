#include "stdafx.h"
#include "Linda.h"
#include "ScrollMgr.h"
#include "Mouse.h"
#include "MonsterNameBar.h"
#include "TargetUI.h"
#include "ShopWindow.h"

CLinda::CLinda()
	: m_pKeyMgr(CKeyMgr::GetInstance())
	, m_bIsTarget(false)
	, m_bIsClick(false)
	, m_bIsRender(false)
{
}


CLinda::~CLinda()
{
	Release();
}

int CLinda::Update()
{
	CGameObject::UpdateRect_Scroll();
	MoveFrame();

	if (m_bIsTarget)
	{
		m_pNameBar->Update();
		m_pTargetUI->Update();
	}
	if (m_bIsRender)
		m_pShop->Update();

	if (dynamic_cast<CShopWindow*>(m_pShop)->Get_Close())
		m_bIsRender = false; // 만약 윈도우가 닫힌 상태라면 클릭 값을 false로 만들어준다.


	return NO_EVENT;
}

void CLinda::LateUpdate()
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
			dynamic_cast<CShopWindow*>(m_pShop)->InitSellShop();
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

void CLinda::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_strObjKey,
		int(m_tFrame.fCurFrame),
		m_strStateKey);

	NULL_CHECK(pTexInfo);
	D3DXMATRIX matScale, matTrans;
	D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x - CScrollMgr::GetScroll(0),
		m_tInfo.vPos.y - CScrollMgr::GetScroll(1),
		0.f);

	m_tInfo.matWorld = matScale * matTrans;

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;


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

void CLinda::LineRender()
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

HRESULT CLinda::Initialize()
{
	m_strObjKey = L"NPC";
	m_strStateKey = L"Linda";

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 11.f;

	m_tInfo.fCX = 128.f;
	m_tInfo.fCY = 128.f;

	m_eObjType = CGameObject::NPC;
	m_eGroupID = GroupID::SHOP;

	m_pNameBar = CMonsterNameBar::Create(L"린다", this); // 네임 바 만들어줌.
	m_pTargetUI = CTargetUI::Create(this);

	m_pShop = CShopWindow::Create(2);

	return S_OK;
}

void CLinda::Release()
{
	SafeDelete(m_pNameBar);
	SafeDelete(m_pTargetUI);
	SafeDelete(m_pShop);
}

void CLinda::MoveFrame()
{
	m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDeltaTime();

	if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
		m_tFrame.fCurFrame = 0.f;
}

HRESULT CLinda::Create(float fX, float fY)
{
	CLinda* pInstance = new CLinda;

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
