#pragma once
#include "GameObject.h"
class CItem :
	public CGameObject
{
private:
	explicit CItem();
public:
	virtual ~CItem();

public:
	void Set_Data(ITEM tItem) { m_tItem = tItem; }
	void Set_StateKey(const wstring& wstrStateKey) { m_strStateKey = wstrStateKey; }
	void Set_ItemLocation(ITEM_LOCATION eLocation) { m_tItem.eLocation = eLocation; }
	void Set_ItemCount(const int& iCount) { m_tItem.iCount = iCount; }
public:
	const ITEM& Get_Item() { return m_tItem; }
public:
	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void LineRender() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

private:
	void ClickInven();
	void ClickEquip();
	void ClickSellShop();
	void ClickBuyShop();
public:
	static CItem* Create(ITEM tItem, D3DXVECTOR3 vPos, const wstring& wstrStateKey);
private:
	ITEM m_tItem; // 아이템 구조체.
};

