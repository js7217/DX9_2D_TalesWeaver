#pragma once
#include "GameObject.h"
class CTargetUI :
	public CGameObject
{
private:
	explicit CTargetUI();
	explicit CTargetUI(CGameObject* pTarget);
public:
	virtual ~CTargetUI();


public:
	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void LineRender() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

private:
	void MoveFrame();

public:
	static CTargetUI* Create(CGameObject* pTarget);

private:
	CGameObject*	m_pTarget;
	FRAME			m_tFrame;
	float			m_fStartFrame;
};

