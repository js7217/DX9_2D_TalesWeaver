#include "stdafx.h"
#include "Monster.h"
#include "ScrollMgr.h"
#include "Mouse.h"
#include "DmgEffect.h"
CMonster::CMonster()
	: m_iDamage(0)
	, m_bIsTarget(false)
	, m_bIsDead(false)
	, m_fSpeed(0.f)
	, m_fDist(0.f)
	, m_bIsAtt(false)
	, m_bIsDamage(false)
	, m_fAttTimer(1.f)
	, m_pNameBar(nullptr)
	, m_pHpBar(nullptr)
	, m_pTargetUI(nullptr)
	, m_iDamageShake(6)
	, m_bIsMonsterShake(false)
	, m_iExp(0)
	, m_bIsHit(false)
{
	m_eObjType = CGameObject::MONSTER;
	ZeroMemory(&m_tFrame, sizeof(FRAME));
	m_pTarget = CObjectMgr::GetInstance()->GetObject(CGameObject::PLAYER); // 플레이어를 받는다.
	m_fPreAttTime = m_fAttTimer;
}


CMonster::~CMonster()
{
}

void CMonster::Set_Damage(int iDamage)
{
	CSoundMgr::GetInstance()->StopSound(CSoundMgr::MONSTER_HIT);
	m_iDamage = iDamage;
	CSoundMgr::GetInstance()->MyPlaySound(L"Hit.mp3", CSoundMgr::MONSTER_HIT);
}

void CMonster::Set_TargetSkillDamage(int iCount, int iDamage)
{
	m_eCurState = CMonster::DAMAGE;
	for (int i = 0; i < iCount; ++i)
	{
		CSoundMgr::GetInstance()->StopSound(CSoundMgr::MONSTER_HIT);
		m_tData.iHp -= iDamage;
		CDmgEffect::Create(m_tInfo.vPos, L"DmgFont", iDamage);
		CSoundMgr::GetInstance()->MyPlaySound(L"Hit.mp3", CSoundMgr::MONSTER_HIT);
	}
}

void CMonster::UpdateRect_Tracking(const float & fCX, const float & fCY, const float & fWidth, const float & fHeight)
{
	m_tTrackingRect.left = LONG(m_tInfo.vPos.x - (fCX / 2.f) + fWidth) - CScrollMgr::GetScroll(0);
	m_tTrackingRect.top = LONG(m_tInfo.vPos.y - (fCY / 2.f) + fHeight) - CScrollMgr::GetScroll(1);
	m_tTrackingRect.right = LONG(m_tInfo.vPos.x + (fCX / 2.f) + fWidth) - CScrollMgr::GetScroll(0);
	m_tTrackingRect.bottom = LONG(m_tInfo.vPos.y + (fCY / 2.f) + fHeight) - CScrollMgr::GetScroll(1);
}

void CMonster::CollisionPlayer()
{
	RECT rc = {};

	if (IntersectRect(&rc, &m_tRect, &m_pTarget->Get_Rect())) // 실제 부딪혔을때 해당 시간 초 뒤에 공격모드로 바꿈.
	{
		m_fAttTimer -= m_pTimeMgr->GetDeltaTime();
		if (m_fAttTimer <= 0)
		{
			m_eCurState = CMonster::ATTACK;
			m_fAttTimer = 3.f;
		}
		m_bIsAtt = true;
	}
	else if (IntersectRect(&rc, &m_tTrackingRect, &m_pTarget->Get_Rect())) // 추적용 렉트충돌
	{
		m_tInfo.vDir = m_pTarget->Get_Info().vPos - m_tInfo.vPos;
		m_fDist = D3DXVec3Length(&m_tInfo.vDir);
		D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

		MonsterRotation(m_pTarget->Get_Info().vPos);
		m_eCurState = CMonster::WALK;
	}
	else
		m_bIsAtt = false;

}

void CMonster::ShakeMonster()
{
	if (m_bIsMonsterShake)
	{
		if (m_iDamageShake > 0 && m_iDamageShake % 2 == 0)
		{
			m_tInfo.vPos.x += 5.f;
			m_tInfo.vPos.y -= 5.f;
			--m_iDamageShake;
		}
		else if (m_iDamageShake > 0 && m_iDamageShake % 2 != 0)
		{
			m_tInfo.vPos = m_vMonsterPos;
			--m_iDamageShake;
		}

		if (m_iDamageShake <= 0)
			m_bIsMonsterShake = false;
	}
	else
	{
		m_iDamageShake = 6;
		m_vMonsterPos = m_tInfo.vPos;
	}
}

void CMonster::MonsterRotation(const D3DXVECTOR3& vPos)
{
	if (vPos.y >= m_tInfo.vPos.y)
		m_strDirectionKey = L"LDOWN";
	else
		m_strDirectionKey = L"LUP";

	if (vPos.x >= m_tInfo.vPos.x) // 오른쪽
		m_tInfo.vSize.x = -1.f;
	else
		m_tInfo.vSize.x = 1.f;
}
