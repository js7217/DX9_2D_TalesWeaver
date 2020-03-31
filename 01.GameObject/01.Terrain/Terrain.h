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
	// CGameObject��(��) ���� ��ӵ�
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
	bool				m_bIsTileRender; // Ÿ���� ������������.
	float				m_fWidth; // �� ����
	float				m_fHeight; // �� ����
	TILE*				m_pBackGround; // ��׶���.
	wstring				m_strFilePath; // ���� ���
	vector<TILE*>		m_vecTile;
	vector<list<TILE*>> m_vecGraph;

	// CGameObject��(��) ���� ��ӵ�
	virtual void LineRender() override;
};

