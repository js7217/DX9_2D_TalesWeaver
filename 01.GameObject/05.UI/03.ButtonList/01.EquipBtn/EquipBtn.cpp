#include "stdafx.h"
#include "EquipBtn.h"
#include "Equip.h"
#include "Player.h"
CEquipBtn::CEquipBtn()
	: m_bIsClick(false)
	, m_bIsRender(false)
{
}


CEquipBtn::~CEquipBtn()
{
	Release();
}

int CEquipBtn::Update()
{
	CGameObject::UpdateRect(m_tInfo.fCX * 0.5f, m_tInfo.fCY * 0.5f);
	
	if (m_bIsRender)
		m_pEquip->Update();

	if (dynamic_cast<CEquip*>(m_pEquip)->Get_Close())
		m_bIsClick = false; // ���� �����찡 ���� ���¶�� Ŭ�� ���� false�� ������ش�.

	return NO_EVENT;
}

void CEquipBtn::LateUpdate()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tRect, pt))
	{
		m_iFrame = 1;
		m_bIsRender = true;
		dynamic_cast<CEquip*>(m_pEquip)->Set_Open();
		dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->GetObject(PLAYER))->Set_Move(false);
		if (m_pKeyMgr->KeyUp(KEY_LBUTTON))
		{
			if (!m_bIsClick) // �ش� ��ư�� Ŭ���ߴٸ� ������� ���ƾ���.
				m_bIsClick = true;
			else if (m_bIsClick)
				m_bIsClick = false;
			CSoundMgr::GetInstance()->MyPlaySound(L"wopen.mp3", CSoundMgr::EFFECT);
			return;
		}
	}
	else
	{
		m_iFrame = 0;
		if (!m_bIsClick)
			m_bIsRender = false;
	}

	if (m_bIsRender)
		m_pEquip->LateUpdate();
}

void CEquipBtn::Render()
{
	D3DXMatrixTranslation(&m_tInfo.matWorld, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	{
		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
			m_strObjKey,
			m_iFrame,
			m_strStateKey);

		NULL_CHECK(pTexInfo);

		m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (m_bIsRender)
		m_pEquip->Render();
}

void CEquipBtn::LineRender()
{
	if (m_bIsRender)
		m_pEquip->Render();
}

HRESULT CEquipBtn::Initialize()
{
	m_tInfo.fCX = 18.f;
	m_tInfo.fCY = 51.f;
	m_tInfo.vPos = { WINCX - m_tInfo.fCX * 1.5f, m_tInfo.fCY * 2.f, 0.f };

	m_strObjKey = L"Interface";
	m_strStateKey = L"EquipButton";

	m_iFrame = 0;

	m_pKeyMgr = CKeyMgr::GetInstance();

	m_pEquip = CEquip::Create(m_tInfo.vPos);

	m_eObjType = CGameObject::UI;

	return S_OK;
}

void CEquipBtn::Release()
{
	SafeDelete(m_pEquip);
}

CEquipBtn * CEquipBtn::Create()
{
	CEquipBtn* pInstance = new CEquipBtn;

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}

	return pInstance;
}
