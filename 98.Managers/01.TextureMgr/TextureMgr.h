#pragma once

class CTexture;
class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)

public:
	enum TEX_TYPE { TEX_SINGLE, TEX_MULTI };

private:
	CTextureMgr();
	~CTextureMgr();

public:
	virtual const TEX_INFO* GetTexInfo(
		const wstring& wstrObjKey,
		const DWORD& dwIndex = 0,
		const wstring& wstrStateKey = L"NONE",
		const wstring& wstrDirectionKey = L"NONE") const;

public:
	const wstring& GetLoadingMessage() const;
	const int& GetLoadingCount() const;
public:
	void SetLoadingMessage(const wstring& wstrMessage);

public:
	HRESULT LoadFromImgPath(const wstring& wstrImgPath);
	virtual HRESULT InsertTexture(
		TEX_TYPE eTextureType,
		const wstring& wstrFilePath, /* 이미지 경로. */
		const wstring& wstrObjKey, /* 오브젝트 키 */
		const wstring& wstrStateKey = L"NONE", /* 상태 키는 멀티텍스쳐만 갖는다. */
		const DWORD& dwCnt = 0 /* 이미지 개수, 멀티 텍스쳐일 경우에만 사용함 */,
		const wstring& wstrDircetionKey = L"NONE");
	virtual void Release();

private:
	int m_iCount;
	map<const wstring, map<const wstring, CTexture*>>	m_MapTexture;
	wstring												m_wstrLoadingMessage;
};

