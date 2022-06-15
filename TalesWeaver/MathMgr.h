#pragma once

class Obj;
class Image;

class MathMgr
{
public:
	MathMgr();
	~MathMgr();

public:
	static float CaclRadian(Obj* pSrc, Obj* pDest);
	static float CaclRadian(Obj* pSrc, Image* pDest);
	static float CaclRadian(Image* pSrc, Image* pDest);
	static float CaclRadian(Obj* pSrc, INFO info);

	static float CaclDistance(Obj* pSrc, Obj* pDest);
	static float CaclDistance(Image* pSrc, Image* pDest);
	static float CaclDistance(Obj* pSrc, Image* pDest);

	static void PointToTileIndex(size_t& tileIndex, float srcX, float srcY );

	static size_t CalTargetIndex();
	static POINT CalPoint();
	static const TCHAR* CalPlayRotation(float degree);
	static const TCHAR* CalMonsterRotation(float degree);
};

