#pragma once

class MyBmp
{
public:
	MyBmp();
	~MyBmp();

public:
	HDC GetMemDC() { return m_hMemDC; }

public:
	MyBmp* LoadBmp(const TCHAR* pFilePath);
	MyBmp* LoadAlphaBmp();
	MyBmp* LoadReverseBmp(wstring imageKey, const TCHAR* pMotionKey, bool bDynamic);
	void Release();

public:
	int GetWidth() const { return mWidth; }
	int GetHeight() const { return mHeight; }

private:
	HDC m_hDC;
	HDC m_hMemDC;
	HBITMAP m_hBitmap;
	HBITMAP m_hOldBitmap;
	TCHAR mFilePath[256] = {};

private:
	int mWidth = 0;
	int mHeight = 0;
};

