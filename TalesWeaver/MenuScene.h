#pragma once
#include "Scene.h"
class MenuScene final : public Scene
{
public:
	MenuScene();
	virtual ~MenuScene();

public:
	// Scene��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	Obj* m_pMouse = nullptr;


};

