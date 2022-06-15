#pragma once
#include "Scene.h"

class Obj;


class VillageScene final : public Scene
{
public:
	VillageScene();
	virtual ~VillageScene();

	// Scene을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	void EffectLoadBmp();

private:
	const TCHAR* m_pStagePreKey = nullptr;
	const TCHAR* m_pStageCurrentKey = nullptr;
	MyBmp* m_pMapBmp = nullptr;



};

