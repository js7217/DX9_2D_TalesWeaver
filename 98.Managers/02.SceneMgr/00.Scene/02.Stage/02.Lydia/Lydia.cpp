#include "stdafx.h"
#include "Lydia.h"
#include "Player.h"
#include "Terrain.h"
#include "ScrollMgr.h"
#include "Linda.h"
CLydia::CLydia()
{
}


CLydia::~CLydia()
{
	Release();
}

void CLydia::Update()
{
	m_pObjectMgr->Update();

	BYTE byOption = dynamic_cast<CPlayer*>(m_pObjectMgr->
		GetObject(CGameObject::PLAYER))->Get_PlayerTileOption();

	switch (byOption)
	{
	case 2: // 상점
		break;
	case 3: // 워프(보스맵)
		CSceneMgr::GetInstance()->SceneChange(CScene::BOSS);
		m_pObjectMgr->GetObject(CGameObject::PLAYER)->Set_Pos(1664.f, 915.f);
		dynamic_cast<CPlayer*>(m_pObjectMgr->GetObject(CGameObject::PLAYER))->Set_Mode(CPlayer::ATT);
		CScrollMgr::Set_Scroll(D3DXVECTOR3{ 1664.f - WINCX * 0.5f, 915.f - WINCY * 0.5f, 0.f });
		break;
	case 4: // 필드1
		CSceneMgr::GetInstance()->SceneChange(CScene::FIELD1);
		m_pObjectMgr->GetObject(CGameObject::PLAYER)->Set_Pos(5000.f, 920.f);
		dynamic_cast<CPlayer*>(m_pObjectMgr->GetObject(CGameObject::PLAYER))->Set_Mode(CPlayer::ATT);
		CScrollMgr::Set_Scroll(D3DXVECTOR3{ 5000.f - WINCX * 0.5f, 920.f - WINCY * 0.5f, 0.f });
		break;
	default:
		break;
	}

}

void CLydia::LateUpdate()
{
	m_pObjectMgr->LateUpdate();
}

void CLydia::Render()
{
	m_pObjectMgr->Render();
}

void CLydia::LineRender()
{
	m_pObjectMgr->LineRender();
}

HRESULT CLydia::Initialize()
{
	CGameObject* pGameObject = nullptr;

	pGameObject = CTerrain::Create(L"../Data/Lydia_Tile.dat");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	HRESULT hr = m_pObjectMgr->AddObject(CGameObject::TERRAIN, pGameObject);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	CLinda::Create(600.f, 640.f);
	CSoundMgr::GetInstance()->PlayBGM(L"Lydia.mp3");
	return S_OK;
}

void CLydia::Release()
{
	CObjectMgr::GetInstance()->DestroyRenderList();
	CObjectMgr::GetInstance()->DestroyGroup(CGameObject::EFFECT);
	CObjectMgr::GetInstance()->DestroyGroup(CGameObject::NPC);
	CObjectMgr::GetInstance()->DestroyGroup(CGameObject::TERRAIN);
	CSoundMgr::GetInstance()->StopSound(CSoundMgr::BGM);
}

CLydia * CLydia::Create()
{
	CLydia* pInstance = new CLydia;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
