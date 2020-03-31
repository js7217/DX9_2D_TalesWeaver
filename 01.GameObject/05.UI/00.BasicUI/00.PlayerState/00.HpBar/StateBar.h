#pragma once
#include "GameObject.h"

class CObserver;
class CStateBar :
	public CGameObject
{
public:
	CStateBar();
	virtual ~CStateBar();

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
	static HRESULT Create();

private:
	void RenderText();
private:
	CDataSubject*	m_pDataSubject;
	CObserver*		m_pObserver;
	UNIT_DATA		m_tData;
	RECT			m_tRect[3];
	int				m_iWidth; // 바 길이 계산용
	float			m_fMaxSp;
	float			m_fSp;
	int				m_iMaxMp;
	int				m_iFrame;
	int				m_iCHp;
	int				m_iCMp;
	int				m_iCSp;
	int				m_iMaxExp;
	int				m_iExp;
};

