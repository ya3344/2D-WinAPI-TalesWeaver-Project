#pragma once

class Obj;
class Effect;

class CollisionMgr
{

	DECLARE_SINGLETON(CollisionMgr);

private:
	CollisionMgr();
	~CollisionMgr();

public:
	void CollisionRectEX(OBJ_LIST& srcList, OBJ_LIST& destList);

	void PlayerEnemyCollision(OBJ_LIST& playList, OBJ_LIST& enemyList);
	void EnemyEnemyCollision(OBJ_LIST& srcList, OBJ_LIST& destLis);

	void CollisionCircle(OBJ_LIST& srcList, OBJ_LIST& destList);
	void CollsionEnemyEffect(OBJ_LIST& playList , unordered_map<wstring, Image*> effectSpace);
	void CollsionPlayerEffect(OBJ_LIST& enemyList, unordered_map<wstring, Image*> effectSpace);
	void WeaponEnemyCollsion(OBJ_LIST& playList, OBJ_LIST& enemyList);

public:
	int GetComboCount() const { return mComboCount; }

public:
	bool CheckRect(Obj* pSrc, Obj* pDest, float* pMoveX, float* pMoveY);
	bool CheckCircle(Obj* pSrc, Obj* pDest);

private:
	// Attack Related
	DWORD mAttackTime = GetTickCount();
	int mBlockTime = 0;
	int mAttackPercent = 0;

	// Enemy Skill Realted 
	DWORD mEnemySkillTime = GetTickCount();
	int mEnemySkillBlockTime = 0;
	int mEnemySKillPercent = 0;

	// Player Skill Realted 
	DWORD mPlayerSkillTime = GetTickCount();
	int mPlayerSkillBlockTime = 0;
	int mPlayerSKillPercent = 0;
	int mPlayerSkillMaxPercent = 0;

	bool m_bAttackSucess = false;
	int mComboCount = 0;

	// Skill Related
	//DWORD mAttackTime = GetTickCount();
	//int mBlockTime = 0;
	//int mAttackPercent = 0;

	// Sound Related
	bool m_bDamagedSound = false;
	DWORD mDamagedTime = GetTickCount();
};

