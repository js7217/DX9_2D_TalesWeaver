#pragma once
#include "Monster.h"
class CSoracap :
	public CMonster
{
public:
	CSoracap();
	virtual ~CSoracap();

	// CMonster을(를) 통해 상속됨
public:
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

public:
	static HRESULT Create(float fX, float fY, UNIT_DATA tData);

private:
	float			m_fDeadTimer;
};

