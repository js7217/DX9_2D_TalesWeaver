#include "stdafx.h"
#include "SkillIcon.h"
#include "Player.h"

CSkillIcon::CSkillIcon()
{
}


CSkillIcon::~CSkillIcon()
{
}

int CSkillIcon::Update()
{
	CGameObject::LateInit();

	CGameObject::UpdateRect();
	return 0;
}

void CSkillIcon::LateUpdate()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tRect, pt))
	{
		dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->GetObject(PLAYER))->Set_Move(false);
		if (CKeyMgr::GetInstance()->KeyPressing(KEY_LBUTTON))
		{
			m_tInfo.vPos = D3DXVECTOR3((float)pt.x, (float)pt.y, 0.f);
		}
	}
	else
	{
		m_tInfo.vPos = m_vPrePos;
	}
}

void CSkillIcon::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_strObjKey,
		m_iFrame);
	NULL_CHECK(pTexInfo);

	D3DXMatrixTranslation(&m_tInfo.matWorld,
		m_tInfo.vPos.x,
		m_tInfo.vPos.y, 0.f);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CSkillIcon::LineRender()
{
}

HRESULT CSkillIcon::Initialize()
{
	m_strObjKey = L"SkillIcon";

	m_tInfo.fCX = 24.f;
	m_tInfo.fCY = 24.f;

	m_eGroupID = GroupID::ICON;

	return S_OK;
}

HRESULT CSkillIcon::LateInit()
{
	m_vPrePos = m_tInfo.vPos;

	return S_OK;
}

void CSkillIcon::Release()
{
}

HRESULT CSkillIcon::Create(float fX, float fY, const int& iFrame)
{
	CSkillIcon* pInstance = new CSkillIcon;

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return E_FAIL;
	}

	pInstance->Set_Pos(fX, fY);
	pInstance->Set_Frame(iFrame);

	HRESULT hr = CObjectMgr::GetInstance()->AddObject(CGameObject::UI, pInstance);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	return S_OK;
}
