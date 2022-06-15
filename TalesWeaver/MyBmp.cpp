#include "stdafx.h"
#include "MyBmp.h"


MyBmp::MyBmp()
{
}


MyBmp::~MyBmp()
{
}

MyBmp * MyBmp::LoadBmp(const TCHAR * pFilePath)
{
	BITMAP bmp;
	m_hDC = GetDC(g_hWnd);

	m_hMemDC = CreateCompatibleDC(m_hDC);
	ReleaseDC(g_hWnd, m_hDC);

	lstrcpy(mFilePath, pFilePath);

	m_hBitmap = (HBITMAP)LoadImage(0, mFilePath, IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	if (m_hBitmap == nullptr)
	{
		MESSAGE_BOX(L"FilePath Fail!");
	}

	int res = ::GetObject(m_hBitmap, sizeof(BITMAP), (LPVOID)&bmp);

	mWidth = (int)bmp.bmWidth;
	mHeight = (int)bmp.bmHeight;


	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);


	return this;
}

MyBmp * MyBmp::LoadAlphaBmp()
{
	MyBmp* pBmp = BmpMgr::GetInstance()->GetFindBmp(L"BackBuffer", false);

	if (pBmp == nullptr)
	{
		MESSAGE_BOX(L"BackBuffer Find Fail!");
	}
	else
	{
		m_hMemDC = CreateCompatibleDC(pBmp->GetMemDC());
		m_hBitmap = CreateCompatibleBitmap(pBmp->GetMemDC(), WINDOW_WIDTH, WINDOW_HEIGHT);
		m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
		//BitBlt(m_hMemDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, pBmp->GetMemDC(), 0, 0, SRCCOPY);
	}
	return this;
}

MyBmp * MyBmp::LoadReverseBmp(wstring imageKey, const TCHAR * pMotionKey, bool bDynamic)
{
	BITMAP bmp;

	if (!lstrcmp(pMotionKey, L"RD"))
	{
		imageKey = StringMgr::stringReplace(imageKey, pMotionKey, L"LD");
	}
	else if (!lstrcmp(pMotionKey, L"R"))
	{
		imageKey = StringMgr::stringReplace(imageKey, pMotionKey, L"L");
	}
	else if (!lstrcmp(pMotionKey, L"RU"))
	{
		imageKey = StringMgr::stringReplace(imageKey, pMotionKey, L"LU");
	}

	MyBmp* pBmp = BmpMgr::GetInstance()->GetFindBmp(imageKey, bDynamic);

	if (pBmp == nullptr)
	{
		MESSAGE_BOX(L"LoadReverse Find Fail!");
	}
	else
	{
		m_hDC = GetDC(g_hWnd);

		m_hMemDC = CreateCompatibleDC(m_hDC);
		ReleaseDC(g_hWnd, m_hDC);

		m_hBitmap = (HBITMAP)LoadImage(0, pBmp->mFilePath, IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		if (m_hBitmap == nullptr)
		{
			MESSAGE_BOX(L"LoadReverse FilePath Fail!");
		}
		m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);

		int res = ::GetObject(m_hBitmap, sizeof(BITMAP), (LPVOID)&bmp);

		mWidth = (int)bmp.bmWidth;
		mHeight = (int)bmp.bmHeight;
	
		//SetStretchBltMode(m_hMemDC, COLORONCOLOR);
		StretchBlt(m_hMemDC, 0, 0, mWidth, mHeight, pBmp->GetMemDC(), mWidth, 0, mWidth * -1, mHeight, SRCCOPY);
	}

	return this;
}

void MyBmp::Release()
{
	SelectObject(m_hMemDC, m_hOldBitmap);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}
