#pragma once

#include "GameObject.h"

class CObjectMgr
{
	DECLARE_SINGLETON(CObjectMgr)

private:
	CObjectMgr();
	~CObjectMgr();

public:
	CGameObject* GetObject(CGameObject::OBJECT_TYPE eType);
	const OBJLST& Get_List(CGameObject::OBJECT_TYPE eType) {return m_ObjLst[eType]; }
public:
	HRESULT AddObject(CGameObject::OBJECT_TYPE eType, CGameObject* pObject);
	void DestroyGroup(CGameObject::OBJECT_TYPE eType);
	void DestroyRenderList();
	void Update();
	void LateUpdate();
	void Render();
	void LineRender();
public:
	HRESULT LoadObject(const wstring& strDlgPathName);
private:
	void Release();

private:
	OBJLST			m_ObjLst[CGameObject::END];
	OBJLST			m_RenderList[GroupID::END];
};

