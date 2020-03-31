#pragma once
#include "GameObject.h"
class CQuickSlot :
	public CGameObject
{
public:
	CQuickSlot();
	virtual ~CQuickSlot();

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
	void KeyCheck();

public:
	static HRESULT Create();

private:
	int				m_iFrame;
	CKeyMgr*		m_pKeyMgr;
};

