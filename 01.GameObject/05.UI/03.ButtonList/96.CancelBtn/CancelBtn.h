#pragma once
#include "GameObject.h"
class CCancelBtn :
	public CGameObject
{
private:
	explicit CCancelBtn();
	explicit CCancelBtn(const D3DXVECTOR3& vPos);
public:
	virtual ~CCancelBtn();
public:
	const bool& Get_Click() { return m_bIsClick; }
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
	static CCancelBtn* Create(const D3DXVECTOR3& vPos);

private:
	bool m_bIsClick;
	int m_iFrame;
};

