#include "stdafx.h"
#include "TileMgr.h"
#include "Tile.h"

INIT_SINGLETON_VARIABLE(TileMgr);

TILE_NUM gTileNum = {};

TileMgr::TileMgr()
{
}


TileMgr::~TileMgr()
{
	Release();
}

void TileMgr::Initialize()
{
	for (Image* pTile : mTileSpace)
	{
		pTile->Initialize();
	}
}

void TileMgr::LateUpdate()
{
	for (Image* pTile : mTileSpace)
	{
		pTile->LateUpdate();
	}

	if (KeyMgr::GetInstance()->KeyDown(L'D'))
	{
		m_bDraw = !m_bDraw;
	}
}

void TileMgr::Render(HDC hdc)
{
	if (!m_bDraw)
		return;

	size_t index = 0;

	float scrollX = ScrollMgr::GetInstance()->GetScrollX();
	float scrollY = ScrollMgr::GetInstance()->GetScrollY();

	int endX = WINDOW_WIDTH / TILE_WIDTH;
	int endY = WINDOW_HEIGHT / TILE_HEIGHT;
	int culX = (int)(-scrollX / TILE_WIDTH);
	int culY = (int)(-scrollY / TILE_HEIGHT);

	for (int i = culY; i < (culY + endY) + 2; ++i)
	{
		for (int j = culX; j < (culX + endX) + 2; ++j)
		{
			index = j + (i*gTileNum.X);

			if (index < 0 || index >= mTileSpace.size())
				continue;

			mTileSpace[index]->Render(hdc);
		}
	}

	TCHAR name[128] = L"";
	swprintf_s(name, L"ScrollX:%f ScrollY:%f", scrollX, scrollY);
	//TextOut(hdc, 50, 100, name, lstrlen(name));


}


void TileMgr::LoadData(const TCHAR* pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"Tile load failed!!!", L"Error!!!", MB_OK);
		return;
	}

	DWORD byte = 0;
	INFO info;
	int drawID = 0;

	Image* pTile = nullptr;

	ReadFile(hFile, &gTileNum, sizeof(TILE_NUM), &byte, nullptr);
	mTileSpace.reserve(gTileNum.X*gTileNum.Y);

	while (true)
	{
		ReadFile(hFile, &info, sizeof(INFO), &byte, nullptr);
		ReadFile(hFile, &drawID, sizeof(int), &byte, nullptr);

		if (0 == byte)
			break;

		pTile = new Tile(info);
		pTile->SetDrawID(drawID);
		mTileSpace.emplace_back(pTile);

		// Enemy Index Setting
		if (pTile->GetDrawID() == TILE_ENEMY_POSITION)
		{
			mEnemyIndex.emplace_back(pTile->GetInfo());
		}
	}

	CloseHandle(hFile);
}

void TileMgr::Release()
{
	for_each(mTileSpace.begin(), mTileSpace.end(), SafeDelete<Image*>);
	mTileSpace.clear();
	vector<Image*>().swap(mTileSpace);

	mEnemyIndex.clear();
}

