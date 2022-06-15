#pragma once
#include "Obj.h"

class Satellite final : public Obj
{
public:
	Satellite();
	virtual ~Satellite();

public:
	// Obj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit() override;

	virtual int Update() override;
	virtual void Render(HDC hdc) override;
	virtual void LateUpdate() override;

	virtual void Release() override;

private:
	float mDistance = 0.f;
};

