#pragma once

class Obj;

class MainGame
{
public:
	MainGame();
	~MainGame();

public:
	bool Initialize();
	void LateInit();
	void Render();
	void Update();
	void LateUpdate();
	void Release();
	
private:
	HDC m_hDC;
	HDC m_hBackBufferDC;
	HDC m_hBackMemDC;
};

