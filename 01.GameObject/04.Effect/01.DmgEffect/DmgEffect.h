#pragma once
#include "GameObject.h"
class CDmgEffect :
	public CGameObject
{
private:
	explicit CDmgEffect();
	explicit CDmgEffect(const D3DXVECTOR3& vPos, const wstring& wstrStateKey, const int& iDmg);
public:
	virtual ~CDmgEffect();

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
	static HRESULT Create(const D3DXVECTOR3& vPos, const wstring& wstrStateKey, const int& iDmg);

private:
	int m_iDmg;
	float m_fSpeed;
	float m_fTimer;
	float m_fJumpAccel;
};

