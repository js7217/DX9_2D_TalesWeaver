#pragma once
#include "Observer.h"
class CUIObserver :
	public CObserver
{
public:
	CUIObserver();
	virtual ~CUIObserver();

public:
	const int& GetHp() const;
	const int& GetMp() const;
	const float& GetSp() const;
	const int& GetMaxHp() const;
	const int& GetMaxMp() const;
	const float& GetMaxSp() const;
	const float& GetSeed() const;
	const float& GetLevel() const;
	const float& GetExp() const;
	const float& GetMaxExp() const;
	const D3DXMATRIX& GetWorldMatrix() const;


	// CObserver을(를) 통해 상속됨
	virtual void Update(int iMessage, void * pData) override;

public:
	static CUIObserver* Create();

private:
	int			m_iHp = 0;
	int			m_iMp = 0;
	int			m_iSeed = 0;
	int			m_iMaxHp = 0;
	int			m_iMaxMp = 0;
	int			m_iLevel = 0;
	int			m_iExp = 0;
	int			m_iMaxExp = 0;
	float		m_fSp = 0.f;
	float		m_fMaxSp = 0.f;
	D3DXMATRIX	m_matWorld;
};

