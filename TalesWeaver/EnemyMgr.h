#pragma once

class Enemy;

class EnemyMgr
{
	DECLARE_SINGLETON(EnemyMgr);
public:
	EnemyMgr();
	~EnemyMgr();

public:
	void Initialize();
	void Update();
	void LateUpdate();
	void Render(HDC hDC);
	void Release();

public:
	unordered_map<wstring, Obj*> mEnemySpace;
};

