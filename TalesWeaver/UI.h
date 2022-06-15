#pragma once
#include "Image.h"

class UI final : public Image
{
public:
	UI();
	~UI();

	// Image을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	
	HDC hUI_DC;
	MyBmp* m_pUIBmp = nullptr;
	float mDynamicWidth = 0.f;
};

