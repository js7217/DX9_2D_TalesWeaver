#pragma once
#include "GameObject.h"
class CInvenBtn :
	public CGameObject
{
public:
	CInvenBtn();
	virtual ~CInvenBtn();

public:
	CGameObject* Get_Inven() { return m_pInven; }

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
	static CInvenBtn* Create();

private:
	bool			m_bIsRender;
	bool			m_bIsClick;
	CKeyMgr*		m_pKeyMgr;
	CGameObject*	m_pInven;
	int m_iFrame;
};

