#pragma once
#include "GameObject.h"

class CBtnLst :
	public CGameObject
{
public:
	CBtnLst();
	virtual ~CBtnLst();

public:
	CGameObject* Get_Equip();
	CGameObject* Get_Inven();

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void LineRender() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static HRESULT Create();

private:
	list<CGameObject*> m_BtnLst;
	CGameObject* m_pStateBtn;
	CGameObject* m_pEquipBtn;
	CGameObject* m_pInvenBtn;
};

