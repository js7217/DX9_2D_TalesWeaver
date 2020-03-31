#include "stdafx.h"
#include "BossField.h"
#include "Terrain.h"
#include "JellyKing.h"

#include "ScrollMgr.h"
CBossField::CBossField()
{
}


CBossField::~CBossField()
{
	Release();
}

void CBossField::Update()
{
	CScene::LateInit();
	m_pObjectMgr->Update();
	if (m_pObjectMgr->Get_List(CGameObject::MONSTER).empty())
	{
		CSceneMgr::GetInstance()->SceneChange(CScene::CHAPTER4);
	}
}

void CBossField::LateUpdate()
{
	m_pObjectMgr->LateUpdate();
	if (CKeyMgr::GetInstance()->KeyDown(KEY_RETURN))
	{
		CSceneMgr::GetInstance()->SceneChange(CScene::CLAD);
		m_pObjectMgr->GetObject(CGameObject::PLAYER)->Set_Pos(1300.f, 1500.f);
		CScrollMgr::Set_Scroll(D3DXVECTOR3{ 1300.f - WINCX * 0.5f, 1500.f - WINCY * 0.5f, 0.f });
	}
}

void CBossField::Render()
{
	m_pObjectMgr->Render();
}

void CBossField::LineRender()
{
	m_pObjectMgr->LineRender();
}

HRESULT CBossField::Initialize()
{
	CGameObject* pGameObject = nullptr;

	pGameObject = CTerrain::Create(L"../Data/Map0_Tile.dat");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	HRESULT hr = m_pObjectMgr->AddObject(CGameObject::TERRAIN, pGameObject);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	CJellyKing::Create(1100.f, 600.f);

	m_pObjectMgr->LoadObject(L"../Data/Boss_Data.txt");
	CSoundMgr::GetInstance()->PlayBGM(L"Boss.mp3");
	return S_OK;
}

void CBossField::Release()
{
	CObjectMgr::GetInstance()->DestroyRenderList();
	CObjectMgr::GetInstance()->DestroyGroup(CGameObject::EFFECT);
	CObjectMgr::GetInstance()->DestroyGroup(CGameObject::MONSTER);
	CObjectMgr::GetInstance()->DestroyGroup(CGameObject::TERRAIN);
	CSoundMgr::GetInstance()->StopSound(CSoundMgr::BGM);
}

CBossField * CBossField::Create()
{
	CBossField* pInstance = new CBossField;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
