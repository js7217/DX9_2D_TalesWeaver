#pragma once
#include "GameObject.h"
class CChatUI :
	public CGameObject
{
private:
	explicit CChatUI();
public:
	virtual ~CChatUI();

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
	static HRESULT Create();
};

