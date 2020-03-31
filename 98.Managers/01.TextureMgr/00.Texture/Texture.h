#pragma once
class CTexture
{
public:
	CTexture();
	virtual ~CTexture();

public:
	virtual const TEX_INFO* GetTexInfo(
		const wstring& wstrStateKey = L"",
		const DWORD& dwIndex = 0) const PURE;

public:
	virtual HRESULT InsertTexture(
		const wstring& wstrFilePath, /* �̹��� ���. */
		const wstring& wstrStateKey = L"", /* ���� Ű�� ��Ƽ�ؽ��ĸ� ���´�. */
		const DWORD& dwCnt = 0 /* �̹��� ����, ��Ƽ �ؽ����� ��쿡�� ����� */ ) PURE;
	virtual void Release() PURE;
};

