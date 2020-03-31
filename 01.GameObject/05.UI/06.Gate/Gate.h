#pragma once
#include "GameObject.h"
class CGate :
	public CGameObject
{
private:
	explicit CGate();
public:
	virtual ~CGate();

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
	static HRESULT Create(float fX, float fy);

private:
	FRAME m_tFrame;
};

