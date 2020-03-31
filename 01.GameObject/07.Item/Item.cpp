#include "stdafx.h"
#include "Item.h"

#include "Player.h"
#include "BtnLst.h"
#include "Equip.h"
#include "Inven.h"
CItem::CItem()
{
}


CItem::~CItem()
{
	Release();
}

int CItem::Update()
{
	if (m_tItem.iCount <= 0)
		return DEAD_OBJ;

	CGameObject::UpdateRect();
	return NO_EVENT;
}

void CItem::LateUpdate()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tRect, pt))
	{
		if (CKeyMgr::GetInstance()->KeyUp(KEY_RBUTTON))
		{
			switch (m_tItem.eLocation)
			{
			case INVEN:
				ClickInven();
				break;
			case EQUIP:
				ClickEquip();
				break;
			case BUYSHOP:
				ClickBuyShop();
				break;
			case SELLSHOP:
				ClickSellShop();
				break;
			default:
				break;
			}
		}
	}
}

void CItem::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_strObjKey,
		0,
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

}

void CItem::LineRender()
{
	CGameObject::UpdateRect();

	D3DXVECTOR2 vec[5] =
	{
		{ (float)m_tRect.left, (float)m_tRect.top },
		{ (float)m_tRect.left, (float)m_tRect.bottom },
		{ (float)m_tRect.right, (float)m_tRect.bottom },
		{ (float)m_tRect.right, (float)m_tRect.top },
		{ (float)m_tRect.left , (float)m_tRect.top }
	};

	m_pGraphicDev->GetLine()->Draw(vec, 5, D3DCOLOR_ARGB(255, 255, 0, 0));
}

HRESULT CItem::Initialize()
{
	m_strObjKey = L"Item";
	m_tInfo.fCX = 24.f;
	m_tInfo.fCY = 24.f;

	m_eObjType = CGameObject::UI;
	m_eGroupID = GroupID::UI;

	return S_OK;
}

void CItem::Release()
{

}

void CItem::ClickInven()
{
	switch (m_tItem.eType)
	{
	case WEAPON: // 인벤에서 클릭시 장비로 감
	{
		m_tItem.iCount--;
		CGameObject* pEquip = dynamic_cast<CBtnLst*>(CObjectMgr::GetInstance()->GetObjectW(CGameObject::BUTTON))->Get_Equip(); // 장비창 위치알아옴
		dynamic_cast<CEquip*>(pEquip)->CreateItem(m_tItem, m_strStateKey);
		dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->GetObjectW(CGameObject::PLAYER))
			->Set_Att(m_tItem.iValue);
	}
	break;
	case HP:
		CSoundMgr::GetInstance()->StopSound(CSoundMgr::POTION);
		m_tItem.iCount--;
		dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->GetObjectW(CGameObject::PLAYER))
			->Set_HPHeal(m_tItem.iValue);
		CSoundMgr::GetInstance()->MyPlaySound(L"PotionUse.wav", CSoundMgr::POTION);
		break;
	case MP:
		CSoundMgr::GetInstance()->StopSound(CSoundMgr::POTION);
		m_tItem.iCount--;
		dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->GetObjectW(CGameObject::PLAYER))
			->Set_MPHeal(m_tItem.iValue);
		CSoundMgr::GetInstance()->MyPlaySound(L"PotionUse.wav", CSoundMgr::POTION);
		break;
	default:
		break;
	}
}

void CItem::ClickEquip()
{
	switch (m_tItem.eType)
	{
	case WEAPON:
	{
		m_tItem.iCount--;
		CGameObject* pInven = dynamic_cast<CBtnLst*>(CObjectMgr::GetInstance()->GetObjectW(CGameObject::BUTTON))->Get_Inven(); // 장비창 위치알아옴
		dynamic_cast<CInven*>(pInven)->CreateItem(m_tItem, m_strStateKey);
		dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->GetObjectW(CGameObject::PLAYER))
			->Set_Att(-m_tItem.iValue);
	}
	break;
	default:
		break;
	}
}

void CItem::ClickSellShop()
{
	m_tItem.iCount--;
	dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->GetObjectW(CGameObject::PLAYER))
		->Set_Seed(int(m_tItem.iPrice * 0.7f));
}

void CItem::ClickBuyShop()
{
	m_tItem.eLocation = READY;
}

CItem * CItem::Create(ITEM tItem, D3DXVECTOR3 vPos, const wstring& wstrStateKey)
{
	CItem* pInstance = new CItem;

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}

	pInstance->Set_Pos(vPos.x, vPos.y);
	pInstance->Set_Data(tItem);
	pInstance->Set_StateKey(wstrStateKey);

	return pInstance;
}
