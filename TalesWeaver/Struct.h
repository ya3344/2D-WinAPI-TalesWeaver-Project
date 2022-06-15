#pragma once

typedef struct tagInfo
{
	tagInfo()
	{
		ZeroMemory(this, sizeof(tagInfo));
	};
	explicit tagInfo(float X , float Y , float width , float height)
	{
		this->X = X;
		this->Y = Y;
		this->width = width;
		this->height = height;
	}


	float X = 0.f;
	float Y = 0.f;
	float width = 0.f;
	float height = 0.f;
}INFO;


typedef struct tagTileNum
{
	int X = 0;
	int Y = 0;

}TILE_NUM;








