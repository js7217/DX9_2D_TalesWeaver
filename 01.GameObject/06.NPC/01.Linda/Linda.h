#pragma once
#include "GameObject.h"
class CLinda :
	public CGameObject
{
private:
	explicit CLinda();
public:
	virtual ~CLinda();
public:
	void Set_Target(const bool& bIsTarget) { m_bIsTarget = bIsTarget; }

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
	static HRESULT Create(float fX, float fY);

private:
	bool			m_bIsRender;
	bool			m_bIsClick;
	bool		m_bIsTarget;
	CKeyMgr*	m_pKeyMgr;
	FRAME		m_tFrame;
	CGameObject*	m_pShop;
	CGameObject*	m_pNameBar;
	CGameObject*	m_pTargetUI;
};

