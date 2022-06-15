#pragma once
class SoundMgr
{
	DECLARE_SINGLETON(SoundMgr);


private:
	SoundMgr();
	virtual ~SoundMgr();

public:
	void Initialize();
	void Update();
	void Release();

public:
	void LoadSoundFile();
	void PlaySound(const TCHAR* pSoundKey, CHANNAL id);
	void PlayBGM(const TCHAR* pSoundKey, CHANNAL id);
	void StopSound(CHANNAL id);
	void StopAll();
	void SetVolume(CHANNAL id, float fVal);
	int IsPlayCheck(CHANNAL id);

private:
	FMOD_SYSTEM* m_pSystem;	// FMOD 시스템을 총괄 관리
	FMOD_CHANNEL* m_pChannelArr[(size_t)CHANNAL::CHANNEL_END]; // 사운드 재생에 필요한 채널
	map<const TCHAR*, FMOD_SOUND*>	m_MapSound;
	FMOD_BOOL m_bChannalPlayCheck;

};

