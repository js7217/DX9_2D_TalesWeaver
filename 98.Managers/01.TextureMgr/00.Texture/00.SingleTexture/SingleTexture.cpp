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

	// �̹��� ���Ϸκ��� �̹��� ������ ������ �Լ�.
	if (FAILED(D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &(m_pTexInfo->tImgInfo))))
	{
		TCHAR szBuf[256] = L"";

		lstrcpy(szBuf, wstrFilePath.c_str());
		lstrcat(szBuf, L"Get ImageInfo Failed");
		ERR_MSG(szBuf);

		Release();

		return E_FAIL;
	}

	// �̹��� ������ �о�鿩�� IDirect3DTexture9�� �Ҵ�.
	if (FAILED(D3DXCreateTextureFromFileEx(
		CGraphicDev::GetInstance()->GetDevice(),
		wstrFilePath.c_str(), /* �̹��� ��� */
		m_pTexInfo->tImgInfo.Width, 
		m_pTexInfo->tImgInfo.Height,
		m_pTexInfo->tImgInfo.MipLevels, 
		0,
		m_pTexInfo->tImgInfo.Format, 
		D3DPOOL_MANAGED, /* �ҷ����� �̹����� � �޸𸮿� ������ ������ */
		D3DX_DEFAULT, /* �ܰ��� �׵θ��� ��� ǥ���ұ�? ������ ��ġ�� �˾Ƽ�! */
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
