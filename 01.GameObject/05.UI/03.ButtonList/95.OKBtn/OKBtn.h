#pragma once
#include "GameObject.h"
class COKBtn :
	public CGameObject
{
private:
	explicit COKBtn();
	explicit COKBtn(const D3DXVECTOR3& vPos);
public:
	virtual ~COKBtn();
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
	static COKBtn* Create(const D3DXVECTOR3& vPos);

private:
	bool m_bIsClick;
	int m_iFrame;
};

