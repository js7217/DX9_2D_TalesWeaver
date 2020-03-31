#include "stdafx.h"
#include "Inven.h"
#include "CloseBtn.h"
#include "Player.h"
#include "Item.h"
CInven::CInven()
{
}

CInven::CInven(const D3DXVECTOR3 & vPos)
	: m_fDist(25.f)
{
	m_tInfo.fCX = 345.f;
	m_tInfo.fCY = 400.f;

	m_tInfo.vPos.x = vPos.x - m_tInfo.fCX; // 이때 vPos는 Right Top
	m_tInfo.vPos.y = vPos.y;
	m_tInfo.vPos.z = 0.f;

	m_pCloseBtn = CCloseBtn::Create(vPos);
}


CInven::~CInven()
{
	Release();
}

void CInven::CreateItem(ITEM tItem, const wstring & wstrStateKey)
{
	// 소모품은 갯수를 증가시켜보자.
	CGameObject* pItem = nullptr;
	tItem.eLocation = INVEN;
	float fY = 55.f + m_vecItem.size() * m_fDist;

	if (tItem.eType == ITEM_TYPE::WEAPON)
	{
		tItem.iCount = 1;
		pItem = CItem::Create(tItem,
			D3DXVECTOR3(m_tInfo.vPos.x + 25.f, m_tInfo.vPos.y + fY, 0.f), wstrStateKey);

		m_vecItem.emplace_back(pItem);
	}
	else
	{
		if (m_vecItem.empty())
		{
			pItem = CItem::Create(tItem,
				D3DXVECTOR3(m_tInfo.vPos.x + 25.f, m_tInfo.vPos.y + fY, 0.f), wstrStateKey);

			m_vecItem.emplace_back(pItem);
		}
		else
		{
			int iSameCnt = 0;
			for (auto& Item : m_vecItem)
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

			if (0 == iSameCnt)
			{
				pItem = CItem::Create(tItem,
					D3DXVECTOR3(m_tInfo.vPos.x + 25.f, m_tInfo.vPos.y + fY, 0.f), wstrStateKey);

				m_vecItem.emplace_back(pItem);
			}
		}
	}
}

int CInven::Update()
{
	CGameObject::UpdateRect(m_tInfo.fCX * 0.5f, m_tInfo.fCY * 0.5f);
	CGameObject::LateInit();
	m_pCloseBtn->Update();

	for (int i = 0; i < m_vecItem.size(); ++i)
	{
		int iEvent = m_vecItem[i]->Update();

		if (DEAD_OBJ == iEvent)
		{
			SafeDelete(m_vecItem[i]);
			m_vecItem.erase(m_vecItem.begin() + i);
			for (int j = i; j < m_vecItem.size(); ++j)
			{
				m_vecItem[j]->Set_Pos(m_vecItem[j]->Get_Info().vPos.x, 
					m_vecItem[j]->Get_Info().vPos.y - 25.f); // 재정렬
			}
		}
	}
	return NO_EVENT;
}

void CInven::LateUpdate()
{
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
	for (auto& pItem : m_vecItem)
		pItem->LateUpdate();

	m_bIsClose = dynamic_cast<CCloseBtn*>(m_pCloseBtn)->Get_Click();
}

void CInven::Render()
{
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
	wstring wstrCount = L"";
	int i = 0;
	for (auto& pItem : m_vecItem)
	{
		pItem->Render();
		wstrName = dynamic_cast<CItem*>(pItem)->Get_Item().strName;
		wstrCount = to_wstring(dynamic_cast<CItem*>(pItem)->Get_Item().iCount);

		D3DXMatrixTranslation(&m_tInfo.matWorld, // 이름
			m_tInfo.vPos.x + 120.f,
			m_tInfo.vPos.y + 50.f + i * 25.f,
			0.f);

		m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pGraphicDev->GetFont()->DrawTextW(CGraphicDev::GetInstance()->GetSprite(),
			wstrName.c_str(), wstrName.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

		D3DXMatrixTranslation(&m_tInfo.matWorld, // 카운트
			m_tInfo.vPos.x + 290.f,
			m_tInfo.vPos.y + 50.f + i * 25.f,
			0.f);

		m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pGraphicDev->GetFont()->DrawTextW(CGraphicDev::GetInstance()->GetSprite(),
			wstrCount.c_str(), wstrCount.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	
		i++;
	}
}

void CInven::LineRender()
{
	for (auto& pItem : m_vecItem)
		pItem->LineRender();
}

HRESULT CInven::Initialize()
{
	m_strObjKey = L"Interface";
	m_strStateKey = L"Inven";

	m_tInfo.fCX = 345.f;
	m_tInfo.fCY = 400.f;

	m_eObjType = CGameObject::UI;

	return S_OK;
}

HRESULT CInven::LateInit()
{
	return S_OK;
}

void CInven::Release()
{
	for_each(m_vecItem.begin(), m_vecItem.end(), SafeDelete<CGameObject*>);
	m_vecItem.clear();
	m_vecItem.shrink_to_fit();

	SafeDelete(m_pCloseBtn);
}

CInven * CInven::Create(const D3DXVECTOR3 & vPos)
{
	CInven* pInstance = new CInven(vPos);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}

	return pInstance;
}
