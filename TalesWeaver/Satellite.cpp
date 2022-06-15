#include "stdafx.h"
#include "Satellite.h"


Satellite::Satellite()
{
}


Satellite::~Satellite()
{
}

void Satellite::Initialize()
{
	mAttribute.degree= 90.f;
	mAttribute.speed = 5.f;
	mDistance = 100.f;
}

void Satellite::LateInit()
{
	mInfo.X = m_pTarget->GetInfo().X + mDistance * (cosf(ToRadian(mAttribute.degree)));
	mInfo.Y = m_pTarget->GetInfo().Y - mDistance * (sinf(ToRadian(mAttribute.degree)));

	mInfo.width = m_pTarget->GetInfo().width * 0.3f;
	mInfo.height = m_pTarget->GetInfo().height * 0.3f;
}


void Satellite::Render(HDC hdc)
{
	Ellipse(hdc, mRect.left, mRect.top, mRect.right, mRect.bottom);
}

int Satellite::Update()
{
	if (mAttribute.bDie)
		return OBJ_DIE;

	mAttribute.degree += mAttribute.speed;

	mInfo.X = m_pTarget->GetInfo().X + mDistance * (cosf(ToRadian(mAttribute.degree)));
	mInfo.Y = m_pTarget->GetInfo().Y - mDistance * (sinf(ToRadian(mAttribute.degree)));

	return OBJ_NORMAL;
}

void Satellite::LateUpdate()
{
	if (m_pTarget == nullptr)
	{
		mAttribute.bDie = true;
		return;
	}

	UpdateRect();
}

void Satellite::Release()
{

}