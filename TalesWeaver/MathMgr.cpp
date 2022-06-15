#include "stdafx.h"
#include "Obj.h"
#include "MathMgr.h"


MathMgr::MathMgr()
{
}


MathMgr::~MathMgr()
{
}

float MathMgr::CaclRadian(Obj * pSrc, Obj * pDest)
{
	float width = pDest->GetInfo().X - pSrc->GetInfo().X;
	float height = pDest->GetInfo().Y - pSrc->GetInfo().Y;
	float distance = sqrt((width * width) + (height * height));

	float radian = acosf(width / distance);

	// 180도 이후값 각도 보정 acos 0~180도 까지
	if (pDest->GetInfo().Y > pSrc->GetInfo().Y)
	{
		radian *= -1.f;
	}

	return radian;
}

float MathMgr::CaclRadian(Obj * pSrc, Image * pDest)
{
	float width = pDest->GetInfo().X - pSrc->GetInfo().X;
	float height = pDest->GetInfo().Y - pSrc->GetInfo().Y;
	float distance = sqrt((width * width) + (height * height));

	float radian = acosf(width / distance);

	// 180도 이후값 각도 보정 acos 0~180도 까지
	if (pDest->GetInfo().Y > pSrc->GetInfo().Y)
	{
		radian *= -1.f;
	}

	return radian;
}

float MathMgr::CaclRadian(Image * pSrc, Image * pDest)
{
	float width = pDest->GetInfo().X - pSrc->GetInfo().X;
	float height = pDest->GetInfo().Y - pSrc->GetInfo().Y;
	float distance = sqrt((width * width) + (height * height));

	float radian = acosf(width / distance);

	// 180도 이후값 각도 보정 acos 0~180도 까지
	if (pDest->GetInfo().Y > pSrc->GetInfo().Y)
	{
		radian *= -1.f;
	}

	return radian;
}

float MathMgr::CaclRadian(Obj * pSrc, INFO info)
{

	float width = info.X - pSrc->GetInfo().X;
	float height = info.Y - pSrc->GetInfo().Y;
	float distance = sqrt((width * width) + (height * height));

	float radian = acosf(width / distance);

	// 180도 이후값 각도 보정 acos 0~180도 까지
	if (info.Y > pSrc->GetInfo().Y)
	{
		radian *= -1.f;
	}

	return radian;
}

float MathMgr::CaclDistance(Obj * pSrc, Obj * pDest)
{
	float X = pDest->GetInfo().X - pSrc->GetInfo().X;
	float Y = pDest->GetInfo().Y - pSrc->GetInfo().Y;
	float distance = sqrt((X * X) + (Y * Y));

	return distance;
}

float MathMgr::CaclDistance(Image * pSrc, Image * pDest)
{
	float X = pDest->GetInfo().X - pSrc->GetInfo().X;
	float Y = pDest->GetInfo().Y - pSrc->GetInfo().Y;
	float distance = sqrt((X * X) + (Y * Y));

	return distance;
}

float MathMgr::CaclDistance(Obj * pSrc, Image * pDest)
{
	float X = pDest->GetInfo().X - pSrc->GetInfo().X;
	float Y = pDest->GetInfo().Y - pSrc->GetInfo().Y;
	float distance = sqrt((X * X) + (Y * Y));

	return distance;
}

void MathMgr::PointToTileIndex(size_t& tileIndex, float srcX, float srcY )
{
	int X = (int)((srcX) / TILE_WIDTH);
	int Y = (int)((srcY) / TILE_HEIGHT);

	tileIndex = X + (gTileNum.X * Y);

	if (tileIndex < 0 || tileIndex >= TileMgr::GetInstance()->GetTileSpace().size())
		tileIndex = 0;
}

size_t MathMgr::CalTargetIndex()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	float scrollX = ScrollMgr::GetInstance()->GetScrollX();
	float scrollY = ScrollMgr::GetInstance()->GetScrollY();

	int X = (int)((pt.x - scrollX) / TILE_WIDTH);
	int Y = (int)((pt.y - scrollY) / TILE_HEIGHT);

	size_t targetIndex = X + (gTileNum.X * Y);

	if (pt.x <= 0 || pt.x >= WINDOW_WIDTH || pt.y <= 0 || pt.y >= WINDOW_HEIGHT)
		targetIndex = -1;

	return targetIndex;
}

POINT MathMgr::CalPoint()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	float scrollX = ScrollMgr::GetInstance()->GetScrollX();
	float scrollY = ScrollMgr::GetInstance()->GetScrollY();

	int X = (int)((pt.x - scrollX));
	int Y = (int)((pt.y - scrollY));

	POINT target = { X, Y };

	return target;
}

const TCHAR * MathMgr::CalPlayRotation(float degree)
{
	const TCHAR* pMotion = nullptr;

	if (degree >= 80.f && degree <= 110.f)
	{
		pMotion = L"U";
	}
	else if (degree >= 20.f && degree <= 79.f)
	{
		pMotion = L"RU";
	}
	else if (degree >= 110.f && degree <= 170.f)
	{
		pMotion = L"LU";
	}
	else if (degree >= 170.f && degree <= 181.f)
	{
		pMotion = L"L";
	}
	else if (degree >= -181.f && degree <= -160.f)
	{
		pMotion = L"L";
	}
	else if (degree >= -20.f && degree <= 20.f)
	{
		pMotion = L"R";
	}
	else if (degree >= -69.f && degree <= -20.f)
	{
		pMotion = L"RD";
	}
	else if (degree >= -100.f && degree <= -70.f)
	{
		pMotion = L"D";
	}
	else if (degree >= -159.f && degree <= -101.f)
	{
		pMotion = L"LD";
	}

	return pMotion;
}

const TCHAR * MathMgr::CalMonsterRotation(float degree)
{
	const TCHAR* pMotion = nullptr;

	if (degree >= 80.f && degree <= 110.f)
	{
		pMotion = L"LU";
	}
	else if (degree >= 20.f && degree <= 79.f)
	{
		pMotion = L"RU";
	}
	else if (degree >= 110.f && degree <= 170.f)
	{
		pMotion = L"LU";
	}
	else if (degree >= 170.f && degree <= 181.f)
	{
		pMotion = L"LU";
	}
	else if (degree >= -181.f && degree <= -160.f)
	{
		pMotion = L"LD";
	}
	else if (degree >= -20.f && degree <= 20.f)
	{
		pMotion = L"RU";
	}
	else if (degree >= -69.f && degree <= -20.f)
	{
		pMotion = L"RD";
	}
	else if (degree >= -100.f && degree <= -70.f)
	{
		pMotion = L"RD";
	}
	else if (degree >= -159.f && degree <= -101.f)
	{
		pMotion = L"LD";
	}

	return pMotion;
}


