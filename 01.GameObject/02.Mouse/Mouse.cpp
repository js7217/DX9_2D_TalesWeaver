#include "stdafx.h"
#include "Mouse.h"
#include "ScrollMgr.h"
#include "Player.h"

CMouse::CMouse()
	: m_pKeyMgr(CKeyMgr::GetInstance())
	, m_pTarget(nullptr)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}


CMouse::~CMouse()
{
	Release();
}

D3DXVECTOR3 CMouse::GetMousePos()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	return D3DXVECTOR3({ float(pt.x + CScrollMgr::GetScroll(0)), float(pt.y + CScrollMgr::GetScroll(1)), 0.f });
}

int CMouse::Update()
{
	CGameObject::UpdateRect(16.f, 16.f);
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	m_tInfo.vPos = { float(pt.x), float(pt.y), 0.f };

	return NO_EVENT;
}

void CMouse::LateUpdate()
{
	KeyCheck();
	MotionChange();
	MoveFrame();
	MakeWorldMatrix();
}

void CMouse::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_strObjKey,
		int(m_tFrame.fCurFrame),
		m_strStateKey);

	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CMouse::LineRender()
{
	CGameObject::UpdateRect(16.f, 16.f);

	D3DXVECTOR2 vec[5] =
	{
		{ (float)m_tRect.left, (float)m_tRect.top },
		{ (float)m_tRect.left, (float)m_tRect.bottom },
		{ (float)m_tRect.right, (float)m_tRect.bottom },
		{ (float)m_tRect.right, (float)m_tRect.top },
		{ (float)m_tRect.left , (float)m_tRect.top }
	};

	m_pGraphicDev->GetLine()->Draw(vec, 5, D3DCOLOR_ARGB(255, 255, 0, 0));
}

HRESULT CMouse::Initialize()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	m_eGroupID = GroupID::MOUSE;
	m_eObjType = CGameObject::MOUSE;

	m_tInfo.vPos = { float(pt.x), float(pt.y), 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 0.f, -1.f, 0.f };
	m_tInfo.vDir = { 0.f, -1.f, 0.f };
	m_tInfo.fCX = 32.f;
	m_tInfo.fCY = 32.f;

	D3DXMatrixIdentity(&m_tInfo.matWorld);

	m_strObjKey = L"Mouse";
	m_strStateKey = L"Normal";

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 12.f;

	m_eCurState = CMouse::NORMAL;
	m_ePreState = CMouse::NORMAL;

	return S_OK;
}

void CMouse::Release()
{
}

CMouse * CMouse::Create()
{
	CMouse* pInstance = new CMouse;

	if (pInstance->Initialize())
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}


void CMouse::MakeWorldMatrix()
{
	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x,
		m_tInfo.vPos.y,
		0.f);

	m_tInfo.matWorld = matScale * matTrans;
}

void CMouse::MotionChange()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case CMouse::BATTLE:
			m_strStateKey = L"Battle";
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 6.f;
			break;
		case CMouse::CHAT:
			m_strStateKey = L"Chat";
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 1.f;
			break;
		case CMouse::CLICK:
			m_strStateKey = L"Click";
			break;
		case CMouse::GATE:
			m_strStateKey = L"Gate";
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 3.f;
			break;
		case CMouse::MAGIC:
			m_strStateKey = L"Magic";
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 8.f;
			break;
		case CMouse::NOPASS:
			m_strStateKey = L"NoPass";
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 11.f;
			break;
		case CMouse::NORMAL:
			m_strStateKey = L"Normal";
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 12.f;
			break;
		case CMouse::PICK:
			m_strStateKey = L"Pick";
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 2.f;
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CMouse::MoveFrame(float fSpeed)
{
	m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDeltaTime() * fSpeed;

	if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
		m_tFrame.fCurFrame = 0.f;
}

void CMouse::KeyCheck()
{
	switch (m_eCurState)
	{
	case CMouse::BATTLE:
	case CMouse::CHAT:
		break;
	case CMouse::CLICK:
		if (m_pKeyMgr->KeyUp(KEY_LBUTTON))
		{
			m_eCurState = CMouse::NORMAL;
		}
		break;
	case CMouse::GATE:
		break;
	case CMouse::MAGIC:
		break;
	case CMouse::NOPASS:
		break;
	case CMouse::NORMAL:
		if (m_pKeyMgr->KeyPressing(KEY_LBUTTON))
		{
			m_eCurState = CMouse::CLICK;
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 0.f;
		}
		else if (m_pKeyMgr->KeyUp(KEY_LBUTTON))
		{
			m_tFrame.fCurFrame = 1.f;
			m_tFrame.fMaxFrame = 1.f;
		}
		else
			m_eCurState = CMouse::NORMAL;
		break;
	case CMouse::PICK:
		break;
	default:
		break;
	}
}