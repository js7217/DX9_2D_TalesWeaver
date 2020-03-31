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

void CCollisionMgr::CollisionRectMouse(OBJLST & rDst, OBJLST & rSrc) // ���콺 vs ����
{
	RECT rc = {};
	CGameObject* pPlayer = CObjectMgr::GetInstance()->GetObjectW(CGameObject::PLAYER); // �÷��̾� �޾ƿ�,
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
			if (IntersectRect(&rc, &rDestObj->Get_Rect(), &rSourceObj->Get_Rect())) // ���콺�� ���� ��Ʈ�� ��ĥ��
			{
				dynamic_cast<CMouse*>(rDestObj)->Set_MouseState(CMouse::BATTLE); // ��Ʋ ���콺�� �ٲ�
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

				// Ÿ���� ���õǾ��ٰ� ���Ϳ��� �˷��ش�.
				break;// Ÿ�ٵ� �ش�.
			}
			else
			{
				dynamic_cast<CMouse*>(rDestObj)->Set_MouseState(CMouse::NORMAL); // ��Ʋ ���콺�� �ٲ�
				dynamic_cast<CMonster*>(rSourceObj)->Set_Target(false);
				pTarget = nullptr;
				pSkillTarget = nullptr;
			}
		}
		if (CKeyMgr::GetInstance()->KeyDown(KEY_LBUTTON))
		{
			dynamic_cast<CPlayer*>(pPlayer)->Set_Target(pTarget); // �÷��̾�� �ش� Ÿ���� �ش�. Ÿ���� �ְ� ����
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
			continue; // �÷��̾� �Ҹ��� �ƴϸ� ���ư���
		for (auto& pMonster : rSrc)
		{
			if (IntersectRect(&rc, &pBullet->Get_Rect(), &pMonster->Get_Rect()))
			{
				if (dynamic_cast<CMonster*>(pMonster)->Get_IsHit()) // �̹� �浹�ߴ����
					continue;

				iDamage = dynamic_cast<CSilpidLance*>(pBullet)->Get_Att(); // �Ҹ� ������
				
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
