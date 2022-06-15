#include "stdafx.h"
#include "StringMgr.h"


StringMgr::StringMgr()
{
}


StringMgr::~StringMgr()
{
}

wstring StringMgr::stringReplace(wstring pImageKey, const TCHAR * pSrcKey, const TCHAR* pReplacekey, bool bSceneChange)
{
	wstring reverseKey = L"Empty";
	wstring::size_type pos = 0;
	wstring::size_type effectPos = 0;

	if (bSceneChange)
	{
		pos = pImageKey.find(pSrcKey, pos);
	}
	else
	{
		// 파일이름이 변하는 위치까지 가서 변환한다.(중간이름이 변경되는 것을 방지)
		pos = pImageKey.find(L'_');
		pos = pImageKey.find(L'_', pos + 1);

		// 이펙트는 _스코어가 하나이므로 예외처리한다.
		effectPos = pImageKey.find(L'_');

		if (pos == string::npos)
		{
			pos = pImageKey.find(pSrcKey, effectPos);
		}
		else
		{
			pos = pImageKey.find(pSrcKey, pos);
		}

	}

	if (pos != string::npos)
	{
		reverseKey = pImageKey.replace(pImageKey.begin() + pos,
			pImageKey.begin() + pos + lstrlen(pSrcKey), pReplacekey);
	}

	return reverseKey;
}
