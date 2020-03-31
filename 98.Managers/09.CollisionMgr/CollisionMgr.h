#pragma once

class CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();
public:
	static void CollisionRectMouse(OBJLST& rDst, OBJLST& rSrc); // ���콺 <-> ����
	static void CollisionRectSkill(OBJLST& rDst, OBJLST& rSrc);
private:
	static bool CheckRect(CGameObject* pDst, CGameObject* pSrc, float* pMoveX, float* pMoveY);
};

