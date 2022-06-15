#include "stdafx.h"
#include "Line.h"


Line::Line()
{
}

Line::Line(LINE_INFO& lineInfo) : mInfo(lineInfo)
{
}


Line::~Line()
{
}

void Line::Render(HDC hdc)
{
	float scrollX = ScrollMgr::GetInstance()->GetScrollX();
	float scrollY = ScrollMgr::GetInstance()->GetScrollY();
	MoveToEx(hdc, (int)(mInfo.startX + scrollX), (int)(mInfo.startY + scrollY), nullptr);
	LineTo(hdc, (int)(mInfo.endX + scrollX), (int)(mInfo.endY + scrollY));
}

