#include "stdafx.h"
#include "JellyKing.h"

#include "ScrollMgr.h"
#include "Player.h"
#include "Effect.h"
#include "DmgEffect.h"
#include "MonsterNameBar.h"
#include "TargetUI.h"
#include "MonsterHpBar.h"
#include "AStar.h"
#include "Bullet.h"

CJellyKing::CJellyKing()
	: m_fDeadTimer(3.f)
	, m_fJump(0.f)
	, m_iLoopCount(3)
	, m_fPreY(0.f)
	, m_bIsShake(false)
	, m_iShakeNum(6)
{
	ZeroMemory(&m_tAttRect, sizeof(RECT));
}


CJellyKing::~CJellyKing()
{
	Release();
}

int CJellyKing::Update()
{
	CGameObject::LateInit();
	if (m_bIsDead)
	{
		dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->
			GetObjectW(CGameObject::PLAYER))->Set_Target(nullptr);
		return DEAD_OBJ;
	}
	CGameObject::UpdateRect_Scroll();
	CMonster::UpdateRect_Tracking(1000.f, 1000.f); // 추적렉트
	UpdateRect_Attack(500.f, 500.f); // 렉트

	MoveFrame();
	MotionChange();
	MoveMonster();
	Jump();
	IsShake();
	if (m_bIsTarget)
	{
		m_pNameBar->Update();
		m_pTargetUI->Update();
		m_pHpBar->Update();
	}

	return 0;
}

void CJellyKing::LateUpdate()
{
	CMonster::ShakeMonster();
	MakeWorldMatrix();
	Set_Dead();

	if (m_bIsTarget)
	{
		m_pNameBar->LateUpdate();
		m_pTargetUI->LateUpdate();
		m_pHpBar->LateUpdate();
	}
}

void CJellyKing::Render()
{
	const TEX_INFO* pTexShadow = m_pTextureMgr->GetTexInfo(
		L"Shadow",
		1);
	NULL_CHECK(pTexShadow);

	D3DXMATRIX matShadow;
	D3DXMatrixTranslation(&matShadow,
		m_tInfo.vPos.x - CScrollMgr::GetScroll(0),
		m_fPreY - CScrollMgr::GetScroll(1),
		0.f);

	float fCenterX_Shadow = pTexShadow->tImgInfo.Width * 0.5f;
	float fCenterY_Shadow = pTexShadow->tImgInfo.Height * 0.5f - m_tInfo.fCY * 0.4f;

	m_pGraphicDev->GetSprite()->SetTransform(&matShadow);

	m_pGraphicDev->GetSprite()->Draw(pTexShadow->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX_Shadow, fCenterY_Shadow, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255)); // 그림자

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_strObjKey,
		int(m_tFrame.fCurFrame),
		m_strStateKey);

	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_bIsTarget)
	{
		m_pNameBar->Render();
		m_pTargetUI->Render();
		m_pHpBar->Render();
	}
}

void CJellyKing::LineRender()
{
	CGameObject::UpdateRect_Scroll();
	UpdateRect_Attack(500.f, 500.f); // 렉트
	CMonster::UpdateRect_Tracking(1000.f, 1000.f);

	D3DXVECTOR2 vecRect[5] =
	{
		{ (float)m_tRect.left, (float)m_tRect.top },
		{ (float)m_tRect.left, (float)m_tRect.bottom },
		{ (float)m_tRect.right, (float)m_tRect.bottom },
		{ (float)m_tRect.right, (float)m_tRect.top },
		{ (float)m_tRect.left, (float)m_tRect.top }
	};

	D3DXVECTOR2 vecTracking[5] =
	{
		{ (float)m_tTrackingRect.left, (float)m_tTrackingRect.top },
		{ (float)m_tTrackingRect.left, (float)m_tTrackingRect.bottom },
		{ (float)m_tTrackingRect.right, (float)m_tTrackingRect.bottom },
		{ (float)m_tTrackingRect.right, (float)m_tTrackingRect.top },
		{ (float)m_tTrackingRect.left, (float)m_tTrackingRect.top }
	};

	D3DXVECTOR2 vecAttRect[5] =
	{
		{ (float)m_tAttRect.left, (float)m_tAttRect.top },
		{ (float)m_tAttRect.left, (float)m_tAttRect.bottom },
		{ (float)m_tAttRect.right, (float)m_tAttRect.bottom },
		{ (float)m_tAttRect.right, (float)m_tAttRect.top },
		{ (float)m_tAttRect.left, (float)m_tAttRect.top }
	};

	m_pGraphicDev->GetLine()->Draw(vecRect, 5, D3DCOLOR_ARGB(255, 0, 255, 0));
	m_pGraphicDev->GetLine()->Draw(vecTracking, 5, D3DCOLOR_ARGB(255, 255, 255, 0));
	m_pGraphicDev->GetLine()->Draw(vecAttRect, 5, D3DCOLOR_ARGB(255, 255, 0, 0));
}

HRESULT CJellyKing::Initialize()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 0.f, -1.f, 0.f };
	m_tInfo.vDir = { 0.f, -1.f, 0.f };
	m_tInfo.fCX = 150.f;
	m_tInfo.fCY = 130.f;

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 0.f;

	m_strObjKey = L"JellyKing";
	m_strStateKey = L"STAND";

	m_fSpeed = 30.f;
	m_iExp = 50.f;
	m_tData.strObjKey = L"JellyKing";
	m_tData.strName = L"젤리킹";
	m_tData.iAtt = 5;
	m_tData.iMaxHp = 5500;
	m_tData.iHp = 5000;
	m_tData.iMP = 0;

	m_eCurState = CMonster::STAND;
	m_ePreState = CMonster::STAND;

	return S_OK;
}

HRESULT CJellyKing::LateInit()
{
	m_tData.strName = L"젤리킹";

	m_pNameBar = CMonsterNameBar::Create(m_tData.strName, this); // 네임 바 만들어줌.
	m_pTargetUI = CTargetUI::Create(this);
	m_pHpBar = CMonsterHpBar::Create(this);


	return S_OK;
}

void CJellyKing::Release()
{
	SafeDelete(m_pNameBar);
	SafeDelete(m_pTargetUI);
	SafeDelete(m_pHpBar);
}

void CJellyKing::MoveFrame(float fSpeed)
{
	switch (m_eCurState)
	{
	case CMonster::STAND:
		m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDeltaTime();

		if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
			m_tFrame.fCurFrame = 0.f;
		break;
	case CMonster::WALK:
		m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDeltaTime();

		if (m_tFrame.fCurFrame > 6.f && m_iLoopCount > 0) //  프레임 반복
		{
			m_tFrame.fCurFrame = 4.f;
			m_iLoopCount--;
		}

		if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
		{
			m_tFrame.fCurFrame = 0.f;
			m_eCurState = CMonster::STAND;
			m_tInfo.vPos.y = m_fPreY;
			m_bIsShake = true;
		}
		break;
	case CMonster::ATTACK:
		m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDeltaTime();

		if (m_tFrame.fCurFrame > 7.f && m_iLoopCount > 0) //  프레임 반복
		{
			m_tFrame.fCurFrame = 4.f;
			m_iLoopCount--;
			CBullet::Create(m_pTarget, m_tInfo.vPos.x, m_tInfo.vPos.y);

		}
		if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
		{
			m_tFrame.fCurFrame = 0.f;
			m_eCurState = CMonster::STAND;
			m_tInfo.vPos.y = m_fPreY;
			m_bIsShake = true;
		}
		break;
	case CMonster::DAMAGE:
		m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDeltaTime() * 5.f;

		if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
		{
			m_tFrame.fCurFrame = 0.f;
			m_eCurState = CMonster::ATTACK;
			m_bIsHit = false;
		}
		break;
	case CMonster::DIE:
		m_fDeadTimer -= m_pTimeMgr->GetDeltaTime();
		if (m_fDeadTimer <= 0.f)
			m_bIsDead = true;
		break;
	default:
		break;
	}
}

void CJellyKing::MotionChange()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case CMonster::STAND:
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 0.f;
			m_strStateKey = L"STAND";
			m_bIsHit = false;
			break;
		case CMonster::WALK:
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 7.f;
			m_strStateKey = L"WALK";
			m_tInfo.vPos.y = m_fPreY;
			m_iLoopCount = 3;
			m_fJump = 0.f;
			break;
		case CMonster::ATTACK:
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 8.f;
			m_strStateKey = L"ATTACK";
			m_tInfo.vPos.y = m_fPreY;
			m_iLoopCount = 3;
			m_fJump = 0.f;
			break;
		case CMonster::DAMAGE:
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 1.f;
			m_strStateKey = L"DAMAGE";
			m_tData.iHp -= m_iDamage;
			CEffect::Create(m_tInfo.matWorld, { 0, 3 }, 5.f, L"MonsterHit");
			CDmgEffect::Create(m_tInfo.vPos,
				L"DmgFont", m_iDamage);
			m_bIsMonsterShake = true;
			m_bIsHit = true;
			break;
		case CMonster::DIE:
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 0.f;
			m_strStateKey = L"DIE";
			dynamic_cast<CPlayer*>(m_pTarget)->Set_Exp(m_iExp);
			dynamic_cast<CPlayer*>(m_pTarget)->Set_Target(nullptr);
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CJellyKing::MakeWorldMatrix()
{
	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x - CScrollMgr::GetScroll(0),
		m_tInfo.vPos.y - CScrollMgr::GetScroll(1),
		0.f);

	m_tInfo.matWorld = matScale * matTrans;
}

void CJellyKing::Set_Dead()
{
	if (m_tData.iHp <= 0)
	{
		m_eCurState = CMonster::DIE;
	}
}

void CJellyKing::MoveMonster()
{
	CollisionRect();

	switch (m_eCurState)
	{
	case CMonster::WALK:
	case CMonster::ATTACK:
		if (m_tFrame.fCurFrame >= 4.f)
		{
			if (m_fDist > 0)
			{
				m_fDist -= m_fSpeed * m_pTimeMgr->GetDeltaTime();
				m_tInfo.vPos += m_tInfo.vDir * m_fSpeed * m_pTimeMgr->GetDeltaTime();
			}
			else
			{
				m_tInfo.vPos -= m_tInfo.vDir * m_fDist;
				m_fDist = 0.f;
			}
		}
		break;
	default:
		break;
	}

	/*if (!m_bIsAtt)
	{
		switch (m_eCurState)
		{
		case CMonster::STAND:
		case CMonster::WALK:
			if (m_tFrame.fCurFrame >= 4.f)
			{
				if (m_fDist > 0)
				{
					m_fDist -= m_fSpeed * m_pTimeMgr->GetDeltaTime();
					m_tInfo.vPos += m_tInfo.vDir * m_fSpeed * m_pTimeMgr->GetDeltaTime();
				}
				else
				{
					m_tInfo.vPos -= m_tInfo.vDir * m_fDist;
					m_fDist = 0.f;
				}
			}
			break;
		default:
			break;
		}
	}*/
}

void CJellyKing::CollisionRect()
{
	RECT rc = {};

	if (IntersectRect(&rc, &m_tAttRect, &m_pTarget->Get_Rect()))
	{
		m_fAttTimer -= m_pTimeMgr->GetDeltaTime();
		if (m_fAttTimer <= 0)
		{
			m_eCurState = CMonster::ATTACK;
			m_fAttTimer = 3.f;
		}
		m_bIsAtt = true;
	}
	else if (IntersectRect(&rc, &m_tTrackingRect, &m_pTarget->Get_Rect()))
	{
		m_eCurState = CMonster::WALK;
		m_bIsAtt = false;
	}
	else
		m_bIsAtt = false;

	m_tInfo.vDir = m_pTarget->Get_Info().vPos - m_tInfo.vPos;
	m_fDist = D3DXVec3Length(&m_tInfo.vDir);
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

	if (m_pTarget->Get_Info().vPos.x >= m_tInfo.vPos.x)
		m_tInfo.vSize.x = -1.f;
	else
		m_tInfo.vSize.x = 1.f;
}

void CJellyKing::Jump()
{
	switch (m_eCurState)
	{
	case CMonster::WALK:
	case CMonster::ATTACK:
		if (m_tFrame.fCurFrame >= 4.f && m_tFrame.fCurFrame < m_tFrame.fMaxFrame)
		{
			m_fJump -= m_pTimeMgr->GetDeltaTime() * 10.f; //  점프??

			m_tInfo.vPos.y = m_fPreY - 100.f + m_fJump;
		}
		else if (m_tFrame.fCurFrame < 4.f)
			m_fPreY = m_tInfo.vPos.y;

		break;
	default:
		break;
	}
}

void CJellyKing::UpdateRect_Attack(float fCX, float fCY)
{
	m_tAttRect.left = LONG(m_tInfo.vPos.x - ((m_tInfo.fCX + fCX) / 2.f)) - CScrollMgr::GetScroll(0);
	m_tAttRect.top = LONG(m_tInfo.vPos.y - ((m_tInfo.fCY + fCY) / 2.f)) - CScrollMgr::GetScroll(1);
	m_tAttRect.right = LONG(m_tInfo.vPos.x + ((m_tInfo.fCX + fCX) / 2.f)) - CScrollMgr::GetScroll(0);
	m_tAttRect.bottom = LONG(m_tInfo.vPos.y + ((m_tInfo.fCY + fCY) / 2.f)) - CScrollMgr::GetScroll(1);
}

void CJellyKing::IsShake()
{
	if (m_bIsShake)
	{
		if (m_iShakeNum > 0 && m_iShakeNum % 2 == 0)
		{
			CScrollMgr::Set_Scroll(m_vScroll + D3DXVECTOR3(20.f, 20.f, 0.f));
			--m_iShakeNum;
		}
		else if (m_iShakeNum > 0 && m_iShakeNum % 2 != 0)
		{
			CScrollMgr::Set_Scroll(m_vScroll);
			--m_iShakeNum;
		}

		if (m_iShakeNum <= 0)
			m_bIsShake = false;
	}
	else
	{
		m_iShakeNum = 6;
		m_vScroll.x = CScrollMgr::GetScroll(0);
		m_vScroll.y = CScrollMgr::GetScroll(1);
		m_vScroll.z = 0.f;
	}
}

HRESULT CJellyKing::Create(float fX, float fY)
{
	CJellyKing* pInstance = new CJellyKing;

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return E_FAIL;
	}

	pInstance->Set_Pos(fX, fY);

	HRESULT hr = CObjectMgr::GetInstance()->AddObject(CGameObject::MONSTER, pInstance);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	return S_OK;
}
