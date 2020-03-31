#pragma once
#include "GameObject.h"
class CSellBtn :
	public CGameObject
{
private:
	explicit CSellBtn();
	explicit CSellBtn(const D3DXVECTOR3& vPos);
public:
	virtual ~CSellBtn();
public:
	const bool& Get_Click() { return m_bIsClick; }
	void Set_Click(const bool& bIsClick) { m_bIsClick = bIsClick; }
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
	static CSellBtn* Create(const D3DXVECTOR3& vPos);

private:
	bool m_bIsClick;
	int m_iFrame;
};

