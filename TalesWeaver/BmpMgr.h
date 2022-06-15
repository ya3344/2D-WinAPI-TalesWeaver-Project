#pragma once
#include "MyBmp.h"

class BmpMgr
{
	DECLARE_SINGLETON(BmpMgr);

private:
	BmpMgr();
	~BmpMgr();


public:
	MyBmp* GetFindBmp(wstring findKey, bool bDynamic);
	MyBmp* GetFindAllBmp(wstring findKey);
	void AddBmp(wstring key, TCHAR* pStr, bool bDynamic, bool bReverse = false);

public:
	void DeleteDynamicBmpSpace();
public:
	void Release();
	
private:
	unordered_map<wstring, MyBmp*> mBmpSpace[CLASSIFY_END];
};