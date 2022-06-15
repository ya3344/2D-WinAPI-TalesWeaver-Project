#pragma once
#include "Obj.h"

class Bullet;
class AnimationMgr;
class AStar;

class Player final : public Obj
{
public:
	Player();
	virtual ~Player();

public:
	// Obj Inherited
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual void Render(HDC hdc) override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Release() override;

public:
	const TCHAR* GetStageKey() const { return m_pStageKey; }
	const TCHAR* GetImageKey() const { return m_pFinalImageKey; }
	const TCHAR* GetEffectKey() const { return m_pImageKey; }
	bool GetStageInit() const { return m_bStageInit; }

private:
	Obj* CreateBullet();
	void SceneChange();
	void StageChange();
	void StageInit(float X, float Y, const TCHAR* pStageKey, const TCHAR* pFilePath );
	void PlayerMove();
	void ComboSystem(size_t curScene);
	void PlayRotation(float radian);
	void HorCut();
	void VerCut();
	void Stab();
	void CollisionEX();
	bool EXChecRect(Obj * pDest, float * pMoveX, float * pMoveY);
	

private:
	enum BARREL
	{
		BARREL_DISTANCE = 100,
	};


private:
	// Bullet Related Variable
	float mBarrelX = 0.f;
	float mBarrelY = 0.f;

	// Animation Related Variable
	AnimationMgr* m_pAnimationMgr = nullptr;

	const TCHAR* m_pPreMotion = nullptr;
	const TCHAR* m_pCurMotion = nullptr;
	const TCHAR* m_pSceneKey = nullptr;

	// Move Related Variable
	float mRadian = 0.f;
	float mDegree = 0.f;
	AStar* m_pAStar = nullptr;
	bool m_bRun = false;
	Obj* m_pMouse = nullptr;

	// Bmp Related Variable;
	const TCHAR* m_pImageKey = nullptr;
	const TCHAR* m_pFinalImageKey = nullptr;
	wstring mImageTempKey;

	// Tile Related Variable
	size_t mTargetIndex = 0;

	// Stage Related Variable
	const TCHAR* m_pStageKey = nullptr;
	bool m_bStageInit = true;

	// Rotation Related Variable
	bool m_bRotated = false;

	// Collision Related Variable
	float mCollisionX = 0.f;
	float mCollisionY = 0.f;
	bool m_bCollisionDraw = false;
	float mCollisionRadian = 0.f;

	// Combo Related Variable
	int mComboCount = 0;

	// Skill Related
	Image* m_pThunderBolt = nullptr;
	Image* m_pBossSkill = nullptr;
	Image* m_pMultiAttack = nullptr;

	// Overlap Sound Related
	bool m_bOverlapSound = false;
};

