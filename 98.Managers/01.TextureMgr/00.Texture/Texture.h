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
		const wstring& wstrFilePath, /* 이미지 경로. */
		const wstring& wstrStateKey = L"", /* 상태 키는 멀티텍스쳐만 갖는다. */
		const DWORD& dwCnt = 0 /* 이미지 개수, 멀티 텍스쳐일 경우에만 사용함 */ ) PURE;
	virtual void Release() PURE;
};

