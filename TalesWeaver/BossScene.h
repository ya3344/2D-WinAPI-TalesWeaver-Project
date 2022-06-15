#pragma once

#include "Scene.h"

class AnimationMgr;

class BossScene : public Scene
{
public:
	BossScene();
	virtual ~BossScene();

	// Scene을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	HDC m_hBossRoomDC;

private:
	AnimationMgr* m_pCreedWizardAnimation = nullptr;
	AnimationMgr* m_pBossAnimation = nullptr;
};

