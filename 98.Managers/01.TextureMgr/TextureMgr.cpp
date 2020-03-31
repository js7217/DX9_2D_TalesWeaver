#include "stdafx.h"
#include "TextureMgr.h"
#include "SingleTexture.h"
#include "MultiTexture.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr()
	: m_iCount(0)
{
}


CTextureMgr::~CTextureMgr()
{
	Release();
}

const TEX_INFO * CTextureMgr::GetTexInfo(
	const wstring & wstrObjKey, 
	const DWORD & dwIndex,
	const wstring & wstrStateKey, 
	const wstring& wstrDirectionKey) const
{
	auto iter_find = m_MapTexture.find(wstrObjKey);
	auto iter = iter_find->second.find(wstrStateKey);

	if (m_MapTexture.end() == iter_find)
		return nullptr;

	return iter->second->GetTexInfo(wstrDirectionKey, dwIndex);
}

const wstring & CTextureMgr::GetLoadingMessage() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_wstrLoadingMessage;
}

const int & CTextureMgr::GetLoadingCount() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_iCount;
}

void CTextureMgr::SetLoadingMessage(const wstring & wstrMessage)
{
	m_wstrLoadingMessage = wstrMessage;
}

HRESULT CTextureMgr::LoadFromImgPath(const wstring & wstrImgPath)
{
	wifstream fin;

	fin.open(wstrImgPath.c_str());

	if (fin.fail())
	{
		ERR_MSG(L"LoadFromImgPath Failed");
		return E_FAIL;
	}

	TCHAR szObjKey[MAX_STR] = L"";
	TCHAR szStateKey[MAX_STR] = L"";
	TCHAR szDirectionKey[MAX_STR] = L"";
	TCHAR szImgCount[MIN_STR] = L"";
	TCHAR szPath[MAX_STR] = L"";

	IMG_PATH* pImgPath = nullptr;
	int iCount = 0;
	HRESULT hr = 0;
	while (true)
	{
		fin.getline(szObjKey, MAX_STR, '|');
		fin.getline(szStateKey, MAX_STR, '|');
		fin.getline(szDirectionKey, MAX_STR, '|');
		fin.getline(szImgCount, MIN_STR, '|');
		fin.getline(szPath, MAX_STR);

		if (fin.eof())
			break;

		iCount = _wtoi(szImgCount);
		hr = InsertTexture(TEX_MULTI, szPath, szObjKey, szStateKey, iCount, szDirectionKey);
		FAILED_CHECK_MSG_RETURN(hr, L"LoadFromImgPath Failed", E_FAIL);
	}

	fin.close();

	return S_OK;
}

HRESULT CTextureMgr::InsertTexture(
	TEX_TYPE eTextureType, 
	const wstring & wstrFilePath, 
	const wstring & wstrObjKey, 
	const wstring & wstrStateKey, 
	const DWORD & dwCnt,
	const wstring& wstrDircetionKey)
{
	CTexture* pTexture = nullptr;

	auto iter_find = m_MapTexture.find(wstrObjKey); // 오브젝트키가 있는지 없는지.

	if (m_MapTexture.end() == iter_find)
	{
		switch (eTextureType)
		{
		case TEX_SINGLE:
			pTexture = new CSingleTexture;
			break;
		case TEX_MULTI:
			pTexture = new CMultiTexture;
			break;
		}

		if (FAILED(pTexture->InsertTexture(wstrFilePath, wstrDircetionKey, dwCnt)))
		{
			ERR_MSG(L"InsertTexture Failed");
			return E_FAIL;
		}

		m_MapTexture[wstrObjKey][wstrStateKey] = pTexture; // 오브젝트 키가 없을 경우 새로 만든다.
	}
	else if (TEX_MULTI == eTextureType)
	{
		auto iter_find_second = m_MapTexture[wstrObjKey].find(wstrStateKey);

		if (m_MapTexture[wstrObjKey].end() == iter_find_second)
		{
			pTexture = new CMultiTexture;

			if (FAILED(pTexture->InsertTexture(wstrFilePath, wstrDircetionKey, dwCnt)))
			{
				ERR_MSG(L"InsertTexture Failed");
				return E_FAIL;
			}

			m_MapTexture[wstrObjKey].insert({ wstrStateKey, pTexture });
		}

		else if (FAILED(m_MapTexture[wstrObjKey][wstrStateKey]->InsertTexture(wstrFilePath, wstrDircetionKey, dwCnt)))
		{
			ERR_MSG(L"InsertTexture Failed");
			return E_FAIL;
		}

		m_wstrLoadingMessage = wstrFilePath;
		m_iCount++;
	}

	return S_OK;
}

void CTextureMgr::Release()
{
	for_each(m_MapTexture.begin(), m_MapTexture.end(),
		[](auto& MyPair)
	{
		for_each(MyPair.second.begin(), MyPair.second.end(),
			[](auto& Pair)
		{
			SafeDelete(Pair.second);
		});
	});

	m_MapTexture.clear();
}
