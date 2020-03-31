#include "stdafx.h"
#include "BtnLst.h"
#include "StatusBtn.h"
#include "EquipBtn.h"
#include "InvenBtn.h"
CBtnLst::CBtnLst()
{
}


CBtnLst::~CBtnLst()
{
	Release();
}

CGameObject * CBtnLst::Get_Equip()
{
	return dynamic_cast<CEquipBtn*>(m_pEquipBtn)->Get_Equip();
}

CGameObject * CBtnLst::Get_Inven()
{
	if (nullptr == dynamic_cast<CInvenBtn*>(m_pInvenBtn)->Get_Inven())
		return nullptr;
	return dynamic_cast<CInvenBtn*>(m_pInvenBtn)->Get_Inven();
}

int CBtnLst::Update()
{
	for (auto& pBtn : m_BtnLst)
		pBtn->Update();


	return NO_EVENT;
}

void CBtnLst::LateUpdate()
{
	for (auto& pBtn : m_BtnLst)
		pBtn->LateUpdate();
}

void CBtnLst::Render()
{
	for (auto& pBtn : m_BtnLst)
	{
		pBtn->Render();
	}
}

void CBtnLst::LineRender()
{
	for (auto& pBtn : m_BtnLst)
	{
		pBtn->LineRender();
	}
}

HRESULT CBtnLst::Initialize()
{
	m_pStateBtn = CStatusBtn::Create();
	m_BtnLst.emplace_back(m_pStateBtn);

	m_pEquipBtn = CEquipBtn::Create();
	m_BtnLst.emplace_back(m_pEquipBtn);

	m_pInvenBtn = CInvenBtn::Create();
	m_BtnLst.emplace_back(m_pInvenBtn);

	m_eGroupID = GroupID::UI;

	// 벡터 형식으로 하나씩 추가.
	return S_OK;
}

void CBtnLst::Release()
{
	for_each(m_BtnLst.begin(), m_BtnLst.end(), SafeDelete<CGameObject*>);

	m_BtnLst.clear();

	m_pStateBtn = nullptr;
	m_pEquipBtn = nullptr;
	m_pInvenBtn = nullptr;
}

HRESULT CBtnLst::Create()
{
	CBtnLst* pInstance = new CBtnLst;

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return E_FAIL;
	}

	HRESULT hr = CObjectMgr::GetInstance()->AddObject(CGameObject::BUTTON, pInstance);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	return S_OK;
}
