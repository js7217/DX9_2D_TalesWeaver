#pragma once
#include "GameObject.h"
class CCloseBtn :
	public CGameObject
{
private:
	explicit CCloseBtn();
	explicit CCloseBtn(const D3DXVECTOR3& vPos);
public:
	virtual ~CCloseBtn();

public:
	const bool& Get_Click() { return m_bIsClose; }
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
	static CCloseBtn* Create(const D3DXVECTOR3& vPos);

private:
	bool m_bIsClose;
	int m_iFrame;
	CKeyMgr* m_pKeyMgr;
};

