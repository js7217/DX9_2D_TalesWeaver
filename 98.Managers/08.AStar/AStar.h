#pragma once
class CAStar
{
private:
	explicit CAStar();
public:
	~CAStar();

public:
	list<TILE*>& GetBestLst() { return m_BestLst; }
	void Set_ClearBestLst() { m_BestLst.clear(); }
public:
	void StartAstar(const D3DXVECTOR3& vStart, const D3DXVECTOR3& vGoal);

private:
	bool IsFindingPath(int iStartIndex, int iGoalIndex);	// 경로 탐색
	void MakeBestLst(int iStartIndex, int iGoalIndex);		// 경로 생성

public:
	int SelectIndex(const D3DXVECTOR3& vPos);
	BYTE SelectOption(const D3DXVECTOR3& vPos); // 해당 인덱스의 Option을 반환.
	bool IsPicking(const D3DXVECTOR3& vPos, int iIndex);

private:
	bool CheckOpenLst(int iIndex);
	bool CheckCloseLst(int iIndex);

public:
	static CAStar* Create();

private:
	int			m_iStartIndex;
	list<int>	m_OpenLst;
	list<int>	m_CloseLst;
	list<TILE*> m_BestLst;
};

