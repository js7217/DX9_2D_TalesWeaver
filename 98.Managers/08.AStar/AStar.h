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
	bool IsFindingPath(int iStartIndex, int iGoalIndex);	// ��� Ž��
	void MakeBestLst(int iStartIndex, int iGoalIndex);		// ��� ����

public:
	int SelectIndex(const D3DXVECTOR3& vPos);
	BYTE SelectOption(const D3DXVECTOR3& vPos); // �ش� �ε����� Option�� ��ȯ.
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

