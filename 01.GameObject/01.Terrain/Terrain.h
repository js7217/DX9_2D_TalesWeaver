#pragma once

#include "GameObject.h"

class CTerrain : public CGameObject
{
private:
	explicit CTerrain();
	explicit CTerrain(const wstring& strFilePath);
public:
	virtual ~CTerrain();
public:
	const vector<TILE*>& GetVecTile() const { return m_vecTile; }
	const vector<list<TILE*>>& GetGraph() const { return m_vecGraph; }

public:
	void SetTileRender(bool bIsRender) { m_bIsTileRender = bIsRender; }
	const float& Get_Width() { return m_fWidth; }
public:
	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInit();
	virtual void Release() override;

private:
	HRESULT LoadTile(const wstring& strFilePath);
	HRESULT MakeGraph();
	void KeyCheck();
public:
	static CTerrain* Create(const wstring& strFilePath);
private:
	CKeyMgr*			m_pKeyMgr;
	bool				m_bIsTileRender; // 타일을 렌더할지말지.
	float				m_fWidth; // 맵 가로
	float				m_fHeight; // 맵 세로
	TILE*				m_pBackGround; // 백그라운드.
	wstring				m_strFilePath; // 파일 경로
	vector<TILE*>		m_vecTile;
	vector<list<TILE*>> m_vecGraph;

	// CGameObject을(를) 통해 상속됨
	virtual void LineRender() override;
};

