#pragma once
#include "Scene.h"
class CLogo :
	public CScene
{
public:
	CLogo();
	virtual ~CLogo();

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
	static CLogo* Create();
	static unsigned int __stdcall LoadingResource(void* pParam);

private:
	CRITICAL_SECTION	m_CriticalSec;
	HANDLE				m_hLoadingThread;
	CGameObject*		m_pLoadingBar;
};

