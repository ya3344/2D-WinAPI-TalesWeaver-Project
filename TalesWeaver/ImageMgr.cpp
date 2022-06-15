#include "stdafx.h"
#include "ImageMgr.h"


INIT_SINGLETON_VARIABLE(ImageMgr);

ImageMgr::ImageMgr()
{
}


ImageMgr::~ImageMgr()
{
	Release();
}

bool ImageMgr::AddImage(Image * pImage, IMAGE_ID id)
{
	if (pImage == nullptr)
		return false;

	pImage->Initialize();
	mImageSpace[id].emplace_back(pImage);

	return true;
}

void ImageMgr::Render(HDC hDC)
{
	for (int i = 0; i<IMAGE_END; ++i)
	{
		for (Image* pImage : mImageSpace[i])
		{
			pImage->Render(hDC);
		}
	}
}

void ImageMgr::LateUpdate()
{
	for (int i = 0; i<IMAGE_END; ++i)
	{
		for (Image* pImage : mImageSpace[i])
		{
			pImage->LateUpdate();
		}
	}
}

void ImageMgr::Release()
{
	for (int i = 0; i < IMAGE_END; ++i)
	{
		for_each(mImageSpace[i].begin(), mImageSpace[i].end(), SafeDelete<Image*>);
		mImageSpace[i].clear();
	}
}
