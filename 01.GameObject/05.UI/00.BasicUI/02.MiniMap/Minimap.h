#pragma once
#include "GameObject.h"
class CMinimap :
	public CGameObject
{
public:
	CMinimap();
	virtual ~CMinimap();

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

private:
	int m_iFrame;
};

