#include "stdafx.h"
#include "SceneMgr.h"
#include "Logo.h"
#include "MainMenu.h"
#include "Clad.h"
#include "Shop.h"
#include "Field1.h"
#include "Lydia.h"
#include "Chapter1.h"
#include "Chapter2.h"
#include "Chapter3.h"
#include "Chapter4.h"
#include "BossField.h"
IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr()
	: m_ePreScene(CScene::END)
	, m_eCurScene(CScene::END)
	, m_pScene(nullptr)
	, m_bInit(false)
{
}


CSceneMgr::~CSceneMgr()
{
	Release();
}

HRESULT CSceneMgr::SceneChange(CScene::SCENE_TYPE eType)
{
	m_eCurScene = eType;

	if (m_ePreScene != m_eCurScene)
	{
		SafeDelete(m_pScene);

		switch (m_eCurScene)
		{
		case CScene::LOGO:
			m_pScene = CLogo::Create();
			break;
		case CScene::MENU:
			m_pScene = CMainMenu::Create();
			break;
		case CScene::CLAD:
			m_pScene = CClad::Create();
			break;
		case CScene::FIELD1:
			m_pScene = CField1::Create();
			break;
		case CScene::BOSS:
			m_pScene = CBossField::Create();
			break;
		case CScene::LYDIA:
			m_pScene = CLydia::Create();
			break;
		case CScene::SHOP:
			m_pScene = CShop::Create();
			break;
		case CScene::CHAPTER1:
			m_pScene = CChapter1::Create();
			break;
		case CScene::CHAPTER2:
			m_pScene = CChapter2::Create();
			break;
		case CScene::CHAPTER3:
			m_pScene = CChapter3::Create();
			break;
		case CScene::CHAPTER4:
			m_pScene = CChapter4::Create();
			break;
		case CScene::END:
			break;
		default:
			break;
		}

		m_ePreScene = m_eCurScene;
	}

	NULL_CHECK_RETURN(m_pScene, E_FAIL);

	return S_OK;
}

void CSceneMgr::Update()
{
	NULL_CHECK(m_pScene);
	m_pScene->Update();
}

void CSceneMgr::LateUpdate()
{
	NULL_CHECK(m_pScene);
	m_pScene->LateUpdate();
}

void CSceneMgr::Render()
{
	NULL_CHECK(m_pScene);
	m_pScene->Render();
}

void CSceneMgr::LineRender()
{
	NULL_CHECK(m_pScene);
	m_pScene->LineRender();
}

void CSceneMgr::Release()
{
	SafeDelete(m_pScene);
}
