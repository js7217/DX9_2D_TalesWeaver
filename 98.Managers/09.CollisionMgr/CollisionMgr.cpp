#include "stdafx.h"
#include "CollisionMgr.h"
#include "GameObject.h"
#include "Mouse.h"
#include "Player.h"
#include "Monster.h"
#include "SilpidLance.h"
CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::CollisionRectMouse(OBJLST & rDst, OBJLST & rSrc) // 마우스 vs 몬스터
{
	RECT rc = {};
	CGameObject* pPlayer = CObjectMgr::GetInstance()->GetObjectW(CGameObject::PLAYER); // 플레이어 받아옴,
	CGameObject* pTarget = nullptr;
	CGameObject* pSkillTarget = nullptr;

	for (auto& rDestObj : rDst)
	{
		if (rSrc.empty())
		{
			dynamic_cast<CMouse*>(rDestObj)->Set_MouseState(CMouse::NORMAL);
			dynamic_cast<CPlayer*>(pPlayer)->Set_Target(nullptr);
			dynamic_cast<CPlayer*>(pPlayer)->Set_SkillTarget(nullptr);
		}
		for (auto& rSourceObj : rSrc)
		{
			if (IntersectRect(&rc, &rDestObj->Get_Rect(), &rSourceObj->Get_Rect())) // 마우스와 몬스터 렉트가 겹칠때
			{
				dynamic_cast<CMouse*>(rDestObj)->Set_MouseState(CMouse::BATTLE); // 배틀 마우스로 바꿈
				pTarget = rSourceObj;
				pSkillTarget = rSourceObj;
				if(dynamic_cast<CMonster*>(pTarget)->Get_State() == CMonster::DIE)
					pTarget = nullptr;
				else
					dynamic_cast<CMonster*>(rSourceObj)->Set_Target(true);

				if (dynamic_cast<CMonster*>(pSkillTarget)->Get_State() == CMonster::DIE)
					pSkillTarget = nullptr;
				else
					dynamic_cast<CMonster*>(rSourceObj)->Set_Target(true);

				// 타겟이 선택되었다고 몬스터에게 알려준다.
				break;// 타겟도 준다.
			}
			else
			{
				dynamic_cast<CMouse*>(rDestObj)->Set_MouseState(CMouse::NORMAL); // 배틀 마우스로 바꿈
				dynamic_cast<CMonster*>(rSourceObj)->Set_Target(false);
				pTarget = nullptr;
				pSkillTarget = nullptr;
			}
		}
		if (CKeyMgr::GetInstance()->KeyDown(KEY_LBUTTON))
		{
			dynamic_cast<CPlayer*>(pPlayer)->Set_Target(pTarget); // 플레이어에게 해당 타겟을 준다. 타겟이 있건 없건
		}

		if (CKeyMgr::GetInstance()->KeyDown(KEY_RBUTTON))
		{
			dynamic_cast<CPlayer*>(pPlayer)->Set_SkillTarget(pSkillTarget);
		}
	}
}

void CCollisionMgr::CollisionRectSkill(OBJLST & rDst, OBJLST & rSrc)
{
	RECT rc = {};
	int iDamage = 0;
	for (auto& pBullet : rDst)
	{
		if (CGameObject::PLAYER != pBullet->Get_ObjType())
			continue; // 플레이어 불릿이 아니면 돌아가라
		for (auto& pMonster : rSrc)
		{
			if (IntersectRect(&rc, &pBullet->Get_Rect(), &pMonster->Get_Rect()))
			{
				if (dynamic_cast<CMonster*>(pMonster)->Get_IsHit()) // 이미 충돌했더라면
					continue;

				iDamage = dynamic_cast<CSilpidLance*>(pBullet)->Get_Att(); // 불릿 데미지
				
				dynamic_cast<CMonster*>(pMonster)->Set_State(CMonster::DAMAGE);
				dynamic_cast<CMonster*>(pMonster)->Set_Damage(iDamage);
			}
		}
	}
}

bool CCollisionMgr::CheckRect(CGameObject * pDst, CGameObject * pSrc, float * pMoveX, float * pMoveY)
{
	float fRadSumX = (pDst->Get_Info().fCX + pSrc->Get_Info().fCX) * 0.5f;
	float fRadSumY = (pDst->Get_Info().fCY + pSrc->Get_Info().fCY) * 0.5f;

	float fDistX = fabs(pDst->Get_Info().vPos.x - pSrc->Get_Info().vPos.x);
	float fDistY = fabs(pDst->Get_Info().vPos.y - pSrc->Get_Info().vPos.y);

	if (fRadSumX >= fDistX && fRadSumY >= fDistY)
	{
		*pMoveX = fRadSumX - fDistX;
		*pMoveY = fRadSumY - fDistY;
		return true;
	}
	return false;
}
