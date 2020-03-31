#pragma once

#ifdef _AFX	
#define ERR_MSG(message)									\
	AfxMessageBox(message)						
#else											
#define ERR_MSG(message)									\
	MessageBox(nullptr, message, L"System Error", MB_OK)	
#endif

#define NO_EVENT 0
#define DEAD_OBJ 1

#define NULL_CHECK(PTR)					\
	if(nullptr == (PTR)) { return; }

#define NULL_CHECK_RETURN(PTR, RETURN_VAL)		\
	if(nullptr == (PTR)) { return RETURN_VAL; }

#define NULL_CHECK_MSG_RETURN(PTR, MESSAGE, RETURN_VAL)				\
	if(nullptr == (PTR)) { ERR_MSG(MESSAGE); return RETURN_VAL; }

#define FAILED_CHECK(HR)		\
	if(FAILED(HR)) { return; }

#define FAILED_CHECK_RETURN(HR, RETURN_VAL) \
	if(FAILED(HR)) { return RETURN_VAL; }

#define FAILED_CHECK_MSG_RETURN(HR, MESSAGE, RETURN_VAL)		\
	if(FAILED(HR)) { ERR_MSG(MESSAGE); return RETURN_VAL; }

#define RUBY		0x01
#define SAPPHIRE	0x02
#define DIAMOND		0x04

#define WINCX 800
#define WINCY 600

#define TILEX 20
#define TILEY 30

#define TILECX 94
#define TILECY 48

#define MIN_STR 64
#define MID_STR 128
#define MAX_STR 256

// 무분별한 복사 방지.
#define NO_COPY(ClassName)						\
private:										\
	ClassName(const ClassName& obj);			\
	ClassName& operator=(const ClassName& obj);

#define DECLARE_SINGLETON(ClassName)		\
		NO_COPY(ClassName)					\
public:										\
	static ClassName* GetInstance()			\
	{										\
		if(nullptr == m_pInstance)			\
			m_pInstance = new ClassName;	\
											\
		return m_pInstance;					\
	}										\
											\
	void DestroyInstance()					\
	{										\
		if(m_pInstance)						\
		{									\
			delete m_pInstance;				\
			m_pInstance = nullptr;			\
		}									\
	}										\
											\
private:									\
	static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)		\
ClassName* ClassName::m_pInstance = nullptr;