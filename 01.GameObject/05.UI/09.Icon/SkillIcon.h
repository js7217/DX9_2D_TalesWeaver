#pragma once
#include "GameObject.h"
class CSkillIcon :
	public CGameObject
{
public:
	CSkillIcon();
	virtual ~CSkillIcon();

public:
	void Set_Frame(const int& iFrame) { m_iFrame = iFrame; }

public:
	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void LineRender() override;
private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInit();
	virtual void Release() override;

public:
	static HRESULT Create(float fX, float fY, const int& iFrame);

private:
	int m_iFrame;
	D3DXVECTOR3 m_vPrePos;
};

