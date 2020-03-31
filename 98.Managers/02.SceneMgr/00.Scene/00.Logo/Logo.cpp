#include "stdafx.h"
#include "Logo.h"
#include "LoadingBar.h"

CLogo::CLogo()
{
}


CLogo::~CLogo()
{
	Release();
}

void CLogo::Update()
{
	int iCount = CTextureMgr::GetInstance()->GetLoadingCount();

	dynamic_cast<CLoadingBar*>(m_pLoadingBar)->Set_BarGauge(iCount);
	m_pLoadingBar->Update();
}

void CLogo::LateUpdate()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		WaitForSingleObject(m_hLoadingThread, INFINITE);
		m_pSceneMgr->SceneChange(CScene::MENU);
		return;
	}
	m_pLoadingBar->LateUpdate();
}

void CLogo::Render()
{
	D3DXMATRIX matWorld;

	const wstring& wstrMessage = CTextureMgr::GetInstance()->GetLoadingMessage();

	D3DXMatrixTranslation(&matWorld, 0.f, 0.f, 0.f);

	const TEX_INFO* pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(L"Logo");
	NULL_CHECK(pTexInfo);

	CGraphicDev::GetInstance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDev::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixTranslation(&matWorld, 0.f, 550.f, 0.f);

	CGraphicDev::GetInstance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDev::GetInstance()->GetFont()->DrawText(
		CGraphicDev::GetInstance()->GetSprite(),
		wstrMessage.c_str(), wstrMessage.length(), nullptr, 0, D3DCOLOR_ARGB(255, 0, 255, 0));

	m_pLoadingBar->Render();
}

void CLogo::LineRender()
{
}

HRESULT CLogo::Initialize()
{
	HRESULT hr = CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TEX_MULTI,
		L"../Texture/Loading/Loading%d.png", L"Loading", L"NONE", 2);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TEX_MULTI,
		L"../Texture/Logo0.png", L"Logo", L"NONE", 1);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	m_pLoadingBar = CLoadingBar::Create(0.f, 455.f);

	InitializeCriticalSection(&m_CriticalSec);
	m_hLoadingThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingResource, this, 0, nullptr);

	return S_OK;
}

void CLogo::Release()
{
	SafeDelete(m_pLoadingBar);
	DeleteCriticalSection(&m_CriticalSec);
	CloseHandle(m_hLoadingThread);
}

CLogo * CLogo::Create()
{
	CLogo* pInstance = new CLogo;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}

unsigned int CLogo::LoadingResource(void * pParam)
{
	CLogo* pLogo = reinterpret_cast<CLogo*>(pParam);
	EnterCriticalSection(&pLogo->m_CriticalSec);

	// Texture Load

	HRESULT hr = CTextureMgr::GetInstance()->LoadFromImgPath(L"../Data/ImgPath.txt");
	FAILED_CHECK_RETURN(hr, 1);

	CTextureMgr::GetInstance()->SetLoadingMessage(L"로딩 완료!");

	LeaveCriticalSection(&pLogo->m_CriticalSec);

	return 0;
}
