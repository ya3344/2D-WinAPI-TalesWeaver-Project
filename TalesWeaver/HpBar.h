#pragma once
#include "Obj.h"

class HpBar final : public Obj
{
public:
	HpBar();
	virtual ~HpBar();

public:
	// Obj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit() override;

	virtual int Update() override;
	virtual void Render(HDC hdc) override;
	virtual void LateUpdate() override;

	virtual void Release() override;
};

