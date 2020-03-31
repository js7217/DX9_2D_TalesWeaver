#pragma once

class CSceneMgr;
class CTextureMgr;
class CObjectMgr;
class CScene
{
public:
	enum SCENE_TYPE { LOGO, MENU, CLAD, FIELD1, BOSS, LYDIA, SHOP, CHAPTER1, CHAPTER2, CHAPTER3, CHAPTER4, END };

protected:
	explicit CScene();

public:
	virtual ~CScene();

public:	
	virtual void Update()			PURE;
	virtual void LateUpdate()		PURE;
	virtual void Render()			PURE;
	virtual void LineRender()		PURE;
protected:
	virtual HRESULT Initialize()	PURE;
	virtual HRESULT LateInit();
	virtual void Release()			PURE;
public:
	static bool		m_bIsInit;
protected:
	CSceneMgr*		m_pSceneMgr;
	CTextureMgr*	m_pTextureMgr;
	CObjectMgr*		m_pObjectMgr;
};

