#include "stdafx.h"
#include "TimeMgr.h"

IMPLEMENT_SINGLETON(CTimeMgr)

CTimeMgr::CTimeMgr()
	:m_fDeltaTime(0.f)
{
	ZeroMemory(&m_OldTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CurTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CpuTick, sizeof(LARGE_INTEGER));
}


CTimeMgr::~CTimeMgr()
{
}

const float & CTimeMgr::GetDeltaTime() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_fDeltaTime;
}

HRESULT CTimeMgr::InitTimeMgr()
{
	// 하드웨어(메인보드)에 존재하는 고해상도 타이머 값(현재 CPU 클럭수)을 얻어오는 함수. 
	// 1 / 10000초
	QueryPerformanceCounter(&m_OldTime);
	QueryPerformanceCounter(&m_CurTime);

	// 현재 CPU의 진동수를 얻어오는 함수. 1초동안 얼마나 진동하는지.
	QueryPerformanceFrequency(&m_CpuTick);

	return S_OK;
}

void CTimeMgr::UpdateTime()
{
	QueryPerformanceCounter(&m_CurTime);

	// 일정 딜레이가 걸렸을 때 초기 상태로 돌림.
	if (m_CurTime.QuadPart - m_OldTime.QuadPart > m_CpuTick.QuadPart)
		InitTimeMgr();

	m_fDeltaTime = float(m_CurTime.QuadPart - m_OldTime.QuadPart) / m_CpuTick.QuadPart;
	m_OldTime = m_CurTime;
}
