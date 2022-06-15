#pragma once

#include "Obj.h"

class AnimationMgr;
class AStar;
class Image;

class EnemyWolf final : public Obj
{
public:
	EnemyWolf();
	virtual ~EnemyWolf();

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
	void EffectCollision();
	void RandmoMobe();
	void AttackCollision();

private:
	// Random Move Related Variable
	size_t  mMoveState;
	DWORD mMoveTime = GetTickCount();

	size_t mRandomTime = 0;
	size_t mRandomTileIndex = 0;

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
	Image* m_pDustEffect = nullptr;

	// Attack Related
	DWORD mAttackTime = GetTickCount();
	int mBlockTime = 0;
	int mAttackPercent = 0;
;

private:
	enum FRAME_SPEED
	{
		STAND = 200,
		WALK = 90,
		ATTACK = 100,
		DAMAGE = 20,
		MONSTER_HIT = 80,
		DUST = 150,
	};

};

