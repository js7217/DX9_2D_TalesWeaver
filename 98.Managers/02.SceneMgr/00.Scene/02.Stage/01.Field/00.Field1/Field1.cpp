#include "stdafx.h"
#include "Field1.h"
#include "Terrain.h"
#include "Player.h"
#include "ScrollMgr.h"
CField1::CField1()
{
}


CField1::~CField1()
{
	Release();
}

void CField1::Update()
{
//	CScene::LateInit();
	m_pObjectMgr->Update();

	BYTE byOption = dynamic_cast<CPlayer*>(m_pObjectMgr->
		GetObject(CGameObject::PLAYER))->Get_PlayerTileOption();

	if (2 == byOption)
	{
		//	CSceneMgr::GetInstance()->SceneChange(CScene::SHOP);
	}
	else if (3 == byOption) // 라이디아
	{
		m_pObjectMgr->GetObject(CGameObject::PLAYER)->Set_Pos(720.f, 650.f);
		dynamic_cast<CPlayer*>(m_pObjectMgr->GetObject(CGameObject::PLAYER))->Set_Mode(CPlayer::NOATT);
		dynamic_cast<CPlayer*>(m_pObjectMgr->GetObject(CGameObject::PLAYER))->Set_DirectionKey(L"Down");
		CSceneMgr::GetInstance()->SceneChange(CScene::CHAPTER3);
		CScrollMgr::Set_Scroll(D3DXVECTOR3{ 700.f - WINCX * 0.5f, 950.f - WINCY * 0.5f, 0.f });
	}
	else if (4 == byOption) // 클라드
	{
		CSceneMgr::GetInstance()->SceneChange(CScene::CLAD);
		m_pObjectMgr->GetObject(CGameObject::PLAYER)->Set_Pos(1300.f, 1500.f);
		dynamic_cast<CPlayer*>(m_pObjectMgr->GetObject(CGameObject::PLAYER))->Set_Mode(CPlayer::NOATT);
		CScrollMgr::Set_Scroll(D3DXVECTOR3{ 1300.f - WINCX * 0.5f, 1500.f - WINCY * 0.5f, 0.f });
	}
}

void CField1::LateUpdate()
{
	m_pObjectMgr->LateUpdate();
}

void CField1::Render()
{
	m_pObjectMgr->Render();
}

void CField1::LineRender()
{
	m_pObjectMgr->LineRender();
}

HRESULT CField1::Initialize()
{
	CGameObject* pGameObject = nullptr;

	pGameObject = CTerrain::Create(L"../Data/Map1_Tile.dat");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	HRESULT hr = m_pObjectMgr->AddObject(CGameObject::TERRAIN, pGameObject);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	m_pObjectMgr->LoadObject(L"../Data/Field1_Data.txt");
	CSoundMgr::GetInstance()->PlayBGM(L"Field.mp3");
	return S_OK;
}

void CField1::Release()
{
	CObjectMgr::GetInstance()->DestroyRenderList();
	CObjectMgr::GetInstance()->DestroyGroup(CGameObject::EFFECT);
	CObjectMgr::GetInstance()->DestroyGroup(CGameObject::MONSTER);
	CObjectMgr::GetInstance()->DestroyGroup(CGameObject::TERRAIN);
	CSoundMgr::GetInstance()->StopSound(CSoundMgr::BGM);
}

CField1 * CField1::Create()
{
	CField1* pInstance = new CField1;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
