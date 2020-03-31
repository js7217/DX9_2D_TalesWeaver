#pragma once
#include "Subject.h"
class CDataSubject :
	public CSubject
{
	DECLARE_SINGLETON(CDataSubject)

public:
	enum DATA_TYPE { PLAYER_HP, PLAYER_MAXHP, PLAYER_MP, PLAYER_MAXMP, PLAYER_SP, PLAYER_MAXSP,
		PLAYER_SEED, PLAYER_LEVEL, PLAYER_EXP, PLAYER_MAXEXP, PLAYER_MATRIX };

private:
	CDataSubject();
	virtual ~CDataSubject();

public:
	list<void*>* GetDataLst(int iMessage);

public:
	void AddData(int iMessage, void* pData = nullptr);
	void RemoveData(int iMessage, void* pData = nullptr);

private:
	map<int, list<void*>>	m_MapData;
};

