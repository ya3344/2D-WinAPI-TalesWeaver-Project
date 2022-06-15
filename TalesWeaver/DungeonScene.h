#pragma once

#include "Scene.h"

class AnimationMgr;

class DungeonScene final : public Scene
{
public:
	DungeonScene();
	virtual ~DungeonScene();

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
	HDC m_hDungeonDC;

private:
	AnimationMgr* m_pDarkCowAnimation = nullptr;
	AnimationMgr* m_pCreedWizardAnimation = nullptr;
};

