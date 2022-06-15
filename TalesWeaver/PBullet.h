#pragma once
#include "Obj.h"

class AnimationMgr;
class Image;

class PBullet final : public Obj
{
public:
	PBullet();
	virtual ~PBullet();

	// Obj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void Render(HDC hdc) override;
	virtual void LateUpdate() override;
	virtual void Release() override;

private:
	void BulletCollision();

	float mRadian = 0.f;
	bool m_bInit = false;
	bool m_bBulletDie = false;

	Image* m_pBombEffect = nullptr;

private:
	AnimationMgr* m_pBulletAnimation = nullptr;
	BLENDFUNCTION mBlendInfo;
};

