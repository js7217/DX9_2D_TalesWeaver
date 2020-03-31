#pragma once
#include "GameObject.h"
class CStatusBtn :
	public CGameObject
{
public:
	CStatusBtn();
	virtual ~CStatusBtn();


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
	static CStatusBtn* Create();

private:
	bool			m_bIsClick;
	bool			m_bIsRender;
	int				m_iFrame;
	CKeyMgr*		m_pKeyMgr;
	CGameObject*	m_pStatusWindow;
};

