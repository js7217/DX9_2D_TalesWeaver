#pragma once
#include "GameObject.h"

class CObserver;
class CStatus :
	public CGameObject
{
private:
	explicit CStatus();
	explicit CStatus(const D3DXVECTOR3& vPos);
public:
	virtual ~CStatus();

public:
	void Set_Open() { m_bIsClose = false; }
	const bool& Get_Close() { return m_bIsClose; }
public:
	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void LineRender() override;
private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInit();
	virtual void Release() override;

public:
	static CStatus* Create(const D3DXVECTOR3& vPos);

private:
	void RenderText();
private:
	bool			m_bIsClose;
	float			m_fMaxSp;
	float			m_fSp;
	int				m_iMaxMp;
	int				m_iSeed;
	int				m_iLevel;
	int				m_iExp;
	int				m_iMaxExp;
	int				m_iCExp;
	int				m_iWidth;
	CDataSubject*	m_pDataSubject;
	CObserver*		m_pObserver;
	CGameObject*	m_pCloseBtn;
	UNIT_DATA		m_tData;
	RECT			m_tRectExp;
};

