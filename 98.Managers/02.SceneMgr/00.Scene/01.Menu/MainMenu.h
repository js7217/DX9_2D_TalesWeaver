#pragma once
#include "Scene.h"
class CMainMenu :
	public CScene
{
public:
	CMainMenu();
	virtual ~CMainMenu();

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
	static CMainMenu* Create();
};

