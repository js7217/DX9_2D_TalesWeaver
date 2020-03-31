#pragma once
#include "GameObject.h"

class CObserver;
class CEquip :
	public CGameObject
{
private:
	explicit CEquip();
	explicit CEquip(const D3DXVECTOR3& vPos);
public:
	virtual ~CEquip();

public:
	void Set_Open() { m_bIsClose = false; }
	const bool& Get_Close() { return m_bIsClose; }
	void CreateItem(ITEM tItem, const wstring& wstrStateKey);
public:
	// CGameObject��(��) ���� ��ӵ�
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void LineRender() override;
private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInit();
	virtual void Release() override;

public:
	static CEquip* Create(const D3DXVECTOR3& vPos);

private:
	bool				m_bIsClose;
	CGameObject*		m_pCloseBtn;
	CDataSubject*		m_pDataSubject;
	CObserver*			m_pObserver;
	vector<CGameObject*> m_vecEquip;
};

