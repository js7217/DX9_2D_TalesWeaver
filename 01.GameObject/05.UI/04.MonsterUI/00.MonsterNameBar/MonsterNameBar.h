#pragma once
#include "GameObject.h"
class CMonsterNameBar :
	public CGameObject
{
private:
	explicit CMonsterNameBar();
	explicit CMonsterNameBar(const wstring& wstrName, CGameObject* pMonster);
public:
	virtual ~CMonsterNameBar();

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
	static CMonsterNameBar* Create(const wstring& wstrName, CGameObject* pMonster);

private:
	wstring m_wstrName;
	CGameObject* m_pTarget;
};

