#include "stdafx.h"
#include "SoundMgr.h"

INIT_SINGLETON_VARIABLE(SoundMgr);

SoundMgr::SoundMgr()
{
	Initialize();
}


SoundMgr::~SoundMgr()
{
	Release();
}

void SoundMgr::Initialize()
{
	FMOD_System_Create(&m_pSystem);
	FMOD_System_Init(m_pSystem, size_t(CHANNAL::CHANNEL_END), FMOD_INIT_NORMAL, nullptr);

	LoadSoundFile();
}

void SoundMgr::Update()
{
	FMOD_System_Update(m_pSystem);
}

void SoundMgr::Release()
{
	for (auto& MyPair : m_MapSound)
	{
		delete[] MyPair.first;
		FMOD_Sound_Release(MyPair.second);
	}

	m_MapSound.clear();

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}

void SoundMgr::LoadSoundFile()
{
	_finddata_t fd;

	long handle = 0;
	handle = _findfirst("../Sound/*.*", &fd);

	if (-1 == handle)
	{
		MessageBox(g_hWnd, L"Not Found!", L"Failed!!", MB_OK);
		return;
	}

	int iResult = 0;

	while (-1 != iResult)
	{
		char szFullPath[128] = "../Sound/";
		strcat_s(szFullPath, fd.name);

		FMOD_SOUND* pSound = nullptr;
		FMOD_RESULT res = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE
			, nullptr, &pSound);

		if (FMOD_OK == res)
		{
			int iLength = strlen(fd.name);
			TCHAR* pSoundKey = new TCHAR[iLength + 1];

			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength + 1, pSoundKey, iLength + 1);
			m_MapSound.insert({ pSoundKey, pSound });
		}

		iResult = _findnext(handle, &fd);
	}

	_findclose(handle);
	FMOD_System_Update(m_pSystem);
}

void SoundMgr::PlaySound(const TCHAR * pSoundKey, CHANNAL iD)
{
	auto iter_find = find_if(m_MapSound.begin(), m_MapSound.end(),
		[&pSoundKey](auto& MyPair)
	{
		return !lstrcmp(pSoundKey, MyPair.first);
	});

	if (m_MapSound.end() == iter_find)
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE
		, iter_find->second, FALSE, &(m_pChannelArr[iD]));
}

void SoundMgr::PlayBGM(const TCHAR * pSoundKey, CHANNAL id)
{
	auto iter_find = find_if(m_MapSound.begin(), m_MapSound.end(),
		[&pSoundKey](auto& MyPair)
	{
		return !lstrcmp(pSoundKey, MyPair.first);
	});

	if (m_MapSound.end() == iter_find)
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE
		, iter_find->second, FALSE, &(m_pChannelArr[id]));

	FMOD_Channel_SetMode(m_pChannelArr[id], FMOD_LOOP_NORMAL);
}

void SoundMgr::StopSound(CHANNAL iD)
{
	FMOD_Channel_Stop(m_pChannelArr[iD]);
}

void SoundMgr::StopAll()
{
	for (size_t i = 0; i < (size_t)CHANNAL::CHANNEL_END; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void SoundMgr::SetVolume(CHANNAL id, float fVal)
{
	FMOD_Channel_SetVolume(m_pChannelArr[id], fVal);
}

int SoundMgr::IsPlayCheck(CHANNAL id)
{
	FMOD_Channel_IsPlaying(m_pChannelArr[id], &m_bChannalPlayCheck);
	return m_bChannalPlayCheck;
}

// Sample
//CSoundMgr::GetInstance()->SetVolume(CSoundMgr::BGM, m_MaxVolume -= 0.1f);