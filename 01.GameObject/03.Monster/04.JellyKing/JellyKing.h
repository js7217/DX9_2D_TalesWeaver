#pragma once
#include "Monster.h"
class CJellyKing :
	public CMonster
{
public:
	CJellyKing();
	virtual ~CJellyKing();

public:
	// CMonster을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void LineRender() override;
private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInit();
	virtual void Release() override;
private:
	virtual void MoveFrame(float fSpeed = 1.f) override;
	virtual void MotionChange() override;
	virtual void MakeWorldMatrix() override;
	virtual void Set_Dead() override;
	virtual void MoveMonster() override;

private:
	void CollisionRect(); // 보스몬스터의 추적
	void Jump(); // 공격과 이동 상태때 점프
	void UpdateRect_Attack(float fCX, float fCY); // 몬스터 공격용 렉트
	void IsShake(); // 흔들림.
public:
	static HRESULT Create(float fX, float fY);

private:
	bool m_bIsShake;
	float m_fDeadTimer;
	float m_fJump;
	int	m_iLoopCount;
	int m_iShakeNum;
	float m_fPreY;
	RECT m_tAttRect;
	D3DXVECTOR3 m_vScroll;
};

