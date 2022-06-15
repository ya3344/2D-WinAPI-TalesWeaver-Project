#pragma once
#include "Obj.h"

class AnimationMgr;
class Image;

class Bullet final : public Obj
{
public:
	Bullet();
	virtual ~Bullet();

public:
	// Obj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit() override;

	virtual int Update() override;
	virtual void Render(HDC hdc) override;
	virtual void LateUpdate() override;

	virtual void Release() override;

private:
	void BulletCollision();

private:
	float mDistance = 0.f;
	float mRotSpeed = 0.f; // ���� �ӵ�
	float mCenterX = 0.f;
	float mCenterY = 0.f;
	float mRotAngle = 0.f;

	float mRadian = 0.f;
	bool m_bInit = false;
	bool m_bBulletDie = false;


	Image* m_pPopEffect = nullptr;

private:
	AnimationMgr* m_pBulletAnimation = nullptr;
	BLENDFUNCTION mBlendInfo;

private:


};

