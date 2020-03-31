#include "stdafx.h"
#include "Shop.h"
#include "Player.h"
#include "Terrain.h"
#include "Kenishi.h"
#include "ScrollMgr.h"
CShop::CShop()
{
}


CShop::~CShop()
{
	Release();
}

void CShop::Update()
{
	m_pObjectMgr->Update();

	BYTE byOption = dynamic_cast<CPlayer*>(m_pObjectMgr->
		GetObject(CGameObject::PLAYER))->Get_PlayerTileOption();

	if (2 == byOption)
	{
		CSceneMgr::GetInstance()->SceneChange(CScene::CLAD);
		//1020, 980
		m_pObjectMgr->GetObject(CGameObject::PLAYER)->Set_Pos(1020.f, 980.f);
		CScrollMgr::Set_Scroll(D3DXVECTOR3{ 1020.f - WINCX * 0.5f, 980.f - WINCY * 0.5f, 0.f });
	}
}

void CShop::LateUpdate()
{
	m_pObjectMgr->LateUpdate();
}

void CShop::Render()
{
	m_pObjectMgr->Render();
}

void CShop::LineRender()
{
	m_pObjectMgr->LineRender();
}

HRESULT CShop::Initialize()
{
	CGameObject* pGameObject = nullptr;

	pGameObject = CTerrain::Create(L"../Data/Shop_Tile.dat");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	HRESULT hr = m_pObjectMgr->AddObject(CGameObject::TERRAIN, pGameObject);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	m_pObjectMgr->GetObject(CGameObject::PLAYER)->Set_Pos(850.f, 570.f);

	CKenishi::Create(600.f, 290.f);
	CSoundMgr::GetInstance()->PlayBGM(L"Shop.mp3");
	// NPC³Ö±â.
	return S_OK;
}

void CShop::Release()
{
	CObjectMgr::GetInstance()->DestroyGroup(CGameObject::NPC);
	CObjectMgr::GetInstance()->DestroyGroup(CGameObject::TERRAIN);
	CSoundMgr::GetInstance()->StopSound(CSoundMgr::BGM);

	CSoundMgr::GetInstance()->PlayBGM(L"Clad.mp3");
}

CShop * CShop::Create()
{
	CShop* pInstance = new CShop;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
