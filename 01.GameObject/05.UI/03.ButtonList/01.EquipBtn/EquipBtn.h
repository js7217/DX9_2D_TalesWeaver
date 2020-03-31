#pragma once
#include "GameObject.h"
class CEquipBtn :
	public CGameObject
{
public:
	CEquipBtn();
	virtual ~CEquipBtn();

public:
	CGameObject* Get_Equip() { return m_pEquip; }
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
	static CEquipBtn* Create();

private:
	bool			m_bIsRender;
	bool			m_bIsClick;
	CKeyMgr*		m_pKeyMgr;
	CGameObject*	m_pEquip;
	int				m_iFrame;
};

