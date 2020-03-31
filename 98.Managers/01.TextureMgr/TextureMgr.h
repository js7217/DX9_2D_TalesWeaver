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
		const wstring& wstrFilePath, /* �̹��� ���. */
		const wstring& wstrObjKey, /* ������Ʈ Ű */
		const wstring& wstrStateKey = L"NONE", /* ���� Ű�� ��Ƽ�ؽ��ĸ� ���´�. */
		const DWORD& dwCnt = 0 /* �̹��� ����, ��Ƽ �ؽ����� ��쿡�� ����� */,
		const wstring& wstrDircetionKey = L"NONE");
	virtual void Release();

private:
	int m_iCount;
	map<const wstring, map<const wstring, CTexture*>>	m_MapTexture;
	wstring												m_wstrLoadingMessage;
};

