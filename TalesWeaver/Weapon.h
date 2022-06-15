#pragma once

#include "Obj.h"

class AnimationMgr;

class Weapon final : public Obj
{
public:
	Weapon();
	virtual ~Weapon();

	// Obj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void Render(HDC hdc) override;
	virtual void LateUpdate() override;
	virtual void Release() override;

private:
	void PlayerKeyConvert();
	void EffectKeyConvert();

private:
	// Animation Related Variable
	AnimationMgr* m_pAnimationMgr = nullptr;

	// Bmp Related Variable;
	const TCHAR* m_pImageKey = nullptr;
	wstring mImageTempKey = L"Empty";

	// Effect Related Varibale
	wstring mEffectCurrentKey = L"";
	wstring mEffectPreKey = L"";

private:
	bool m_bWeapon = false;

};

