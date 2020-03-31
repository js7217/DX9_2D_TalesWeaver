#include "stdafx.h"
#include "UIObserver.h"


CUIObserver::CUIObserver()
{
}


CUIObserver::~CUIObserver()
{
}

const int & CUIObserver::GetHp() const
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return m_iHp;
}

const int & CUIObserver::GetMp() const
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return m_iMp;
}

const float & CUIObserver::GetSp() const
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return m_fSp;
}

const int & CUIObserver::GetMaxHp() const
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return m_iMaxHp;
}

const int & CUIObserver::GetMaxMp() const
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return m_iMaxMp;
}

const float & CUIObserver::GetMaxSp() const
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return m_fMaxSp;
}

const float & CUIObserver::GetSeed() const
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return m_iSeed;
}

const float & CUIObserver::GetLevel() const
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return m_iLevel;
}

const float & CUIObserver::GetExp() const
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return m_iExp;
}

const float & CUIObserver::GetMaxExp() const
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return m_iMaxExp;
}

const D3DXMATRIX & CUIObserver::GetWorldMatrix() const
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return m_matWorld;
}

void CUIObserver::Update(int iMessage, void * pData)
{
	NULL_CHECK(pData);

	// Pull ������
	list<void*>* pDataLst = CDataSubject::GetInstance()->GetDataLst(iMessage);
	NULL_CHECK(pDataLst);

	auto& iter_find = find(pDataLst->begin(), pDataLst->end(), pData);

	if (pDataLst->end() == iter_find)
		return;

	switch (iMessage)
	{
	case CDataSubject::PLAYER_HP:
		m_iHp = *reinterpret_cast<int*>(*iter_find);
		break;
	case CDataSubject::PLAYER_MAXHP:
		m_iMaxHp = *reinterpret_cast<int*>(*iter_find);
		break;
	case CDataSubject::PLAYER_MP:
		m_iMp = *reinterpret_cast<int*>(*iter_find);
		break;
	case CDataSubject::PLAYER_MAXMP:
		m_iMaxMp = *reinterpret_cast<int*>(*iter_find);
		break;
	case CDataSubject::PLAYER_SP:
		m_fSp = *reinterpret_cast<float*>(*iter_find);
		break;
	case CDataSubject::PLAYER_MAXSP:
		m_fMaxSp = *reinterpret_cast<float*>(*iter_find);
		break;
	case CDataSubject::PLAYER_MATRIX:
		m_matWorld = *reinterpret_cast<D3DXMATRIX*>(*iter_find);
		break;
	case CDataSubject::PLAYER_SEED:
		m_iSeed = *reinterpret_cast<int*>(*iter_find);
		break;
	case CDataSubject::PLAYER_LEVEL:
		m_iLevel = *reinterpret_cast<int*>(*iter_find);
		break;
	case CDataSubject::PLAYER_EXP:
		m_iExp = *reinterpret_cast<int*>(*iter_find);
		break;
	case CDataSubject::PLAYER_MAXEXP:
		m_iMaxExp = *reinterpret_cast<int*>(*iter_find);
		break;
	}
}

CUIObserver * CUIObserver::Create()
{
	return new CUIObserver;
}
