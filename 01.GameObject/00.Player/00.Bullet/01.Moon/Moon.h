#pragma once
#include "GameObject.h"
class CMoon :
	public CGameObject
{
private:
	explicit CMoon();
public:
	virtual ~CMoon();

public:
	void Set_Target(CGameObject* pTarget) { m_pTarget = pTarget; }
public:
	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void LineRender() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static HRESULT Create(CGameObject* pTarget);

private:
	bool m_bIsDead;
	CGameObject* m_pTarget;
	FRAME m_tFrame;
};

