#pragma once

#include "Texture.h"

class CMultiTexture : 
	public CTexture
{
public:
	CMultiTexture();
	virtual ~CMultiTexture();

public:
	// CTexture을(를) 통해 상속됨
	virtual const TEX_INFO * GetTexInfo(
		const wstring & wstrStateKey = L"", 
		const DWORD & dwIndex = 0) const override;

public:
	virtual HRESULT InsertTexture(
		const wstring & wstrFilePath, 
		const wstring & wstrStateKey = L"", 
		const DWORD & dwCnt = 0) override;
	virtual void Release() override;

public:
	map<const wstring, vector<TEX_INFO*>>		m_MapMultiTex;
};

