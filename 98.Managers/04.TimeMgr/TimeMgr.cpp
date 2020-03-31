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
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return m_fDeltaTime;
}

HRESULT CTimeMgr::InitTimeMgr()
{
	// �ϵ����(���κ���)�� �����ϴ� ���ػ� Ÿ�̸� ��(���� CPU Ŭ����)�� ������ �Լ�. 
	// 1 / 10000��
	QueryPerformanceCounter(&m_OldTime);
	QueryPerformanceCounter(&m_CurTime);

	// ���� CPU�� �������� ������ �Լ�. 1�ʵ��� �󸶳� �����ϴ���.
	QueryPerformanceFrequency(&m_CpuTick);

	return S_OK;
}

void CTimeMgr::UpdateTime()
{
	QueryPerformanceCounter(&m_CurTime);

	// ���� �����̰� �ɷ��� �� �ʱ� ���·� ����.
	if (m_CurTime.QuadPart - m_OldTime.QuadPart > m_CpuTick.QuadPart)
		InitTimeMgr();

	m_fDeltaTime = float(m_CurTime.QuadPart - m_OldTime.QuadPart) / m_CpuTick.QuadPart;
	m_OldTime = m_CurTime;
}
