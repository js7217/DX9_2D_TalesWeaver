#pragma once
#include "Scene.h"
class CField1 :
	public CScene
{
public:
	CField1();
	virtual ~CField1();

public:
	// CScene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void LineRender() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	static CField1* Create();
};

