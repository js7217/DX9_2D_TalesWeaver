#include "stdafx.h"
#include "SingleTexture.h"


CSingleTexture::CSingleTexture()
	: m_pTexInfo(nullptr)
{
}


CSingleTexture::~CSingleTexture()
{
	Release();
}

const TEX_INFO * CSingleTexture::GetTexInfo(
	const wstring& wstrStateKey, 
	const DWORD& dwIndex) const
{
	return m_pTexInfo;
}

HRESULT CSingleTexture::InsertTexture(
	const wstring& wstrFilePath, 
	const wstring & wstrStateKey/* = L""*/, 
	const DWORD & dwCnt/* = 0*/)
{
	m_pTexInfo = new TEX_INFO;
	ZeroMemory(m_pTexInfo, sizeof(TEX_INFO));

	// 이미지 파일로부터 이미지 정보를 얻어오는 함수.
	if (FAILED(D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &(m_pTexInfo->tImgInfo))))
	{
		TCHAR szBuf[256] = L"";

		lstrcpy(szBuf, wstrFilePath.c_str());
		lstrcat(szBuf, L"Get ImageInfo Failed");
		ERR_MSG(szBuf);

		Release();

		return E_FAIL;
	}

	// 이미지 파일을 읽어들여서 IDirect3DTexture9를 할당.
	if (FAILED(D3DXCreateTextureFromFileEx(
		CGraphicDev::GetInstance()->GetDevice(),
		wstrFilePath.c_str(), /* 이미지 경로 */
		m_pTexInfo->tImgInfo.Width, 
		m_pTexInfo->tImgInfo.Height,
		m_pTexInfo->tImgInfo.MipLevels, 
		0,
		m_pTexInfo->tImgInfo.Format, 
		D3DPOOL_MANAGED, /* 불러들인 이미지를 어떤 메모리에 보관할 것인지 */
		D3DX_DEFAULT, /* 외곽선 테두리를 어떻게 표현할까? 지금은 장치가 알아서! */
		D3DX_DEFAULT,
		0, nullptr, nullptr,
		&(m_pTexInfo->pTexture))))
	{
		TCHAR szBuf[256] = L"";

		lstrcpy(szBuf, wstrFilePath.c_str());
		lstrcat(szBuf, L"Create Texture Failed");
		ERR_MSG(szBuf);

		Release();

		return E_FAIL;
	}

	return S_OK;
}

void CSingleTexture::Release()
{
	if (m_pTexInfo)
	{
		SafeRelease(m_pTexInfo->pTexture);
		SafeDelete(m_pTexInfo);
	}
}
