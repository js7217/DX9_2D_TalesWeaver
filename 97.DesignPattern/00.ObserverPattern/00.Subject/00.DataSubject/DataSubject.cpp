#include "stdafx.h"
#include "DataSubject.h"

IMPLEMENT_SINGLETON(CDataSubject)

CDataSubject::CDataSubject()
{
}


CDataSubject::~CDataSubject()
{
	for (auto& MyPair : m_MapData)
		MyPair.second.clear();

	m_MapData.clear();
}

list<void*>* CDataSubject::GetDataLst(int iMessage)
{
	auto iter_find = m_MapData.find(iMessage);

	if (m_MapData.end() == iter_find)
		return nullptr;

	return &(iter_find->second);
}

void CDataSubject::AddData(int iMessage, void * pData)
{
	NULL_CHECK(pData);

	auto iter_find = find(m_MapData[iMessage].begin(), m_MapData[iMessage].end(), pData);

	if (m_MapData[iMessage].end() != iter_find)
		return;

	m_MapData[iMessage].push_back(pData);
	CSubject::Notify(iMessage, pData);
}

void CDataSubject::RemoveData(int iMessage, void * pData)
{
	NULL_CHECK(pData);

	auto iter_find = find(m_MapData[iMessage].begin(), m_MapData[iMessage].end(), pData);

	if (m_MapData[iMessage].end() == iter_find)
		return;

	m_MapData[iMessage].erase(iter_find);
}
