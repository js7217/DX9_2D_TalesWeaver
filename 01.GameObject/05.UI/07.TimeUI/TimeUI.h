#pragma once
#include "GameObject.h"
class CTimeUI :
	public CGameObject
{
private:
	explicit CTimeUI();
public:
	virtual ~CTimeUI();

public:
	void Set_Target(CGameObject* pTarget) { m_pTarget = pTarget; }
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
	static HRESULT Create(CGameObject* pTarget);
private:
	bool m_bIsNight;
	float m_fAlpha;
	float m_fTimer;
	float m_fChangeTime;
	CGameObject* m_pTarget;
};

