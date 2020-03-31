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
	// CGameObject��(��) ���� ��ӵ�
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

