#include "stdafx.h"
#include "Equip.h"
#include "UIObserver.h"
#include "CloseBtn.h"
#include "Player.h"
#include "Item.h"
CEquip::CEquip()
{
}

CEquip::CEquip(const D3DXVECTOR3 & vPos)
	: m_pDataSubject(CDataSubject::GetInstance())
{
	m_tInfo.fCX = 195.f;
	m_tInfo.fCY = 277.f;

	m_tInfo.vPos.x = vPos.x - m_tInfo.fCX; // 이때 vPos는 Right Top
	m_tInfo.vPos.y = vPos.y;
	m_tInfo.vPos.z = 0.f;

	m_pCloseBtn = CCloseBtn::Create(vPos);
}


CEquip::~CEquip()
{
	Release();
}

void CEquip::CreateItem(ITEM tItem, const wstring & wstrStateKey)
{ // 장비는 30단위(첫 아이콘 : 20,40)
	CGameObject* pEquip = nullptr;
	tItem.eLocation = EQUIP;
	tItem.iCount = 1;
	switch (tItem.eType)
	{
	case ITEM_TYPE::WEAPON:
		pEquip = CItem::Create(tItem,
				D3DXVECTOR3(m_tInfo.vPos.x + 20, m_tInfo.vPos.y + 100, 0.f),
				wstrStateKey);
		dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()
			->GetObjectW(CGameObject::PLAYER))->ChangeWeapon(wstrStateKey);
		break;
	default:
		break;
	}

	m_vecEquip.emplace_back(pEquip);
}

int CEquip::Update()
{
	if (m_bIsClose)
		return NO_EVENT;
	CGameObject::LateInit();
	CGameObject::UpdateRect(m_tInfo.fCX * 0.5f, m_tInfo.fCY * 0.5f);
	m_pCloseBtn->Update();

	for (int i = 0; i < m_vecEquip.size(); ++i)
	{
		int iEvent = m_vecEquip[i]->Update();

		if (DEAD_OBJ == iEvent)
		{ // 장비에선 삭제하고 인벤쪽에 Create를 해야함.
			if (dynamic_cast<CItem*>(m_vecEquip[i])->Get_Item().eType == ITEM_TYPE::WEAPON)
			{
				dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()
					->GetObjectW(CGameObject::PLAYER))->ChangeWeapon(L"Sword");
			}
			SafeDelete(m_vecEquip[i]);
			m_vecEquip.erase(m_vecEquip.begin() + i);
		}
	}
	return NO_EVENT;
}

void CEquip::LateUpdate()
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
	else
	{
		m_bIsClose = true;
	}

	m_pCloseBtn->LateUpdate();
	for (auto& pItem : m_vecEquip)
		pItem->LateUpdate();

	m_bIsClose = dynamic_cast<CCloseBtn*>(m_pCloseBtn)->Get_Click();
}

void CEquip::Render()
{
	if (m_bIsClose)
		return;

	CGameObject::UpdateRect(m_tInfo.fCX * 0.5f, m_tInfo.fCY * 0.5f);
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_strObjKey,
		0,
		m_strStateKey);
	NULL_CHECK(pTexInfo);

	D3DXMatrixTranslation(&m_tInfo.matWorld, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pCloseBtn->Render();
	wstring wstrName = L"";
	int i = 0;
	for (auto& pItem : m_vecEquip)
	{
		pItem->Render();
		wstrName = dynamic_cast<CItem*>(pItem)->Get_Item().strName;

		D3DXMatrixTranslation(&m_tInfo.matWorld, // 이름
			pItem->Get_Info().vPos.x + 60.f,
			pItem->Get_Info().vPos.y - 7.f,
			0.f);

		m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pGraphicDev->GetFont()->DrawTextW(CGraphicDev::GetInstance()->GetSprite(),
			wstrName.c_str(), wstrName.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

}

void CEquip::LineRender()
{
	for (auto& pItem : m_vecEquip)
	{
		pItem->LineRender();
	}
}

HRESULT CEquip::Initialize()
{
	m_pObserver = CUIObserver::Create();
	m_pDataSubject->Subcribe(m_pObserver);

	m_strObjKey = L"Interface";
	m_strStateKey = L"Equip";

	m_eObjType = CGameObject::UI;

	return S_OK;
}

HRESULT CEquip::LateInit()
{
	return S_OK;
}

void CEquip::Release()
{
	for_each(m_vecEquip.begin(), m_vecEquip.end(), SafeDelete<CGameObject*>);
	m_vecEquip.clear();
	m_vecEquip.shrink_to_fit();

	SafeDelete(m_pCloseBtn);
	m_pDataSubject->UnSubcribe(m_pObserver);
	SafeDelete(m_pObserver);
}

CEquip * CEquip::Create(const D3DXVECTOR3 & vPos)
{
	CEquip* pInstance = new CEquip(vPos);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}

	return pInstance;
}
