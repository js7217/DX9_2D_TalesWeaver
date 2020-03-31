#pragma once
#include "GameObject.h"

class CObserver;
class CSmallBar :
	public CGameObject
{
public:
	CSmallBar();
	CSmallBar(CGameObject* pTarget);
	virtual ~CSmallBar();

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
	static HRESULT Create(CGameObject* pTarget);

private:
	CDataSubject*		m_pDataSubject;
	CObserver*			m_pObserver;
	UNIT_DATA			m_tData;
	CGameObject*		m_pTarget;
	RECT				m_tRect[2];
	int					m_iMaxMp;
	int					m_iFrame;
	int					m_iWidth;
	int					m_iCHp;
	int					m_iCMp;
};

