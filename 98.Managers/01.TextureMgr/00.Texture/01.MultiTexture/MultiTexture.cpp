#include "stdafx.h"
#include "MultiTexture.h"


CMultiTexture::CMultiTexture()
{
}


CMultiTexture::~CMultiTexture()
{
	Release();
}

const TEX_INFO * CMultiTexture::GetTexInfo(
	const wstring & wstrStateKey/* = L""*/, 
	const DWORD & dwIndex/* = 0*/) const
{
	auto iter_find = m_MapMultiTex.find(wstrStateKey);

	if(m_MapMultiTex.end() == iter_find)
		return nullptr;	

	return iter_find->second[dwIndex];
}

HRESULT CMultiTexture::InsertTexture(
	const wstring & wstrFilePath, 
	const wstring & wstrStateKey/* = L""*/, 
	const DWORD & dwCnt/* = 0*/)
{
	TEX_INFO* pTexInfo = nullptr;
	TCHAR szFullPath[MAX_STR] = L"";

	for (DWORD i = 0; i < dwCnt; ++i)
	{
		// szFullPath: L"../Texture/Stage/Terrain/Tile/Tile%d.png"
		lstrcpy(szFullPath, wstrFilePath.c_str());

		// szFullPath: L"../Texture/Stage/Terrain/Tile/Tile0.png"
		swprintf_s(szFullPath, szFullPath, i);

		pTexInfo = new TEX_INFO;
		ZeroMemory(pTexInfo, sizeof(TEX_INFO));

		// 이미지 파일로부터 이미지 정보를 얻어오는 함수.
		if (FAILED(D3DXGetImageInfoFromFile(szFullPath, &(pTexInfo->tImgInfo))))
		{
			TCHAR szBuf[256] = L"";

			lstrcpy(szBuf, szFullPath);
			lstrcat(szBuf, L"Get ImageInfo Failed");
			ERR_MSG(szBuf);

			Release();

			return E_FAIL;
		}

		// 이미지 파일을 읽어들여서 IDirect3DTexture9를 할당.
		if (FAILED(D3DXCreateTextureFromFileEx(
			CGraphicDev::GetInstance()->GetDevice(),
			szFullPath, /* 이미지 경로 */
			pTexInfo->tImgInfo.Width,
			pTexInfo->tImgInfo.Height,
			pTexInfo->tImgInfo.MipLevels,
			0,
			pTexInfo->tImgInfo.Format,
			D3DPOOL_MANAGED, /* 불러들인 이미지를 어떤 메모리에 보관할 것인지 */
			D3DX_DEFAULT, /* 외곽선 테두리를 어떻게 표현할까? 지금은 장치가 알아서! */
			D3DX_DEFAULT,
			0, nullptr, nullptr,
			&(pTexInfo->pTexture))))
		{
			TCHAR szBuf[256] = L"";

			lstrcpy(szBuf, szFullPath);
			lstrcat(szBuf, L"Create Texture Failed");
			ERR_MSG(szBuf);

			Release();

			return E_FAIL;
		}

		m_MapMultiTex[wstrStateKey].push_back(pTexInfo);
	}	

	return S_OK;
}

void CMultiTexture::Release()
{
	for_each(m_MapMultiTex.begin(), m_MapMultiTex.end(),
		[](auto& MyPair)
	{
		for_each(MyPair.second.begin(), MyPair.second.end(), 
			[](auto& pTexInfo)
		{
			SafeRelease(pTexInfo->pTexture);
			SafeDelete(pTexInfo);
		});

		MyPair.second.clear();
		MyPair.second.shrink_to_fit();
	});

	m_MapMultiTex.clear();
}
