#include "stdafx.h"
#include "Kappa.h"
#include "ScrollMgr.h"
#include "Player.h"
#include "Effect.h"
#include "MonsterNameBar.h"
#include "TargetUI.h"
#include "MonsterHpBar.h"
#include "DmgEffect.h"
CKappa::CKappa()
	: m_fDeadTimer(1.f)
{
}


CKappa::~CKappa()
{
	Release();
}

int CKappa::Update()
{
	CGameObject::LateInit();
	if (m_bIsDead)
	{
		dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->
			GetObjectW(CGameObject::PLAYER))->Set_Target(nullptr);
		return DEAD_OBJ;
	}

	CGameObject::UpdateRect_Scroll(); // 렉트
	CMonster::UpdateRect_Tracking(200.f, 200.f); // 추적렉트

	MoveFrame();
	MotionChange();
	MoveMonster();
	if (m_bIsTarget)
	{
		m_pNameBar->Update();
		m_pTargetUI->Update();
		m_pHpBar->Update();
	}
	return NO_EVENT;
}

void CKappa::LateUpdate()
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

void CKappa::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_strObjKey,
		int(m_tFrame.fCurFrame),
		m_strStateKey,
		m_strDirectionKey);

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

void CKappa::LineRender()
{
	CGameObject::UpdateRect_Scroll();
	CMonster::UpdateRect_Tracking(200.f, 200.f);

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

	m_pGraphicDev->GetLine()->Draw(vecRect, 5, D3DCOLOR_ARGB(255, 0, 255, 0));
	m_pGraphicDev->GetLine()->Draw(vecTracking, 5, D3DCOLOR_ARGB(255, 255, 255, 0));
}

HRESULT CKappa::Initialize()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 0.f, -1.f, 0.f };
	m_tInfo.vDir = { 0.f, -1.f, 0.f };
	m_tInfo.fCX = 49.f;
	m_tInfo.fCY = 66.f;

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 3.f;

	m_strObjKey = L"Kappa";
	m_strStateKey = L"STAND";
	m_strDirectionKey = L"LDOWN";

	m_fSpeed = 30.f;
	m_iExp = 10;
	m_fAttTimer = 0.1f;

	m_eCurState = CMonster::STAND;
	m_ePreState = CMonster::STAND;

	return S_OK;
}

HRESULT CKappa::LateInit()
{
	m_tData.strName = L"캇파";

	m_pNameBar = CMonsterNameBar::Create(m_tData.strName, this); // 네임 바 만들어줌.
	m_pTargetUI = CTargetUI::Create(this);
	m_pHpBar = CMonsterHpBar::Create(this);

	return S_OK;
}

void CKappa::Release()
{
	SafeDelete(m_pNameBar);
	SafeDelete(m_pTargetUI);
	SafeDelete(m_pHpBar);
}

void CKappa::MoveFrame(float fSpeed)
{
	switch (m_eCurState)
	{
	case CMonster::STAND:
	case CMonster::WALK:
		m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDeltaTime();

		if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
			m_tFrame.fCurFrame = 0.f;
		break;
	case CMonster::ATTACK:
		m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDeltaTime() * 5.f;

		if (m_tFrame.fCurFrame >= 1.f && m_tFrame.fCurFrame <= 2.f)
		{
			dynamic_cast<CPlayer*>(m_pTarget)->Set_State(CPlayer::DAMAGE);
			if (!m_bIsDamage)
			{
				dynamic_cast<CPlayer*>(m_pTarget)->Set_Damage(m_tData.iAtt);
				m_bIsDamage = true;
			}
		}

		if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
		{
			m_tFrame.fCurFrame = 0.f;
			m_eCurState = CMonster::STAND;
		}
		break;
	case CMonster::DAMAGE:
		m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDeltaTime() * 5.f;

		if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
		{
			m_tFrame.fCurFrame = 0.f;
			m_eCurState = CMonster::STAND;
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

void CKappa::MotionChange()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case CMonster::STAND:
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 3.f;
			m_strStateKey = L"STAND";
			break;
		case CMonster::WALK:
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 7.f;
			m_strStateKey = L"WALK";
			break;
		case CMonster::ATTACK:
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 3.f;
			m_bIsDamage = false;
			m_strStateKey = L"ATTACK";
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

void CKappa::MakeWorldMatrix()
{
	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x - CScrollMgr::GetScroll(0),
		m_tInfo.vPos.y - CScrollMgr::GetScroll(1),
		0.f);

	m_tInfo.matWorld = matScale * matTrans;
}

void CKappa::Set_Dead()
{
	if (m_tData.iHp <= 0)
	{
		// 흔들리는 느낌 넣어보기.
		m_eCurState = CMonster::DIE;
	}
}

void CKappa::MoveMonster()
{
	CMonster::CollisionPlayer(); // 추적했는지 안했는지부터 파악.

	if (!m_bIsAtt)
	{
		switch (m_eCurState)
		{
		case CMonster::STAND:
		case CMonster::WALK:
			if (m_fDist > 0)
			{
				m_fDist -= m_fSpeed * m_pTimeMgr->GetDeltaTime();
				m_tInfo.vPos += m_tInfo.vDir * m_fSpeed * m_pTimeMgr->GetDeltaTime();
			}
			else
			{
				m_tInfo.vPos -= m_tInfo.vDir * m_fDist;
				m_fDist = 0.f;
				m_eCurState = CMonster::STAND;
			}
			break;
		default:
			break;
		}
	}
}

HRESULT CKappa::Create(float fX, float fY, UNIT_DATA tData)
{
	CKappa* pInstance = new CKappa;

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return E_FAIL;
	}

	pInstance->Set_Pos(fX, fY);
	pInstance->Set_Data(tData);

	HRESULT hr = CObjectMgr::GetInstance()->AddObject(CGameObject::MONSTER, pInstance);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	return S_OK;
}
