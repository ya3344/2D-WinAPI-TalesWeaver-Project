#include "stdafx.h"
#include "UI.h"
#include "Player.h"

UI::UI()
{

}


UI::~UI()
{

}

void UI::Initialize()
{
	m_pUIBmp = BmpMgr::GetInstance()->GetFindAllBmp(m_pBmpKey);

	if (m_pUIBmp == nullptr)
	{
		MESSAGE_BOX(m_pBmpKey);
	}

	hUI_DC = m_pUIBmp->GetMemDC();
	mInfo.width = (float)m_pUIBmp->GetWidth();
	mInfo.height = (float)m_pUIBmp->GetHeight();
	UpdateRect(false);
}

void UI::LateUpdate()
{
	if (m_pBmpKey == L"SpBar")
	{
		mInfo.width = (float)m_pUIBmp->GetWidth() * (m_pTarget->GetAttriBute().currentSP / m_pTarget->GetAttriBute().maxSP);
	}
	else if (m_pBmpKey == L"HpBar")
	{
		mInfo.width = (float)m_pUIBmp->GetWidth() * (m_pTarget->GetAttriBute().currentHP / m_pTarget->GetAttriBute().maxHP);
	}
	else if (m_pBmpKey == L"MpBar")
	{
		mInfo.width = (float)m_pUIBmp->GetWidth() * (m_pTarget->GetAttriBute().currentMP / m_pTarget->GetAttriBute().maxMP);
	}
	else
	{
		mInfo.width;
	}
}

void UI::Render(HDC hDC)
{
	GdiTransparentBlt(hDC, mRect.left, mRect.top, (int)mInfo.width, (int)mInfo.height, hUI_DC,
		0, 0, (int)mInfo.width, (int)mInfo.height, RGB(255, 255, 255));
}

void UI::Release()
{
}
