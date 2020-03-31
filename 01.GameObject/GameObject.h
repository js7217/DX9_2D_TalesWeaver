#pragma once
class CGameObject
{
public:
	enum OBJECT_TYPE { TERRAIN, PLAYER, NPC, MONSTER, BULLET, EFFECT, UI, BUTTON, MOUSE, END };
public:
	explicit CGameObject();
	virtual ~CGameObject();

public:
	const RECT& Get_Rect() { return m_tRect; }
	const INFO& Get_Info() { return m_tInfo; }
	const OBJECT_TYPE& Get_ObjType() { return m_eObjType; }
	const GroupID::ID Get_GroupID() { return m_eGroupID; }
	const wstring& Get_StateKey() { return m_strStateKey; }
public:
	void Set_Pos(float fX, float fY);
public:
	virtual int Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render() PURE;
	virtual void LineRender() PURE;
protected:
	virtual HRESULT Initialize() PURE;
	virtual HRESULT LateInit();
	virtual void Release() PURE;
protected:
	void UpdateRect(const float& fWidth = 0.f, const float& fHeight = 0.f);
	void UpdateRect_Scroll(const float& fWidth = 0.f, const float& fHeight = 0.f);
protected:
	bool m_bIsInit = false;
	OBJECT_TYPE		m_eObjType; // 오프젝트 타입.
	CTimeMgr*		m_pTimeMgr;
	CTextureMgr*	m_pTextureMgr;
	CGraphicDev*	m_pGraphicDev;
	GroupID::ID		m_eGroupID;
	wstring			m_strObjKey;
	wstring			m_strStateKey;
	wstring			m_strDirectionKey;
	RECT			m_tRect;
	INFO			m_tInfo;
};

