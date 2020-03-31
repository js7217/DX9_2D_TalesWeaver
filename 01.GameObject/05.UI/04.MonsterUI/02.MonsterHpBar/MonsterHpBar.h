#pragma once
#include "GameObject.h"
class CMonsterHpBar :
	public CGameObject
{
private:
	explicit CMonsterHpBar();
	explicit CMonsterHpBar(CGameObject* pMonster);
	virtual ~CMonsterHpBar();

public:
	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void LineRender() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CMonsterHpBar* Create(CGameObject* pMonster);

private:
	CGameObject*	m_pMonster; // 표현할 몬스터 주소.
	int				m_iMaxHp;
	int				m_iHp;
	int				m_iWidth; // 리소스 길이.
	int				m_iCHp;
	RECT			m_tRect;
};

