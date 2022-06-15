#pragma once
#include "Scene.h"

class AnimationMgr;

class FieldScene final : public Scene
{
public:
	FieldScene();
	~FieldScene();

	// Scene을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	void CollisionUpdate();
private:
	HDC m_hFieldDC;

private:
	AnimationMgr* m_pJellyAnimation = nullptr;
	AnimationMgr* m_pWolfAnimation = nullptr;
};

