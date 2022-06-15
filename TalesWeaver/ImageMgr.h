#pragma once
#include "Image.h"

class ImageMgr
{
	DECLARE_SINGLETON(ImageMgr);

private:
	ImageMgr();
	~ImageMgr();

public:
	bool AddImage(Image* pImage, IMAGE_ID id);
	IMAGE_LIST& GetImage(IMAGE_ID id) { return mImageSpace[id]; }

public:
	void Render(HDC hDC);
	void LateUpdate();
	void Release();

private:
	IMAGE_LIST mImageSpace[IMAGE_END];
};

