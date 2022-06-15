#pragma once
class StringMgr
{
public:
	StringMgr();
	~StringMgr();

public:
	static wstring stringReplace(wstring pImageKey, const TCHAR* pSrcKey, const TCHAR* replace, bool bSceneChange = false);

};

