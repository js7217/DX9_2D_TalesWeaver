#pragma once
#include "GameObject.h"
class CShopWindow :
	public CGameObject
{
public:
	CShopWindow();
	virtual ~CShopWindow();

public:
	void Set_Open() { m_bIsClose = false; }
	const bool& Get_Close() { return m_bIsClose; }

public:
	void CreateSellShop(ITEM tItem, const wstring& wstrStateKey);
	void CreateReadyShop(ITEM tItem, const wstring& wstrStateKey);
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
	static CShopWindow* Create(const int& iType);
	void InitSellShop();

private:
	void Render_Shop();
	void Render_Price();
	void InitButton();
	void InitBuyWeaponShop();
	void InitBuyPotionShop();
private:
	bool					m_bIsClose;
	int						m_iFrame;
	int						m_iTotalPrice; // �� ����
	CGameObject*			m_pCloseBtn; // �ݱ�
	CGameObject*			m_pBuyBtn; // ���Ż�����
	CGameObject*			m_pSellBtn; // �ǸŻ��� ��
	CGameObject*			m_pOkBtn; // Ȯ��.
	CGameObject*			m_pCancelBtn; // ���.
	vector<CGameObject*>	m_vecBuyShop;
	vector<CGameObject*>	m_vecReady;
	vector<CGameObject*>	m_vecSellShop;
	// ����, �Ǹ�, Ȯ��, ��� ��ư �߰�
};

