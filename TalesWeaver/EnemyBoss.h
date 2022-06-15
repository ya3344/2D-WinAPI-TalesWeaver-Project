#pragma once

#include "Obj.h"

class AnimationMgr;
class AStar;
class Image;

class EnemyBoss final : public Obj
{
public:
	EnemyBoss();
	virtual ~EnemyBoss();

public:
	// Obj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void Render(HDC hdc) override;
	virtual void LateUpdate() override;
	virtual void Release() override;

public:
	const TCHAR* GetImageKey() const { return m_pFinalImageKey; }

private:
	void EnemyMove();
	void SceneChange();
	void Attack();
	void AttackMotion();
	void Damaged();

private:

	// Animation Related Variable
	AnimationMgr* m_pAnimationMgr = nullptr;
	const TCHAR* m_pPreMotion = nullptr;
	const TCHAR* m_pCurMotion = nullptr;
	const TCHAR* m_pSceneKey = nullptr;

	// Move Related Variable
	float mRadian = 0.f;
	AStar* m_pAStar = nullptr;
	float mDistance = 0.f;

	// Bmp Related Variable;
	const TCHAR* m_pImageKey = nullptr;
	const TCHAR* m_pFinalImageKey = nullptr;
	wstring mImageTempKey;

	// Multi Effect Related
	Image* m_pHitEffect = nullptr;
	Image* m_pCastingEffect = nullptr;
	Image* m_pFreezeEffect = nullptr;

	// Attack Related
	DWORD mAttackTime = GetTickCount();
	int mBlockTime = 0;
	int mAttackPercent = 0;
	int mAttackRange = 0;

	// AttackMotion Related
	DWORD mAttackMotionTime = GetTickCount();
	int mAttackMotinoPercent = 0;

	// Avoid Related
	MODE mPreMode;

	// Bullet Related
	Obj* m_pBullet = nullptr;
	DWORD mBulletTime = GetTickCount();
	float mBulletStartX = 0.f;
	float mBulletStartY = 0.f;

	bool m_bOverlapSound = false;

private:
	enum FRAME_SPEED
	{
		STAND = 200,
		WALK = 90,
		ATTACK = 130,
		AVOID = 130,
		SKILL = 300,
		FIRE = 80,
		DAMAGE = 800,
		MONSTER_HIT = 80,
		CASTING = 100,
	};

};

