#include "stdafx.h"
#include "GraphicDev.h"

IMPLEMENT_SINGLETON(CGraphicDev)

CGraphicDev::CGraphicDev()
{
}


CGraphicDev::~CGraphicDev()
{
	Release();
}

LPDIRECT3DDEVICE9 CGraphicDev::GetDevice()
{
	return m_pGraphicDev;
}

LPD3DXSPRITE CGraphicDev::GetSprite()
{
	return m_pSprite;
}

LPD3DXFONT CGraphicDev::GetFont()
{
	return m_pFont;
}

LPD3DXFONT CGraphicDev::GetDialogFont()
{
	return m_pDialogFont;
}

LPD3DXLINE CGraphicDev::GetLine()
{
	return m_pLine;
}

HRESULT CGraphicDev::InitDevice(HWND hWnd, 
	const DWORD & dwWinCX, 
	const DWORD & dwWinCY, 
	WIN_MODE eMode)
{
	// ��ġ �ʱ�ȭ
	// 1. ��ġ�� ������ ��ü ���� (IDirect3D9)
	// IDirect3D9 Com��ü�� ��ġ ������ IDirect3DDevice9 ��ü�� ������ �뵵�� ���.
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	if (nullptr == m_pSDK)
	{
		// AfxMessageBox: MFC���� �����ϴ� MessageBox �Լ�.
		ERR_MSG(L"Direct3DCreate9 Failed");
		return E_FAIL;
	}

	// 2. ��ġ�� ����(���� ����)
	D3DCAPS9	DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// GetDeviceCaps�Լ�: �׷��� ��ġ�� ���� ����(���� ����)�� ������ �Լ�.
	// D3DDEVTYPE_HAL: HAL�� ���� ��ġ�� ����.
	// *** HAL(Hardware Abstraction Layer, �ϵ���� �߻� ����)
	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		ERR_MSG(L"GetDeviceCaps Failed");
		return E_FAIL;
	}

	// 2-1. ������ ��ġ�� ���ؽ� ���μ��� ������ ����.
	// ���ؽ� ���μ���: ���� ��ȯ + ���� ó��.
	DWORD vp = 0;

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;	// �ϵ������ ���ؽ� ���μ����� ����Ѵ�.
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	// ���̷�Ʈ �󿡼� �����带 �����ϰ� ����ϵ��� �ϰڴ�.
	vp |= D3DCREATE_MULTITHREADED;

	// 3. ��ġ�� ������ ��ü ���� (IDirect3DDevice9)
	D3DPRESENT_PARAMETERS	d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = dwWinCX;
	d3dpp.BackBufferHeight = dwWinCY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	// ȭ�� ������ �ؼҸ� ���� "����ü��" ����� ���.
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = eMode;

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	
	// ��ü ȭ�� �� ���� ����� �ֻ����� ���� ��ġ�� �˾Ƽ� ����.
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		vp,
		&d3dpp,
		&m_pGraphicDev)))
	{
		ERR_MSG(L"CreateDevice Failed");
		return E_FAIL;
	}

	// ��������Ʈ Com��ü ����
	if (FAILED(D3DXCreateSprite(m_pGraphicDev, &m_pSprite)))
	{
		ERR_MSG(L"D3DXCreateSprite Failed");
		return E_FAIL;
	}

	// ��Ʈ Com��ü ����
	D3DXFONT_DESCW tFontInfo = {};
	tFontInfo.Width = 7;
	tFontInfo.Height = 12;
	tFontInfo.Weight = 0;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"���");

	if (FAILED(D3DXCreateFontIndirect(m_pGraphicDev, &tFontInfo, &m_pFont)))
	{
		ERR_MSG(L"D3DXCreateFontIndirect Failed");
		return E_FAIL;
	}

	tFontInfo = {};
	tFontInfo.Width = 10;
	tFontInfo.Height = 15;
	tFontInfo.Weight = 0;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"���");

	if (FAILED(D3DXCreateFontIndirect(m_pGraphicDev, &tFontInfo, &m_pDialogFont)))
	{
		ERR_MSG(L"D3DXCreateFontIndirect Failed");
		return E_FAIL;
	}


	if (FAILED(D3DXCreateLine(m_pGraphicDev, &m_pLine)))
	{
		ERR_MSG(L"D3DXCreateLine Failed");
		return E_FAIL;
	}
	m_pLine->SetWidth(3);

	return S_OK;
}

void CGraphicDev::Render_Begin()
{
	// ������ ����
	// 1. �ĸ���۸� ����.
	m_pGraphicDev->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);

	// 2. �ĸ���ۿ� �׸��⸦ �����Ѵ�.
	m_pGraphicDev->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CGraphicDev::Render_End_Sprite(HWND hWnd)
{
	m_pSprite->End();
}

void CGraphicDev::Render_End(HWND hWnd/* = nullptr*/)
{
	m_pGraphicDev->EndScene();

	// 4. �ĸ���۸� ������۷� ��ü�Ͽ� ȭ�鿡 ����Ѵ�.
	m_pGraphicDev->Present(nullptr, nullptr, hWnd, nullptr);
}

void CGraphicDev::Release()
{
	// ���� ���� ����.
	if (SafeRelease(m_pLine))
		ERR_MSG(L"m_pLine Release Failed");

	if (SafeRelease(m_pDialogFont))
		ERR_MSG(L"m_pDialogFont Release Failed");

	if (SafeRelease(m_pFont))
		ERR_MSG(L"m_pFont Release Failed");

	if(SafeRelease(m_pSprite))
		ERR_MSG(L"m_pSprite Release Failed");

	if(SafeRelease(m_pGraphicDev))
		ERR_MSG(L"m_pGraphicDev Release Failed");

	if (SafeRelease(m_pSDK))
		ERR_MSG(L"m_pSDK Release Failed");
}
