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
	void Set_TargetSkillDamage(int iCount, int iDamage); // �ݺ�������
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
	bool			m_bIsHit; // �ǰݽ� �Ѵ븸 ��������(���Ÿ�)
	bool			m_bIsMonsterShake; // �¾����� ��鸲.
	bool			m_bIsTarget; // �ڽ��� Ÿ������ �ƴ���.
	bool			m_bIsDead; // �׾����� ��Ҵ���
	bool			m_bIsAtt; // ������ �ص� �Ǵ��� �ȵǴ���.
	bool			m_bIsDamage; // �������� �ѹ��� ���� �ϹǷ�..
	int				m_iDamageShake; // �ǰݽ� ��鸲 Ƚ��
	int				m_iDamage; // �÷��̾ ���� ������
	int				m_iExp; // ��������� ����ġ��
	float			m_fSpeed; // ���� �̵� ���ǵ�.
	float			m_fDist; // �̵� �Ÿ�.
	float			m_fAttTimer; // �����ϴ� ��Ÿ��.
	float			m_fPreAttTime; // ���ư� ��Ÿ�� ��.
	D3DXVECTOR3		m_vMonsterPos;
	UNIT_DATA		m_tData; // �÷��̾� ������.
	FRAME			m_tFrame;
	RECT			m_tTrackingRect; // ������ ��Ʈ
	STATE			m_eCurState;
	STATE			m_ePreState;
	CGameObject*	m_pTarget; // ���Ͱ� �Ѿư� Ÿ��.
	CGameObject*	m_pNameBar; // ���� �̸� �� UI
	CGameObject*	m_pTargetUI; // ���� Ÿ�� UI
	CGameObject*	m_pHpBar; // ���� Hp Bar
};

