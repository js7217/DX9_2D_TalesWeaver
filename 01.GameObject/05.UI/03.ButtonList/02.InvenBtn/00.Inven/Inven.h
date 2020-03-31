#pragma once
#include "GameObject.h"

class CObserver;
class CInven :
	public CGameObject
{
private:
	explicit CInven();
	explicit CInven(const D3DXVECTOR3& vPos);
public:
	virtual ~CInven();
public:
	void Set_Open() { m_bIsClose = false; }
	const bool& Get_Close() { return m_bIsClose; }
	const vector<CGameObject*>& Get_Inven() { return m_vecItem; }
public:
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
	static CInven* Create(const D3DXVECTOR3& vPos);

private: // �κ��� �÷��̾�� ������ ���� ���� ������ �غ�����...
	bool			m_bIsClose;
	CGameObject*	m_pCloseBtn;
	vector<CGameObject*>	m_vecItem; // �κ��丮
	float				m_fDist; // ������ ����
};

