#include "stdafx.h"
#include "MainGame.h"
#include "Mouse.h"

CMainGame::CMainGame()
	: m_pGraphicDev(CGraphicDev::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pSceneMgr(CSceneMgr::GetInstance()),
	m_pObjectMgr(CObjectMgr::GetInstance()),
	m_pTimeMgr(CTimeMgr::GetInstance()),
	m_pKeyMgr(CKeyMgr::GetInstance())
{
}


CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Update()
{
	m_pKeyMgr->CheckKeyInput();
	m_pTimeMgr->UpdateTime();
	m_pSceneMgr->Update();
}

void CMainGame::LateUpdate()
{
	m_pSceneMgr->LateUpdate();
}

void CMainGame::Render(CFrameMgr & frameMgr)
{
	m_pGraphicDev->Render_Begin();

	m_pSceneMgr->Render();
	frameMgr.RenderFrame();

	m_pGraphicDev->Render_End_Sprite(); // 스프라이트만 종료

	if(m_pKeyMgr->KeyPressing(KEY_SPACE))
		m_pSceneMgr->LineRender();

	m_pGraphicDev->Render_End(); // 라인까지 다 종료.
}

HRESULT CMainGame::Initialize()
{
	ShowCursor(false);
	HRESULT hr = m_pTimeMgr->InitTimeMgr();
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = m_pGraphicDev->InitDevice(g_hWnd, 800, 600, MODE_FULL);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = m_pSceneMgr->SceneChange(CScene::LOGO);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	CSoundMgr::GetInstance()->Initialize();
	return S_OK;
}

void CMainGame::Release()
{
	CSoundMgr::GetInstance()->DestroyInstance();
	m_pKeyMgr->DestroyInstance();
	m_pTimeMgr->DestroyInstance();
	m_pSceneMgr->DestroyInstance();
	m_pObjectMgr->DestroyInstance();
	m_pTextureMgr->DestroyInstance();
	m_pGraphicDev->DestroyInstance();
	CDataSubject::GetInstance()->DestroyInstance();
}

CMainGame * CMainGame::Create()
{
	CMainGame* pInstance = new CMainGame;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
