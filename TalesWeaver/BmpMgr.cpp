#include "stdafx.h"
#include "BmpMgr.h"

INIT_SINGLETON_VARIABLE(BmpMgr);

BmpMgr::BmpMgr()
{
}

BmpMgr::~BmpMgr()
{
	Release();
}

MyBmp * BmpMgr::GetFindBmp(wstring findKey, bool bDynamic)
{
	unordered_map<wstring, MyBmp*>::iterator iterFind;
	if (bDynamic)
	{
		iterFind = mBmpSpace[DYNAMIC].find(findKey);

		if (iterFind == mBmpSpace[DYNAMIC].end())
		{
			return nullptr;
		}
	}
	else
	{
		iterFind = mBmpSpace[STATIC].find(findKey);

		if (iterFind == mBmpSpace[STATIC].end())
		{
			return nullptr;
		}
	}


	return iterFind->second;
}

MyBmp * BmpMgr::GetFindAllBmp(wstring findKey)
{
	unordered_map<wstring, MyBmp*>::iterator iterFind;

	iterFind = mBmpSpace[DYNAMIC].find(findKey);

	if (iterFind != mBmpSpace[DYNAMIC].end())
	{
		return iterFind->second;
	}
	else
	{
		iterFind = mBmpSpace[STATIC].find(findKey);

		if (iterFind == mBmpSpace[STATIC].end())
		{
			return nullptr;
		}
	}


	return iterFind->second;
}

void BmpMgr::AddBmp(wstring key, TCHAR * pStr, bool bDynamic, bool bReverse)
{
	if (bDynamic)
	{
		MyBmp* pBmp = GetFindBmp(key, bDynamic);

		if (pBmp == nullptr)
		{
			MyBmp* pBmp = new MyBmp;

			// 알파 블렌딩용 Bmp 생성
			if (pStr == nullptr)
			{
				mBmpSpace[DYNAMIC].emplace(bmpMapPair(key, pBmp->LoadAlphaBmp()));
			}
			else
			{
				// LoadMap으로 Bmp 생성
				if (bReverse == false)
				{
					mBmpSpace[DYNAMIC].emplace(bmpMapPair(key, pBmp->LoadBmp(pStr)));
				}
				// 좌우반전으로 Bmp 생성
				else
				{
					mBmpSpace[DYNAMIC].emplace(bmpMapPair(key, pBmp->LoadReverseBmp(key, pStr, true)));
				}
			}
		}
		else
		{
			MESSAGE_BOX(L"BmpKey Overlap!!");
		}
	}
	else
	{
		MyBmp* pBmp = GetFindBmp(key, bDynamic);

		if (pBmp == nullptr)
		{
			MyBmp* pBmp = new MyBmp;

			// 알파 블렌딩용 Bmp 생성
			if (pStr == nullptr)
			{
				mBmpSpace[STATIC].emplace(bmpMapPair(key, pBmp->LoadAlphaBmp()));
			}
			else
			{
				// LoadMap으로 Bmp 생성
				if (bReverse == false)
				{
					mBmpSpace[STATIC].emplace(bmpMapPair(key, pBmp->LoadBmp(pStr)));
				}
				// 좌우반전으로 Bmp 생성
				else
				{
					mBmpSpace[STATIC].emplace(bmpMapPair(key, pBmp->LoadReverseBmp(key, pStr, false)));
				}
			}
		}
		else
		{
			MESSAGE_BOX(L"BmpKey Overlap!!");
		}
	}
	
}

void BmpMgr::DeleteDynamicBmpSpace()
{
	for (bmpMapPair mapPair : mBmpSpace[DYNAMIC])
	{
		if (mapPair.second)
		{
			delete mapPair.second;
			mapPair.second = nullptr;
		}
	}

	mBmpSpace[DYNAMIC].clear();
}

void BmpMgr::Release()
{
	for (int i = 0; i < CLASSIFY_END; ++i)
	{
		for_each(mBmpSpace[i].begin(), mBmpSpace[i].end(),
			[](bmpMapPair mapPair)
		{
			if (mapPair.second)
			{
				delete mapPair.second;
				mapPair.second = nullptr;
			}
		}
		);

		mBmpSpace[i].clear();
	}
	
}
