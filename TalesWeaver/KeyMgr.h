#pragma once

class KeyMgr
{
	DECLARE_SINGLETON(KeyMgr);

private:
	KeyMgr();
	~KeyMgr();

public:
	bool KeyPressing(int key)
	{
		if (GetAsyncKeyState(key) & 0x8000)
			return true;

		return false;
	}

	bool KeyDown(int key)
	{
		if (!m_bKey[key] && GetAsyncKeyState(key) & 0x8000)
		{
			m_bKey[key] = !m_bKey[key];
			return true;
		}

		return false;
	}

	bool KeyUp(int key)
	{
		if (m_bKey[key] && !(GetAsyncKeyState(key) & 0x8000))
		{
			m_bKey[key] = !m_bKey[key];
			return true;
		}

		return false;
	}

public:
	void Update() // 프레임 마지막에 호출~!
	{
		for (int i = 0; i < VIRTUAL_KEY_MAX; ++i)
		{
			// KeyDown 상태 초기화
			if (m_bKey[i] && !(GetAsyncKeyState(i) & 0x8000))
			{
				m_bKey[i] = !m_bKey[i];
			}

			// KeyUp 상태 초기화
			if (!m_bKey[i] && (GetAsyncKeyState(i) & 0x8000))
			{
				m_bKey[i] = !m_bKey[i];
			}
		}
	}

private:
	bool m_bKey[VIRTUAL_KEY_MAX];
};

