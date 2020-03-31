#pragma once
#include "GameObject.h"
class CLoadingBar :
	public CGameObject
{
private:
	explicit CLoadingBar();
public:
	virtual ~CLoadingBar();

public:
	void Set_BarGauge(const int& iCount) { m_iBar = iCount; }

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
	static CLoadingBar* Create(float fX, float fY);
private:
	int m_iCLoading;
	int m_iWidth;
	int m_iBar;
	int m_iMaxBar;
};

