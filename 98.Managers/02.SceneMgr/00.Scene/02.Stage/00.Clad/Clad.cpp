#include "stdafx.h"
#include "Clad.h"

#include "Player.h"
#include "Terrain.h"
#include "ScrollMgr.h"
#include "Linda.h"
#include "Jelly.h"
CClad::CClad()
{
}


CClad::~CClad()
{
	Release();
}

void CClad::Update()
{
	CScene::LateInit();
	m_pObjectMgr->Update();

	BYTE byOption = dynamic_cast<CPlayer*>(m_pObjectMgr->
		GetObject(CGameObject::PLAYER))->Get_PlayerTileOption();

	if (2 == byOption)
		CSceneMgr::GetInstance()->SceneChange(CScene::SHOP);
	else if (3 == byOption)
	{
		m_pObjectMgr->GetObject(CGameObject::PLAYER)->Set_Pos(1200.f, 1200.f);
		dynamic_cast<CPlayer*>(m_pObjectMgr->GetObject(CGameObject::PLAYER))->Set_Mode(CPlayer::ATT);
		// 순서를 플레이어부터 무조건 해야함!!!!
		CSceneMgr::GetInstance()->SceneChange(CScene::FIELD1);
		CScrollMgr::Set_Scroll(D3DXVECTOR3{ 1200.f - WINCX * 0.5f, 1200.f - WINCY * 0.5f, 0.f });
	}

	if (false == m_bIsChapter2)
	{
		if (m_pObjectMgr->Get_List(CGameObject::MONSTER).empty())
		{
			CScene::m_bIsInit = false;
			CSceneMgr::GetInstance()->SceneChange(CScene::CHAPTER2);
		}
	}
}

void CClad::LateUpdate()
{
	m_pObjectMgr->LateUpdate();
}

void CClad::Render()
{
	m_pObjectMgr->Render();
}

HRESULT CClad::Initialize()
{
	CGameObject* pGameObject = nullptr;
	m_bIsChapter2 = true; // 일단 트루상태.
	pGameObject = CTerrain::Create(L"../Data/Clad_Tile.dat");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	HRESULT hr = m_pObjectMgr->AddObject(CGameObject::TERRAIN, pGameObject);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	if (m_pObjectMgr->Get_List(CGameObject::PLAYER).empty())
	{
		CObjectMgr::GetInstance()->DestroyGroup(CGameObject::PLAYER);
		CGameObject* pGameObject = CPlayer::Create();
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		HRESULT hr = m_pObjectMgr->AddObject(CGameObject::PLAYER, pGameObject);
		pGameObject->Set_Pos(2080.f, 1060.f);
		FAILED_CHECK_RETURN(hr, E_FAIL);
	}
//	CSoundMgr::GetInstance()->PlayBGM(L"Clad.mp3");
	return S_OK;
}

HRESULT CClad::LateInit()
{
	UNIT_DATA tData = {};
	tData.iAtt = 10;
	tData.iHp = 10;
	tData.iMaxHp = 10;
	tData.strName = L"젤리삐";

	CGameObject* pObj = CJelly::Create(1940.f, 1100.f, tData);
	CObjectMgr::GetInstance()->AddObject(CGameObject::MONSTER, pObj);

	pObj = CJelly::Create(1800.f, 1100.f, tData);
	CObjectMgr::GetInstance()->AddObject(CGameObject::MONSTER, pObj);

	pObj = CJelly::Create(1900.f, 1000.f, tData);
	CObjectMgr::GetInstance()->AddObject(CGameObject::MONSTER, pObj);
	m_bIsChapter2 = false;
	return S_OK;
}

void CClad::Release()
{
	CObjectMgr::GetInstance()->DestroyRenderList();
	CObjectMgr::GetInstance()->DestroyGroup(CGameObject::EFFECT); // 이펙트 삭제
	CObjectMgr::GetInstance()->DestroyGroup(CGameObject::MONSTER); // 몬스터 삭제
	CObjectMgr::GetInstance()->DestroyGroup(CGameObject::TERRAIN); // 터레인 삭제
	CSoundMgr::GetInstance()->StopSound(CSoundMgr::BGM);
}

CClad * CClad::Create()
{
	CClad* pInstance = new CClad;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CClad::LineRender()
{
	m_pObjectMgr->LineRender();
}
