#include "stdafx.h"
#include "ShopWindow.h"
#include "CloseBtn.h"
#include "Player.h"
#include "BuyBtn.h"
#include "OKBtn.h"
#include "SellBtn.h"
#include "CancelBtn.h"
#include "Inven.h"
#include "Item.h"
#include "BtnLst.h"
CShopWindow::CShopWindow()
	: m_bIsClose(false)
{
}


CShopWindow::~CShopWindow()
{
	Release();
}

void CShopWindow::CreateSellShop(ITEM tItem, const wstring & wstrStateKey)
{
	CGameObject* pItem = nullptr;
	tItem.eLocation = SELLSHOP;
	float fY = 70.f + (m_vecSellShop.size() * 25.f);

	pItem = CItem::Create(tItem, D3DXVECTOR3(m_tInfo.vPos.x - m_tInfo.fCX * 0.5f + 45.f,
		m_tInfo.vPos.y - m_tInfo.fCY * 0.5f + fY, 0.f),
		wstrStateKey);

	m_vecSellShop.emplace_back(pItem);
}

void CShopWindow::CreateReadyShop(ITEM tItem, const wstring & wstrStateKey)
{
	CGameObject* pItem = nullptr;
	tItem.eLocation = READY;
	float fY = 270.f + (m_vecReady.size() * 25.f);

	if (tItem.eType != ITEM_TYPE::WEAPON) // 물약일 경우
	{
		if (m_vecReady.empty()) // 비어있을땐 그냥 추가
		{
			pItem = CItem::Create(tItem, D3DXVECTOR3(m_tInfo.vPos.x - m_tInfo.fCX * 0.5f + 45.f,
				m_tInfo.vPos.y - m_tInfo.fCY * 0.5f + fY, 0.f),
				wstrStateKey);

			m_vecReady.emplace_back(pItem);
		}
		else // 비어있지않을때
		{
			int iSameCnt = 0;
			for (auto& Item : m_vecReady) // 같은이름이 해당 벡터 안에 있는지 검사.
			{
				if (tItem.strName == dynamic_cast<CItem*>(Item)->Get_Item().strName) // 같은이름이 있는경우 카운트만 증가
				{
					int iCount = dynamic_cast<CItem*>(Item)->Get_Item().iCount;
					dynamic_cast<CItem*>(Item)->Set_ItemCount(iCount + 1);
					iSameCnt++;
					break;
				}
				else
					continue;
			}

			if (0 == iSameCnt) // 같은 이름 카운트가 하나도 없다면 새로 추가
			{
				pItem = CItem::Create(tItem, D3DXVECTOR3(m_tInfo.vPos.x - m_tInfo.fCX * 0.5f + 45.f,
					m_tInfo.vPos.y - m_tInfo.fCY * 0.5f + fY, 0.f),
					wstrStateKey);

				m_vecReady.emplace_back(pItem);
			}
		}

	}
	else // 무기일경우
	{
		pItem = CItem::Create(tItem, D3DXVECTOR3(m_tInfo.vPos.x - m_tInfo.fCX * 0.5f + 45.f,
			m_tInfo.vPos.y - m_tInfo.fCY * 0.5f + fY, 0.f),
			wstrStateKey);

		m_vecReady.emplace_back(pItem);
	}
}

int CShopWindow::Update()
{
	if (m_bIsClose) // 닫힐때 구매예정 물품 삭제
	{
		m_iTotalPrice = 0;
		for_each(m_vecReady.begin(), m_vecReady.end(), SafeDelete<CGameObject*>);
		m_vecReady.clear();
		m_vecReady.shrink_to_fit();

		return NO_EVENT;
	}

	CGameObject::UpdateRect();
	m_pCloseBtn->Update();
	m_pBuyBtn->Update();
	m_pSellBtn->Update();
	m_pOkBtn->Update();
	m_pCancelBtn->Update();
	if (m_iFrame == 0)
	{
		ITEM_LOCATION eLocation = BUYSHOP;
		for (size_t i = 0; i < m_vecBuyShop.size(); ++i)
		{
			m_vecBuyShop[i]->Update();

			eLocation = dynamic_cast<CItem*>(m_vecBuyShop[i])->Get_Item().eLocation;

			if (eLocation == ITEM_LOCATION::READY)
			{
				CreateReadyShop(dynamic_cast<CItem*>(m_vecBuyShop[i])->Get_Item(), 
					dynamic_cast<CItem*>(m_vecBuyShop[i])->Get_StateKey());
				dynamic_cast<CItem*>(m_vecBuyShop[i])->Set_ItemLocation(ITEM_LOCATION::BUYSHOP);
				m_iTotalPrice += dynamic_cast<CItem*>(m_vecBuyShop[i])->Get_Item().iPrice;
			}
		}

		for (size_t i = 0; i < m_vecReady.size(); ++i)
		{
			m_vecReady[i]->Update();
		}
	}

	if (m_iFrame == 1) // 판매
	{
		CGameObject* pInven = dynamic_cast<CBtnLst*>(CObjectMgr::GetInstance()->GetObjectW(CGameObject::BUTTON))->Get_Inven();
		const vector<CGameObject*>& vecInven = dynamic_cast<CInven*>(pInven)->Get_Inven();

		for (size_t i = 0; i < m_vecSellShop.size(); ++i)
		{
			int iEvent = m_vecSellShop[i]->Update();
			int iCount = dynamic_cast<CItem*>(m_vecSellShop[i])->Get_Item().iCount;

			dynamic_cast<CItem*>(vecInven[i])->Set_ItemCount(iCount);
			
			if (DEAD_OBJ == iEvent)
			{
				SafeDelete(m_vecSellShop[i]);
				m_vecSellShop.erase(m_vecSellShop.begin() + i);
				for (size_t j = i; j < m_vecSellShop.size(); ++j)
				{
					m_vecSellShop[j]->Set_Pos(m_vecSellShop[j]->Get_Info().vPos.x,
						m_vecSellShop[j]->Get_Info().vPos.y - 25.f); // 재정렬
				}
			}
		}
	}
	return NO_EVENT;
}

void CShopWindow::LateUpdate()
{
	if (m_bIsClose)
		return;

	POINT pt = {};

	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tRect, pt))
	{
		dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->GetObject(PLAYER))->Set_Move(false);
		m_bIsClose = false;
	}

	if (m_iFrame == 0) // 구매
	{
		for (auto& pItem : m_vecBuyShop)
			pItem->LateUpdate();

		for (auto& pItem : m_vecReady)
		{
			pItem->LateUpdate();
		}
	}

	if (m_iFrame == 1) // 판매
	{
		for (auto& pItem : m_vecSellShop)
			pItem->LateUpdate();
	}

	m_pCloseBtn->LateUpdate();
	m_pBuyBtn->LateUpdate();
	m_pSellBtn->LateUpdate();
	m_pOkBtn->LateUpdate();
	m_pCancelBtn->LateUpdate();
	if (dynamic_cast<CBuyBtn*>(m_pBuyBtn)->Get_Click())
	{
		m_iFrame = 0;
		dynamic_cast<CSellBtn*>(m_pSellBtn)->Set_Click(false);
	}

	if (dynamic_cast<CSellBtn*>(m_pSellBtn)->Get_Click())
	{
		m_iFrame = 1;
		dynamic_cast<CBuyBtn*>(m_pBuyBtn)->Set_Click(false);
	}
	m_bIsClose = dynamic_cast<CCloseBtn*>(m_pCloseBtn)->Get_Click();
	
	if (dynamic_cast<COKBtn*>(m_pOkBtn)->Get_Click()) // 구매버튼 누를시
	{
		CGameObject* pInven = dynamic_cast<CBtnLst*>(CObjectMgr::GetInstance()->GetObjectW(CGameObject::BUTTON))->Get_Inven();
		int iPrice = 0; // 계산할 가격
		for (auto& pItem : m_vecReady)
		{
			iPrice = dynamic_cast<CItem*>(pItem)->Get_Item().iPrice *
				dynamic_cast<CItem*>(pItem)->Get_Item().iCount;

			dynamic_cast<CInven*>(pInven)->CreateItem(
				dynamic_cast<CItem*>(pItem)->Get_Item(),
				dynamic_cast<CItem*>(pItem)->Get_StateKey());

			CreateSellShop(dynamic_cast<CItem*>(pItem)->Get_Item(),
				dynamic_cast<CItem*>(pItem)->Get_StateKey()); // 판매상점에도 추가.

			dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->
				GetObjectW(CGameObject::PLAYER))->Set_Seed(-(iPrice));

		}

		for_each(m_vecReady.begin(), m_vecReady.end(), SafeDelete<CGameObject*>);
		m_vecReady.clear();
		m_vecReady.shrink_to_fit();
		m_iTotalPrice = 0;
	}
	else if (dynamic_cast<CCancelBtn*>(m_pCancelBtn)->Get_Click()) // 취소버튼누를시
	{
		for_each(m_vecReady.begin(), m_vecReady.end(), SafeDelete<CGameObject*>);
		m_vecReady.clear();
		m_vecReady.shrink_to_fit();
		m_iTotalPrice = 0;
	}
}

void CShopWindow::Render()
{
	if (m_bIsClose)
		return;

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_strObjKey,
		m_iFrame,
		m_strStateKey);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	D3DXMatrixTranslation(&m_tInfo.matWorld, 
		m_tInfo.vPos.x, 
		m_tInfo.vPos.y, 
		0.f);

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pCloseBtn->Render();
	m_pBuyBtn->Render();
	m_pSellBtn->Render();
	m_pOkBtn->Render();
	m_pCancelBtn->Render();
	Render_Shop();
	Render_Price();

}

void CShopWindow::LineRender()
{

}

HRESULT CShopWindow::Initialize()
{
	m_strObjKey = L"Interface";
	m_strStateKey = L"ShopWindow";
	m_iFrame = 0;
	m_iTotalPrice = 0;
	m_tInfo.vPos = { WINCX * 0.5f, WINCY * 0.5f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_tInfo.fCX = 401.f;
	m_tInfo.fCY = 421.f;

	m_eObjType = CGameObject::UI;
	m_eGroupID = GroupID::UI;

	InitButton(); // 버튼 만들기 초기화.
	InitSellShop(); // 판매상점 초기화
	return S_OK;
}

void CShopWindow::Release()
{
	SafeDelete(m_pCloseBtn);
	SafeDelete(m_pBuyBtn);
	SafeDelete(m_pSellBtn);
	SafeDelete(m_pOkBtn);
	SafeDelete(m_pCancelBtn);

	for_each(m_vecBuyShop.begin(), m_vecBuyShop.end(), SafeDelete<CGameObject*>);
	m_vecBuyShop.clear();
	m_vecBuyShop.shrink_to_fit();

	for_each(m_vecSellShop.begin(), m_vecSellShop.end(), SafeDelete<CGameObject*>);
	m_vecSellShop.clear();
	m_vecSellShop.shrink_to_fit();

	for_each(m_vecReady.begin(), m_vecReady.end(), SafeDelete<CGameObject*>);
	m_vecReady.clear();
	m_vecReady.shrink_to_fit();
}

CShopWindow * CShopWindow::Create(const int& iType)
{
	CShopWindow* pInstance = new CShopWindow;

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}

	switch (iType)
	{
	case 1:
		pInstance->InitBuyWeaponShop();
		break;
	case 2:
		pInstance->InitBuyPotionShop();
		break;
	default:
		break;
	}

	return pInstance;
}

void CShopWindow::Render_Shop()
{
	if (0 == m_iFrame)
	{
		wstring wstrName = L"";
		wstring wstrPrice = L"";
		wstring wstrCount = L"";
		int iPrice = 0;
		int i = 0;
		for (auto& pItem : m_vecBuyShop) // 구매상점
		{
			pItem->Render();
			wstrName = dynamic_cast<CItem*>(pItem)->Get_Item().strName;
			iPrice = dynamic_cast<CItem*>(pItem)->Get_Item().iPrice;
			wstrPrice = to_wstring(iPrice);
			D3DXMatrixTranslation(&m_tInfo.matWorld, // 이름
				m_tInfo.vPos.x - m_tInfo.fCX * 0.5f + 130.f,
				m_tInfo.vPos.y - m_tInfo.fCY * 0.5f + 60.f + (i * 25.f),
				0.f);

			m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
			m_pGraphicDev->GetFont()->DrawTextW(CGraphicDev::GetInstance()->GetSprite(),
				wstrName.c_str(), wstrName.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

			D3DXMatrixTranslation(&m_tInfo.matWorld, // 가격
				m_tInfo.vPos.x - m_tInfo.fCX * 0.5f + 320.f,
				m_tInfo.vPos.y - m_tInfo.fCY * 0.5f + 60.f + (i * 25.f),
				0.f);

			m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
			m_pGraphicDev->GetFont()->DrawTextW(CGraphicDev::GetInstance()->GetSprite(),
				wstrPrice.c_str(), wstrPrice.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

			i++;
		}

		i = 0;
		for (auto& pItem : m_vecReady) // 구매를 기다리는 상점.
		{
			pItem->Render();
			wstrName = dynamic_cast<CItem*>(pItem)->Get_Item().strName;
			iPrice = dynamic_cast<CItem*>(pItem)->Get_Item().iPrice *
				dynamic_cast<CItem*>(pItem)->Get_Item().iCount;

			wstrCount = L" X " + to_wstring(dynamic_cast<CItem*>(pItem)->Get_Item().iCount);
			wstrPrice = to_wstring(iPrice);
			D3DXMatrixTranslation(&m_tInfo.matWorld, // 이름
				m_tInfo.vPos.x - m_tInfo.fCX * 0.5f + 130.f,
				m_tInfo.vPos.y - m_tInfo.fCY * 0.5f + 265.f + (i * 25.f),
				0.f);

			m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
			m_pGraphicDev->GetFont()->DrawTextW(CGraphicDev::GetInstance()->GetSprite(),
				wstrName.c_str(), wstrName.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

			D3DXMatrixTranslation(&m_tInfo.matWorld, // 가격
				m_tInfo.vPos.x - m_tInfo.fCX * 0.5f + 320.f,
				m_tInfo.vPos.y - m_tInfo.fCY * 0.5f + 265.f + (i * 25.f),
				0.f);

			m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
			m_pGraphicDev->GetFont()->DrawTextW(CGraphicDev::GetInstance()->GetSprite(),
				wstrPrice.c_str(), wstrPrice.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

			if (dynamic_cast<CItem*>(pItem)->Get_Item().eType != ITEM_TYPE::WEAPON)
			{
				D3DXMatrixTranslation(&m_tInfo.matWorld, // 가격
					m_tInfo.vPos.x - m_tInfo.fCX * 0.5f + 235.f,
					m_tInfo.vPos.y - m_tInfo.fCY * 0.5f + 265.f + (i * 25.f),
					0.f);

				m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
				m_pGraphicDev->GetFont()->DrawTextW(CGraphicDev::GetInstance()->GetSprite(),
					wstrCount.c_str(), wstrCount.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			i++;
		}
	}
	else if (1 == m_iFrame)
	{
		wstring wstrName = L"";
		wstring wstrCount = L"";
		int i = 0;
		for (auto& pItem : m_vecSellShop)
		{
			pItem->Render();
			wstrName = dynamic_cast<CItem*>(pItem)->Get_Item().strName;
			wstrCount = to_wstring(dynamic_cast<CItem*>(pItem)->Get_Item().iCount);

			D3DXMatrixTranslation(&m_tInfo.matWorld, // 이름
				m_tInfo.vPos.x - m_tInfo.fCX * 0.5f + 130.f,
				m_tInfo.vPos.y - m_tInfo.fCY * 0.5f + 70.f + (i * 25.f),
				0.f);

			m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
			m_pGraphicDev->GetFont()->DrawTextW(CGraphicDev::GetInstance()->GetSprite(),
				wstrName.c_str(), wstrName.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

			D3DXMatrixTranslation(&m_tInfo.matWorld, // 카운트
				m_tInfo.vPos.x - m_tInfo.fCX * 0.5f + 330.f,
				m_tInfo.vPos.y - m_tInfo.fCY * 0.5f + 70.f + (i * 25.f),
				0.f);

			m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
			m_pGraphicDev->GetFont()->DrawTextW(CGraphicDev::GetInstance()->GetSprite(),
				wstrCount.c_str(), wstrCount.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

			i++;
		}
	}
}

void CShopWindow::Render_Price()
{
	wstring wstrPrice = L"";
	wstring wstrSeed = L"";

	wstrPrice = to_wstring(m_iTotalPrice);
	wstrSeed = to_wstring(dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->GetObjectW(CGameObject::PLAYER))->Get_Seed());

	D3DXMatrixTranslation(&m_tInfo.matWorld, // 카운트
		m_tInfo.vPos.x - m_tInfo.fCX * 0.5f + 120.f,
		m_tInfo.vPos.y - m_tInfo.fCY * 0.5f + 400.f,
		0.f);

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetFont()->DrawTextW(CGraphicDev::GetInstance()->GetSprite(),
		wstrPrice.c_str(), wstrPrice.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixTranslation(&m_tInfo.matWorld, // 카운트
		m_tInfo.vPos.x - m_tInfo.fCX * 0.5f + 100.f,
		m_tInfo.vPos.y - m_tInfo.fCY * 0.5f + 375.f,
		0.f);

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetFont()->DrawTextW(CGraphicDev::GetInstance()->GetSprite(),
		wstrSeed.c_str(), wstrSeed.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));


}

void CShopWindow::InitButton()
{
	m_pCloseBtn = CCloseBtn::Create(D3DXVECTOR3(m_tInfo.vPos.x + m_tInfo.fCX * 0.5f,
		m_tInfo.vPos.y - m_tInfo.fCY * 0.5f, 0.f));

	m_pBuyBtn = CBuyBtn::Create(D3DXVECTOR3(m_tInfo.vPos.x - m_tInfo.fCX * 0.5f,
		m_tInfo.vPos.y - m_tInfo.fCY * 0.5f, 0.f));

	m_pSellBtn = CSellBtn::Create(D3DXVECTOR3(m_tInfo.vPos.x - m_tInfo.fCX * 0.5f,
		m_tInfo.vPos.y - m_tInfo.fCY * 0.5f, 0.f));

	m_pOkBtn = COKBtn::Create(m_tInfo.vPos);
	m_pCancelBtn = CCancelBtn::Create(m_tInfo.vPos);
}

void CShopWindow::InitBuyWeaponShop()
{
	ITEM tItem = {};
	tItem.eLocation = BUYSHOP;
	tItem.eType = WEAPON;
	tItem.iCount = 1;
	tItem.iPrice = 1000;
	tItem.iValue = 100;
	tItem.strName = L"블러드 소드";
	CGameObject* pItem = CItem::Create(tItem, D3DXVECTOR3(m_tInfo.vPos.x - m_tInfo.fCX * 0.5f + 45.f,
		m_tInfo.vPos.y - m_tInfo.fCY * 0.5f + 65.f, 0.f), L"BloodSword");

	m_vecBuyShop.emplace_back(pItem);
}

void CShopWindow::InitBuyPotionShop()
{
	ITEM tItem = {};
	tItem.eLocation = BUYSHOP;
	tItem.eType = HP;
	tItem.iCount = 1;
	tItem.iPrice = 100;
	tItem.iValue = 10;
	tItem.strName = L"힐링 포션(대)";
	CGameObject* pItem = CItem::Create(tItem, D3DXVECTOR3(m_tInfo.vPos.x - m_tInfo.fCX * 0.5f + 45.f,
		m_tInfo.vPos.y - m_tInfo.fCY * 0.5f + 65.f, 0.f), L"RedPotion");

	m_vecBuyShop.emplace_back(pItem);

	tItem = {};
	tItem.eLocation = BUYSHOP;
	tItem.eType = MP;
	tItem.iCount = 1;
	tItem.iPrice = 100;
	tItem.iValue = 10;
	tItem.strName = L"그린 허브";

	pItem = CItem::Create(tItem, D3DXVECTOR3(m_tInfo.vPos.x - m_tInfo.fCX * 0.5f + 45.f,
		m_tInfo.vPos.y - m_tInfo.fCY * 0.5f + 90.f, 0.f), L"GreenHub");

	m_vecBuyShop.emplace_back(pItem);
}

void CShopWindow::InitSellShop()
{
	CGameObject* pInven = dynamic_cast<CBtnLst*>(CObjectMgr::GetInstance()->GetObjectW(CGameObject::BUTTON))->Get_Inven();
	vector<CGameObject*> vecItem = dynamic_cast<CInven*>(pInven)->Get_Inven();

	//if (!m_vecSellShop.empty()) // 재조정할때 사용.
	//{
	//	for_each(m_vecSellShop.begin(), m_vecSellShop.end(), SafeDelete<CGameObject*>);
	//	m_vecSellShop.clear();
	//	m_vecSellShop.shrink_to_fit();
	//}
	for_each(m_vecSellShop.begin(), m_vecSellShop.end(), SafeDelete<CGameObject*>);
	m_vecSellShop.clear();
	m_vecSellShop.shrink_to_fit();
	for (auto& pItem : vecItem)
	{
		CreateSellShop(dynamic_cast<CItem*>(pItem)->Get_Item(),
			pItem->Get_StateKey()); // 판매상점 만들기.
	}
}
