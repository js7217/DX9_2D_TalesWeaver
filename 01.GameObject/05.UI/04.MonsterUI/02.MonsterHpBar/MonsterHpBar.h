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
	// CGameObject��(��) ���� ��ӵ�
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
	CGameObject*	m_pMonster; // ǥ���� ���� �ּ�.
	int				m_iMaxHp;
	int				m_iHp;
	int				m_iWidth; // ���ҽ� ����.
	int				m_iCHp;
	RECT			m_tRect;
};

