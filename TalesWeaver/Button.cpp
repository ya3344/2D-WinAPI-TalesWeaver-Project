#include "stdafx.h"
#include "Button.h"


Button::Button()
{
}


Button::~Button()
{
}

void Button::Initialize()
{
	mBmpKey = m_pBmpKey;

	m_pButtonBmp = BmpMgr::GetInstance()->GetFindBmp(mBmpKey, true);

	if (m_pButtonBmp == nullptr)
	{
		MESSAGE_BOX(mBmpKey.c_str());
	}

	hButtonDC = m_pButtonBmp->GetMemDC();
	mInfo.width = (float)m_pButtonBmp->GetWidth();
	mInfo.height = (float)m_pButtonBmp->GetHeight();
	UpdateRect(false);
}

void Button::LateUpdate()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	size_t pos = 0;
	pt.x = (long)(pt.x - (mInfo.width/ 2));
	pt.y = (long)(pt.y - (mInfo.height));

	if (PtInRect(&mRect, pt))
	{
		pos = mBmpKey.find(L"0");

		if (pos != wstring::npos)
		{
			mBmpKey.replace(mBmpKey.begin() + pos, mBmpKey.begin() + pos + 1, L"1");	
			m_pBmpKey = mBmpKey.c_str();
			SoundMgr::GetInstance()->PlaySound(L"BtnClick.wav", UI_SOUND);
		}
		
	}
	else
	{
		pos = mBmpKey.find(L"1");

		if (pos != wstring::npos)
		{
			mBmpKey.replace(mBmpKey.begin() + pos, mBmpKey.begin() + pos + 1, L"0");
			m_pBmpKey = mBmpKey.c_str();
		}
	}
	
}

void Button::Render(HDC hDC)
{
	m_pButtonBmp = BmpMgr::GetInstance()->GetFindBmp(mBmpKey, true);

	if (m_pButtonBmp == nullptr)
	{
		MESSAGE_BOX(mBmpKey.c_str());
	}

	hButtonDC = m_pButtonBmp->GetMemDC();

	BitBlt(hDC, (int)mInfo.X, (int)mInfo.Y, (int)mInfo.width, (int)mInfo.height, hButtonDC, 0, 0, SRCCOPY);
}

void Button::Release()
{
}
