#pragma once

#include "Scene.h"

class CSceneMgr
{
	DECLARE_SINGLETON(CSceneMgr)

private:
	CSceneMgr();
	~CSceneMgr();
public:
	CScene::SCENE_TYPE Get_CurScene() { return m_eCurScene; }

public:
	HRESULT SceneChange(CScene::SCENE_TYPE eType);
	void Update();
	void LateUpdate();
	void Render();
	void LineRender();

private:
	void Release();

private:
	bool				m_bInit;
	CScene::SCENE_TYPE	m_ePreScene;
	CScene::SCENE_TYPE	m_eCurScene;
	CScene*				m_pScene;
};

