#include "stdafx.h"
#include "ObjectMgr.h"
#include "GameObject.h"
#include "CollisionMgr.h"
#include "Jelly.h"
#include "Soracap.h"
#include "HorseBee.h"
#include "Kappa.h"
IMPLEMENT_SINGLETON(CObjectMgr)

CObjectMgr::CObjectMgr()
{
}


CObjectMgr::~CObjectMgr()
{
	Release();
}

CGameObject * CObjectMgr::GetObject(CGameObject::OBJECT_TYPE eType)
{
	if (m_ObjLst[eType].empty())
		return nullptr;
	return m_ObjLst[eType].front();
}

HRESULT CObjectMgr::AddObject(CGameObject::OBJECT_TYPE eType, CGameObject * pObject)
{
	NULL_CHECK_RETURN(pObject, E_FAIL);	
	m_ObjLst[eType].push_back(pObject);

	return S_OK;
}

void CObjectMgr::DestroyGroup(CGameObject::OBJECT_TYPE eType)
{
	for_each(m_ObjLst[eType].begin(), m_ObjLst[eType].end(), SafeDelete<CGameObject*>);
	m_ObjLst[eType].clear();
}

void CObjectMgr::DestroyRenderList()
{
	for (int i = 0; i < GroupID::END; ++i)
	{
		m_RenderList[i].clear();
	}
}

void CObjectMgr::Update()
{
	OBJLST::iterator iter_begin;
	OBJLST::iterator iter_end;

	if (!m_ObjLst[CGameObject::PLAYER].empty())
	{
		CCollisionMgr::CollisionRectMouse(m_ObjLst[CGameObject::MOUSE], m_ObjLst[CGameObject::MONSTER]);
		CCollisionMgr::CollisionRectSkill(m_ObjLst[CGameObject::BULLET], m_ObjLst[CGameObject::MONSTER]);
	}

	// 시ㅣㅣㅣ발 이거 확인 ㄱ
	for (int i = 0; i < CGameObject::END; ++i)
	{
		iter_begin = m_ObjLst[i].begin();
		iter_end = m_ObjLst[i].end();

		for (; iter_begin != iter_end; )
		{
			int iEvent = (*iter_begin)->Update();

			if (DEAD_OBJ == iEvent)
			{
				SafeDelete(*iter_begin);
				iter_begin = m_ObjLst[i].erase(iter_begin);
			}
			else
				++iter_begin;
		}
	}	
}

void CObjectMgr::LateUpdate()
{
	for (int i = 0; i < CGameObject::END; ++i)
	{
		for (auto& pObject : m_ObjLst[i])
		{
			pObject->LateUpdate();

			if (m_ObjLst[i].empty())
				break;

			GroupID::ID eID = pObject->Get_GroupID();

			m_RenderList[eID].emplace_back(pObject);
		}
	}
}

void CObjectMgr::Render()
{
	/*for (int i = 0; i < CGameObject::END; ++i)
	{
		for (auto& pObject : m_ObjLst[i])
			pObject->Render();
	}*/
	for (int i = 0; i < GroupID::END; ++i)
	{
		if (GroupID::UI != i)
		{
			m_RenderList[i].sort([](auto& rDst, auto& rSrc)
			{
				return rDst->Get_Info().vPos.y < rSrc->Get_Info().vPos.y;
			});
		}

		for (auto& pObj : m_RenderList[i])
		{
			pObj->Render();

			if (m_RenderList[i].empty())
				break;
		}

		m_RenderList[i].clear();
	}
}

void CObjectMgr::LineRender()
{
	for (int i = 0; i < CGameObject::END; ++i)
	{
		for (auto& pObject : m_ObjLst[i])
			pObject->LineRender();
	}
}

HRESULT CObjectMgr::LoadObject(const wstring & strDlgPathName)
{
	wifstream fin;

	fin.open(strDlgPathName.c_str());

	if (fin.fail())
	{
		ERR_MSG(L"LoadObject Failed");
		return E_FAIL;
	}

	int iDrawID = 0;
	D3DXVECTOR3 vPos = { 0.f, 0.f, 0.f };
	float fSizeX = 0.f;
	UNIT_DATA tData = {};

	TCHAR szDrawID[MIN_STR] = L"";
	TCHAR szPosX[MIN_STR] = L"";
	TCHAR szPosY[MIN_STR] = L"";
	TCHAR szSizeX[MIN_STR] = L"";
	TCHAR szName[MIN_STR] = L"";
	TCHAR szObjKey[MIN_STR] = L"";
	TCHAR szHp[MIN_STR] = L"";
	TCHAR szAtt[MIN_STR] = L"";

	while (true) // 저장목록 : DrawID, 위치, 사이즈, 이름, 오브젝트키, 공격력, 체력
	{
		fin.getline(szDrawID, MIN_STR);
		fin.getline(szPosX, MIN_STR);
		fin.getline(szPosY, MIN_STR);
		fin.getline(szSizeX, MIN_STR);
		fin.getline(szName, MIN_STR);
		fin.getline(szObjKey, MIN_STR);
		fin.getline(szHp, MIN_STR);
		fin.getline(szAtt, MIN_STR);

		if (fin.eof())
			break;

		iDrawID = _wtoi(szDrawID);

		vPos.x = _wtof(szPosX); //
		vPos.y = _wtof(szPosY); //
		fSizeX = _wtof(szSizeX);

		tData.strName = szName; //
		tData.strObjKey = szObjKey; //
		tData.iHp = _wtoi(szHp); //
		tData.iAtt = _wtoi(szAtt); //
		tData.iMaxHp = tData.iHp; //
		tData.iMP = 0; //

		if (L"Jelly" == tData.strObjKey)
		{
			CGameObject* pObj = CJelly::Create(vPos.x, vPos.y, tData);
			AddObject(CGameObject::MONSTER, pObj);
		}
		else if (L"Soracap" == tData.strObjKey)
		{
			HRESULT hr = CSoracap::Create(vPos.x, vPos.y, tData);
			FAILED_CHECK_RETURN(hr, E_FAIL);
		}
		else if (L"HorseBee" == tData.strObjKey)
		{
			HRESULT hr = CHorseBee::Create(vPos.x, vPos.y, tData);
			FAILED_CHECK_RETURN(hr, E_FAIL);
		}
		else if (L"Kappa" == tData.strObjKey)
		{
			HRESULT hr = CKappa::Create(vPos.x, vPos.y, tData);
			FAILED_CHECK_RETURN(hr, E_FAIL);
		}
	}

	return S_OK;
}

void CObjectMgr::Release()
{
	for (int i = 0; i < CGameObject::END; ++i)
	{
		for_each(m_ObjLst[i].begin(), m_ObjLst[i].end(), SafeDelete<CGameObject*>);
		m_ObjLst[i].clear();
	}
}
