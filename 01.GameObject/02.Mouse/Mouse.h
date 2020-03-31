#pragma once
#include "GameObject.h"
#include "Player.h"
class CMouse :
	public CGameObject
{
public:
	enum STATE { BATTLE, CHAT, CLICK, GATE, MAGIC, NOPASS, NORMAL, PICK };
private:
	explicit CMouse();
public:
	virtual ~CMouse();
public:
	static D3DXVECTOR3 GetMousePos();
	STATE Get_MouseState() { return m_eCurState; }
	CGameObject* Get_Target() { return m_pTarget; }
public:
	void Set_MouseState(CMouse::STATE eState) { m_eCurState = eState; }
	void Set_Target(CGameObject* pTarget) { m_pTarget = pTarget; }
	// CGameObject을(를) 통해 상속됨
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CMouse* Create();

private:
	void MakeWorldMatrix();
	void MotionChange();
	void MoveFrame(float fSpeed = 1.f);
	void KeyCheck();
private:
	STATE		m_eCurState;
	STATE		m_ePreState;
	FRAME		m_tFrame;
	CKeyMgr*	m_pKeyMgr;
	CGameObject*m_pTarget;

	// CGameObject을(를) 통해 상속됨
	virtual void LineRender() override;
};

