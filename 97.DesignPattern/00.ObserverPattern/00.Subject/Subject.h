#pragma once

class CObserver;
class CSubject
{
public:
	CSubject();
	virtual ~CSubject();

public:
	void Subcribe(CObserver* pObserver);
	void UnSubcribe(CObserver* pObserver);
	void Notify(int iMessage, void* pData = nullptr);

protected:
	list<CObserver*>	m_ObserverLst;
};

