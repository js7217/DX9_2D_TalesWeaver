#pragma once
#include "Scene.h"
class CShop :
	public CScene
{
public:
	CShop();
	virtual ~CShop();

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
	static CShop* Create();
};

