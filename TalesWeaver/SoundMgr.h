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
	FMOD_SYSTEM* m_pSystem;	// FMOD �ý����� �Ѱ� ����
	FMOD_CHANNEL* m_pChannelArr[(size_t)CHANNAL::CHANNEL_END]; // ���� ����� �ʿ��� ä��
	map<const TCHAR*, FMOD_SOUND*>	m_MapSound;
	FMOD_BOOL m_bChannalPlayCheck;

};

