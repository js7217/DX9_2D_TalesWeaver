#pragma once
class CGraphicDev
{
	DECLARE_SINGLETON(CGraphicDev)

private:
	CGraphicDev();
	~CGraphicDev();

public:
	LPDIRECT3DDEVICE9 GetDevice();
	LPD3DXSPRITE GetSprite();
	LPD3DXFONT GetFont();
	LPD3DXFONT GetDialogFont();
	LPD3DXLINE GetLine();

public:
	HRESULT InitDevice(HWND hWnd,
		const DWORD& dwWinCX,
		const DWORD& dwWinCY,
		WIN_MODE eMode);
	void Render_Begin();
	void Render_End_Sprite(HWND hWnd = nullptr);
	void Render_End(HWND hWnd = nullptr);
private:
	void Release();	

private:
	LPDIRECT3D9			m_pSDK;	// ��ġ�� ������ Com��ü�� ������.
	LPDIRECT3DDEVICE9	m_pGraphicDev; // ��ġ�� ������ Com��ü�� ������.

	// DirectX���� 2D �̹����� �������ϰ� ���ִ� Com��ü�� ������
	LPD3DXSPRITE		m_pSprite;

	// DirectX���� �ؽ�Ʈ�� ����ϱ� ���� Com��ü�� ������. (Direct Font)
	LPD3DXFONT			m_pFont;
	LPD3DXFONT			m_pDialogFont;
	LPD3DXLINE			m_pLine;
	// COM (Component Object Model)

};

