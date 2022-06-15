#pragma once

class Image;
class Obj;

template <typename T>
class ImageFactory
{
public:
	static Image* CreateImage(const float X, const float Y, const TCHAR* pBmpKey)
	{
		Image* pImage = new T;
		//pImage->Initialize();
		pImage->SetBmpKey(pBmpKey);
		pImage->SetPos(X, Y);

		return pImage;
	}


	static Image* CreateImage(Obj* m_pTarget, const float X, const float Y, const TCHAR* pBmpKey)
	{
		Image* pImage = new T;
		pImage->SetBmpKey(pBmpKey);
		pImage->SetPos(X, Y);
		pImage->SetTarget(m_pTarget);

		return pImage;
	}

	static Image* CreateImage(const float X, const float Y)
	{
		Image* pImage = new T;
		pImage->Initialize();
		pImage->SetPos(X, Y);

		return pImage;
	}
};


