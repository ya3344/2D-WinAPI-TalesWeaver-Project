#include "stdafx.h"
#include "LineMgr.h"
#include "Line.h"

INIT_SINGLETON_VARIABLE(LineMgr);

LineMgr::LineMgr()
{

}


LineMgr::~LineMgr()
{
	Release();
}

void LineMgr::Initialize()
{
	LoadData();
}

bool LineMgr::LinearCollision(float targetX, float *pOutY)
{
	Line* pTempLine = nullptr;
	if (mLineSpace.empty())
	{
		return false;
	}

	for (Line* pLine : mLineSpace)
	{
		if (targetX >= pLine->GetLineInfo().startX && targetX <= pLine->GetLineInfo().endX)
		{
			pTempLine = pLine;
			break;
		}
	}

	if (pTempLine == false)
	{
		return false;
	}

	float width = pTempLine->GetLineInfo().endX - pTempLine->GetLineInfo().startX;
	float height = pTempLine->GetLineInfo().endY - pTempLine->GetLineInfo().startY;
	float gradient = height / width;

	*pOutY = gradient*(targetX - pTempLine->GetLineInfo().startX) + pTempLine->GetLineInfo().startY;

	return true;
}

void LineMgr::Render(HDC hdc)
{
	TCHAR szName[128] = L"";

	int count = 0;
	for (Line* pline : mLineSpace)
	{
		pline->Render(hdc);

		//if (count <= 5)
		//{
		//	swprintf_s(szName, L"LineStartX: %f LineStartY: %f LineEndX: %f LineEndY: %f"
		//		, pline->GetLineInfo().startX + ScrollMgr::GetInstance()->GetScrollX(), pline->GetLineInfo().startY, pline->GetLineInfo().endX + ScrollMgr::GetInstance()->GetScrollX(), pline->GetLineInfo().endY);
		//	TextOut(hdc, 50, 90 + (count * 20), szName, lstrlen(szName)); // 지정한 좌표에 폰트 출력!
		//	++count;
		//}

		//TCHAR szScrollName[128] = L"";
		//swprintf_s(szScrollName, L"ScrollX: %f", ScrollMgr::GetInstance()->GetScrollX());
		//TextOut(hdc, 50, 240, szScrollName, lstrlen(szScrollName)); // 지정한 좌표에 폰트 출력!
	
	}

}



void LineMgr::LoadData()
{
	HANDLE file = CreateFile(L"../Data/Line.dat", GENERIC_READ, 
		0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == file)
	{
		MessageBox(g_hWnd, L"Line Load Failed", L"Error!!!", MB_OK);
		return;
	}
	DWORD byte = 0;
	LINE_INFO lineInfo;

	ReadFile(file, &mLineMaxNum, sizeof(mLineMaxNum), &byte, 0);
	mLineSpace.reserve(mLineMaxNum);

	while (true)
	{
		ReadFile(file, &lineInfo, sizeof(LINE_INFO), &byte, 0);

		if (byte == 0)
			break;

		mLineSpace.emplace_back(new Line(lineInfo));
	}

}


void LineMgr::Release()
{
	for_each(mLineSpace.begin(), mLineSpace.end(), SafeDelete<Line*>);

	mLineSpace.clear();
	vector<Line*>().swap(mLineSpace);
}