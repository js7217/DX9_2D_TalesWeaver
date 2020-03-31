#pragma once
#include "GameObject.h"
class CMonster :
	public CGameObject
{
public:
	CMonster();
	virtual ~CMonster();
public:
	enum STATE { STAND, WALK, ATTACK, DAMAGE, DIE };

public:
	void Set_State(STATE eState)
	{ 
		if(STATE::DIE != m_eCurState || STATE::ATTACK != m_eCurState) 
			m_eCurState = eState; 
	}
	void Set_Damage(int iDamage);
	void Set_TargetSkillDamage(int iCount, int iDamage); // 반복데미지
	void Set_Target(const bool& bIsTarget) { m_bIsTarget = bIsTarget; }
	void Set_Data(UNIT_DATA tData) { m_tData = tData; }
public:
	const RECT& Get_TrackingRect() { return m_tTrackingRect; }
	const STATE& Get_State() { return m_eCurState; }
	const UNIT_DATA& Get_Data() { return m_tData; }
	const bool& Get_IsHit() { return m_bIsHit; }
protected:
	virtual void MoveFrame(float fSpeed = 1.f) PURE;
	virtual void MotionChange() PURE;
	virtual void MakeWorldMatrix() PURE;
	virtual void Set_Dead() PURE;
	virtual void MoveMonster() PURE;
protected:
	void UpdateRect_Tracking(const float& fCX, const float& fCY, const float& fWidth = 0.f, const float& fHeight = 0.f);
	void CollisionPlayer();
	void ShakeMonster();
private:
	void MonsterRotation(const D3DXVECTOR3& vPos);
protected:
	bool			m_bIsHit; // 피격시 한대만 맞을것임(원거리)
	bool			m_bIsMonsterShake; // 맞았을때 흔들림.
	bool			m_bIsTarget; // 자신이 타겟인지 아닌지.
	bool			m_bIsDead; // 죽었는지 살았는지
	bool			m_bIsAtt; // 공격을 해도 되는지 안되는지.
	bool			m_bIsDamage; // 데미지는 한번만 들어가야 하므로..
	int				m_iDamageShake; // 피격시 흔들림 횟수
	int				m_iDamage; // 플레이어가 때린 데미지
	int				m_iExp; // 잡았을때의 경험치량
	float			m_fSpeed; // 몬스터 이동 스피드.
	float			m_fDist; // 이동 거리.
	float			m_fAttTimer; // 공격하는 쿨타임.
	float			m_fPreAttTime; // 돌아갈 쿨타임 값.
	D3DXVECTOR3		m_vMonsterPos;
	UNIT_DATA		m_tData; // 플레이어 데이터.
	FRAME			m_tFrame;
	RECT			m_tTrackingRect; // 추적용 렉트
	STATE			m_eCurState;
	STATE			m_ePreState;
	CGameObject*	m_pTarget; // 몬스터가 쫓아갈 타겟.
	CGameObject*	m_pNameBar; // 몬스터 이름 바 UI
	CGameObject*	m_pTargetUI; // 몬스터 타겟 UI
	CGameObject*	m_pHpBar; // 몬스터 Hp Bar
};

