#pragma once
#include "Scene.h"

class LogoScene final : public Scene
{
public:
	LogoScene();
	virtual ~LogoScene();

	// Scene을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	Obj* m_pMouse = nullptr;
};

