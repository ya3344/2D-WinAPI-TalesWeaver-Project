#include "stdafx.h"
#include "Obj.h"


Obj::Obj() 
{
	mAttribute.degree = 90.f;
}


Obj::~Obj()
{
}

void Obj::UpdateRect(bool bScroll)
{
	if (bScroll)
	{
		float scrollX = ScrollMgr::GetInstance()->GetScrollX();
		float scrollY = ScrollMgr::GetInstance()->GetScrollY();

		mRect.left = LONG((mInfo.X + scrollX) - (mInfo.width / 2));
		mRect.top = LONG((mInfo.Y + scrollY) - (mInfo.height / 2));
		mRect.right = LONG((mInfo.X + scrollX) + (mInfo.width / 2));
		mRect.bottom = LONG((mInfo.Y + scrollY) + (mInfo.height / 2));

		MathMgr::PointToTileIndex(mTileIndex, mInfo.X, mInfo.Y);
	}
	else
	{
		mRect.left = LONG((mInfo.X) - (mInfo.width / 2));
		mRect.top = LONG(mInfo.Y - (mInfo.height / 2));
		mRect.right = LONG((mInfo.X) + (mInfo.width / 2));
		mRect.bottom = LONG(mInfo.Y + (mInfo.height / 2));
	}

}

RECT Obj::CollisionRect(float X, float Y, int Width, int Height)
{
	RECT rect = {};

	float scrollX = ScrollMgr::GetInstance()->GetScrollX();
	float scrollY = ScrollMgr::GetInstance()->GetScrollY();

	rect.left = LONG((X + scrollX) - (Width / 2));
	rect.top = LONG((Y + scrollY) - (Height / 2));
	rect.right = LONG((X + scrollX) + (Width / 2));
	rect.bottom = LONG((Y + scrollY) + (Height / 2));

	

	return rect;
}


void Obj::SetPos(const float X, const float Y)
{
	if (X > 0)
	{
		mInfo.X = X;
	}
	if (Y > 0)
	{
		mInfo.Y = Y;
	}
}

void Obj::SetDamage(const float attack)
{
	mAttribute.currentHP -= attack;
	mAttribute.mode = MODE::DAMAGED;

}
