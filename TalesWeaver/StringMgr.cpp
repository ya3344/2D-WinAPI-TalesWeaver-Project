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
		// �����̸��� ���ϴ� ��ġ���� ���� ��ȯ�Ѵ�.(�߰��̸��� ����Ǵ� ���� ����)
		pos = pImageKey.find(L'_');
		pos = pImageKey.find(L'_', pos + 1);

		// ����Ʈ�� _���ھ �ϳ��̹Ƿ� ����ó���Ѵ�.
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
