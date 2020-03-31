#pragma once
#include "GameObject.h"
class CComboEffect :
	public CGameObject
{
private:
	explicit CComboEffect();
	explicit CComboEffect(CGameObject* pTarget, const int& iCombo);
public:
	virtual ~CComboEffect();

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
	static HRESULT Create(CGameObject* pTarget, const int& iCombo);

private:
	int m_iCombo;
	CGameObject* m_pTarget;
};

