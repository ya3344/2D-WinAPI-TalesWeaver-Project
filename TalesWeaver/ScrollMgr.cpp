#include "stdafx.h"
#include "ScrollMgr.h"
#include "Player.h"

INIT_SINGLETON_VARIABLE(ScrollMgr);

ScrollMgr::ScrollMgr()
{
}


ScrollMgr::~ScrollMgr()
{

}

void ScrollMgr::Update()
{
	Obj* pObj = ObjMgr::GetInstance()->GetObj(OBJ_PLAYER).back();


	// Right
	if (pObj->GetInfo().X + mScrollX >= WINDOW_WIDTH - (WINDOW_WIDTH/2))
	{
		mScrollX -= pObj->GetAttriBute().speed / sqrt(2.f);

		if (mScrollX <= (-1 * ((TILE_WIDTH * gTileNum.X) - WINDOW_WIDTH)) )
			mScrollX = (float)(-1 * ((TILE_WIDTH * gTileNum.X) - WINDOW_WIDTH));
	}

	// Up
	if (pObj->GetInfo().Y + mScrollY <= 0 + (WINDOW_HEIGHT / 2))
	{
		mScrollY += pObj->GetAttriBute().speed / sqrt(2.f);

		if (mScrollY >= 0)
			mScrollY = 0;
	}

	// Left
	if (pObj->GetInfo().X + mScrollX <= 0 + (WINDOW_WIDTH / 2))
	{
		mScrollX += pObj->GetAttriBute().speed / sqrt(2.f);

		if (mScrollX >= 0)
			mScrollX = 0;
	}


	// Down
	if (pObj->GetInfo().Y + mScrollY >= WINDOW_HEIGHT - (WINDOW_HEIGHT / 2))
	{
		mScrollY -= pObj->GetAttriBute().speed / sqrt(2.f);

		if (mScrollY <= -1 * ((TILE_HEIGHT * gTileNum.Y) - WINDOW_HEIGHT))
			mScrollY = (float)(-1 * ((TILE_HEIGHT * gTileNum.Y) - WINDOW_HEIGHT));
	}


}
