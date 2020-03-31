#include "stdafx.h"
#include "MainMenu.h"
#include "Mouse.h"
#include "MenuButton.h"

CMainMenu::CMainMenu()
{
}


CMainMenu::~CMainMenu()
{
	Release();
}

void CMainMenu::Update()
{
	m_pObjectMgr->Update();
}

void CMainMenu::LateUpdate()
{
	m_pObjectMgr->LateUpdate();
}

void CMainMenu::Render()
{
	D3DXMATRIX matWorld;

	D3DXMatrixTranslation(&matWorld, 0.f, 0.f, 0.f);

	const TEX_INFO* pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(L"Menu", 0, L"MainMenu");
	NULL_CHECK(pTexInfo);

	CGraphicDev::GetInstance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDev::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pObjectMgr->Render();
}

void CMainMenu::LineRender()
{
	m_pObjectMgr->LineRender();
}

HRESULT CMainMenu::Initialize()
{
	// 버튼 처음이 400, 405
	CGameObject* pGameObject = CMouse::Create();
	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	m_pObjectMgr->AddObject(CGameObject::MOUSE, pGameObject);

	CMenuButton::Create(L"Menu", L"StartBtn", D3DXVECTOR3{ 400.f, 405.f, 0.f });
	CMenuButton::Create(L"Menu", L"OpenningBtn", D3DXVECTOR3{ 400.f, 429.f, 0.f });
	CMenuButton::Create(L"Menu", L"CreditBtn", D3DXVECTOR3{ 400.f, 454.f, 0.f });
	CMenuButton::Create(L"Menu", L"ExitBtn", D3DXVECTOR3{ 400.f, 478.f, 0.f });
	CSoundMgr::GetInstance()->PlayBGM(L"MainMenu.mp3");
	return S_OK;
}

void CMainMenu::Release()
{
	CSoundMgr::GetInstance()->StopSound(CSoundMgr::BGM);
}

CMainMenu * CMainMenu::Create()
{
	CMainMenu* pInstance = new CMainMenu;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
