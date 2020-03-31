#pragma once

template <typename T>
void SafeDelete(T& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

template <typename T>
void SafeDelete_Array(T& ptr)
{
	if (ptr)
	{
		delete[] ptr;
		ptr = nullptr;
	}
}

template <typename T>
DWORD SafeRelease(T ptr)
{
	DWORD dwRefCnt = 0;

	if (ptr)
		dwRefCnt = ptr->Release();

	return dwRefCnt;
}