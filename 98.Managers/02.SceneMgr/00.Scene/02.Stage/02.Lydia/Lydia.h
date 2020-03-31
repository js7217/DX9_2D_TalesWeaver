#pragma once
#include "Scene.h"
class CLydia :
	public CScene
{
public:
	CLydia();
	virtual ~CLydia();

public:
	// CScene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void LineRender() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CLydia* Create();
};

