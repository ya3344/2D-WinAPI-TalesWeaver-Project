#include "stdafx.h"
#include "Image.h"

Image::Image()
{
}


Image::~Image()
{
}

void Image::SetPos(const float X, const float Y)
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

void Image::SetEffect(const float X, const float Y, const int alpha, bool bForever, bool bScroll, float attack)
{
	
	mInfo.X = X;
	mInfo.Y = Y;
	mAlpha = alpha;
	m_bForever = bForever;
	m_bScroll = bScroll;
	mAttack = attack;
}

void Image::UpdateRect(bool bScroll)
{
	if (bScroll)
	{
		float scrollX = ScrollMgr::GetInstance()->GetScrollX();
		float scrollY = ScrollMgr::GetInstance()->GetScrollY();

		mRect.left = LONG((mInfo.X + scrollX) - (mInfo.width / 2));
		mRect.top = LONG((mInfo.Y + scrollY) - (mInfo.height / 2));
		mRect.right = LONG((mInfo.X + scrollX) + (mInfo.width / 2));
		mRect.bottom = LONG((mInfo.Y + scrollY) + (mInfo.height / 2));


	}
	else
	{
		mRect.left = LONG((mInfo.X) - (mInfo.width / 2));
		mRect.top = LONG(mInfo.Y - (mInfo.height / 2));
		mRect.right = LONG((mInfo.X) + (mInfo.width / 2));
		mRect.bottom = LONG(mInfo.Y + (mInfo.height / 2));

	}
}
