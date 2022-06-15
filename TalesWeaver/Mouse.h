#pragma once

#include "Obj.h"
class AnimationMgr;

class Mouse final : public Obj
{
public:
	Mouse();
	virtual ~Mouse();

	// Obj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void Render(HDC hdc) override;
	virtual void LateUpdate() override;
	virtual void Release() override;

public:
	const TCHAR* GetMouseCurrentKey() const { return m_pMouseCurrentKey; }
	bool GetMouseMagic() const { return m_bMouseMagic; }
	void SetMouseMagic(bool bMouseMagic) { m_bMouseMagic = bMouseMagic; }
	void SetFieldScene(bool bFieldScene) { m_bFieldScene = bFieldScene; }

private:
	POINT mPt = {};

	// Animation Related Variable
	AnimationMgr* m_pAnimationMgr = nullptr;
	const TCHAR* m_pMouseCurrentKey = nullptr;
	const TCHAR* m_pMousePreKey = nullptr;

	// MouseMagic Related
	bool m_bMouseMagic = false;
	bool m_bFieldScene = false;


};

