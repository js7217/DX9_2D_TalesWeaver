#include "stdafx.h"
#include "Scene.h"

bool CScene::m_bIsInit = false;

CScene::CScene()
	: m_pSceneMgr(CSceneMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pObjectMgr(CObjectMgr::GetInstance())
{
}


CScene::~CScene()
{
}

HRESULT CScene::LateInit()
{
	if (!m_bIsInit)
	{
		this->LateInit();
		m_bIsInit = true;
	}
	return S_OK;
}
