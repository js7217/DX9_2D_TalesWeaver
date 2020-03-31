#include "stdafx.h"
#include "SoundMgr.h"

IMPLEMENT_SINGLETON(CSoundMgr)

CSoundMgr::CSoundMgr()
{
}


CSoundMgr::~CSoundMgr()
{
	Release();
}

void CSoundMgr::Initialize()
{
	FMOD_System_Create(&m_pSystem);
	// 1. 시스템 포인터, 2.사용할 가상채널 수, 초기화 방식. 

	FMOD_System_Init(m_pSystem, 64, FMOD_INIT_NORMAL, NULL);
	LoadSoundFile();
}

void CSoundMgr::Release()
{
	for (auto& MyPair : m_mapSound)
	{
		delete MyPair.first;
		FMOD_Sound_Release(MyPair.second);
	}
	m_mapSound.clear();
	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}

void CSoundMgr::MyPlaySound(TCHAR * pSoundKey, CHANNELID eID)
{
	auto& iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto Mypair)
	{
		return !lstrcmp(pSoundKey, Mypair.first);
	});

	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL isPlay;
	if (FMOD_Channel_IsPlaying(m_pChannelArr[eID], &isPlay))
	{
		FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[eID]);
	}
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::PlayBGM(TCHAR * pSoundKey)
{
	if (m_mapSound.empty())
		return;

	auto& iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto Mypair)
	{
		return !lstrcmp(pSoundKey, Mypair.first);
	});
	/* m_mapSound.find(pSoundKey);*/

	if (m_mapSound.end() == iter)
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[BGM], FMOD_LOOP_NORMAL);
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::StopSound(CHANNELID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSoundMgr::AllStop()
{
	for (int i = 0; i < MAXCHANNEL; ++i)
	{
		FMOD_Channel_Stop(m_pChannelArr[i]);
	}
}

void CSoundMgr::LoadSoundFile()
{
	_finddata_t fd;
	long Handle = _findfirst("../Sound/*.*", &fd);

	if (0 == Handle)
		return;

	int iResult = 0;

	char szCurPath[128] = "../Sound/";
	char szFullPath[128] = "";

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);
		strcat_s(szFullPath, fd.name);

		FMOD_SOUND* pSound = nullptr;

		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE, 0, &pSound);

		if (FMOD_OK == eRes)
		{
			int iLength = strlen(fd.name) + 1;

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);
			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);


			m_mapSound.emplace(pSoundKey, pSound);

		}
		iResult = _findnext(Handle, &fd);
	}
	FMOD_System_Update(m_pSystem);
}
