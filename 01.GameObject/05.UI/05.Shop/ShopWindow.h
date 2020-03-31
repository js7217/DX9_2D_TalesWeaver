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
	// CGameObject을(를) 통해 상속됨
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
	int						m_iTotalPrice; // 총 가격
	CGameObject*			m_pCloseBtn; // 닫기
	CGameObject*			m_pBuyBtn; // 구매상점탭
	CGameObject*			m_pSellBtn; // 판매상점 탭
	CGameObject*			m_pOkBtn; // 확인.
	CGameObject*			m_pCancelBtn; // 취소.
	vector<CGameObject*>	m_vecBuyShop;
	vector<CGameObject*>	m_vecReady;
	vector<CGameObject*>	m_vecSellShop;
	// 구매, 판매, 확인, 취소 버튼 추가
};

