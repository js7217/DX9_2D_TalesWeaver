#pragma once

#include "FrameMgr.h"

class CMainGame
{
private:
	explicit CMainGame();
public:
	~CMainGame();

public:
	void Update();
	void LateUpdate();
	void Render(CFrameMgr& frameMgr);

private:
	HRESULT Initialize();
	void Release();

public:
	static CMainGame* Create();

private:
	CGraphicDev*	m_pGraphicDev;
	CTextureMgr*	m_pTextureMgr;
	CSceneMgr*		m_pSceneMgr;
	CObjectMgr*		m_pObjectMgr;
	CTimeMgr*		m_pTimeMgr;
	CKeyMgr*		m_pKeyMgr;
};

