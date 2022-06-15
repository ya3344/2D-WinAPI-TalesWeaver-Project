#include "stdafx.h"
#include "MainGame.h"
#include "Player.h"
#include "Mouse.h"
#include "HpBar.h"


MainGame::MainGame() 
{
	srand((unsigned int)time(nullptr));
}


MainGame::~MainGame()
{
	Release();
}

bool MainGame::Initialize()
{
	bool bInit = false;
	m_hDC = GetDC(g_hWnd);

	// 후면 버퍼로 쓰일 BMP 준비
	BmpMgr::GetInstance()->AddBmp(L"BackBuffer", L"../Resource/BackBuffer.bmp", false);

	// 잔상 제거용 BMP 준비
	BmpMgr::GetInstance()->AddBmp(L"Back", L"../Resource/Back.bmp", false);

	// 알파 블렌딩용 BMP 준비
	BmpMgr::GetInstance()->AddBmp(L"Alpha", nullptr, false);

	// DC 준비
	MyBmp* pBackBufferBmp = BmpMgr::GetInstance()->GetFindBmp(L"BackBuffer", false);
	if (pBackBufferBmp == nullptr)
	{
		MESSAGE_BOX(L"BackBuffer Fail!");
	}

	MyBmp* pBackBmp = BmpMgr::GetInstance()->GetFindBmp(L"Back", false);
	if (pBackBmp == nullptr)
	{
		MESSAGE_BOX(L"Back Find Faile!");
	}

	m_hBackBufferDC = pBackBufferBmp->GetMemDC();
	m_hBackMemDC = pBackBmp->GetMemDC();

	bInit = SceneMgr::GetInstance()->ChangeScene(SCENE_ID::LOGO);

	return bInit;
}

void MainGame::LateInit()
{
	SceneMgr::GetInstance()->LateInit();
}

void MainGame::Update()
{
	SceneMgr::GetInstance()->Update();
}

void MainGame::LateUpdate()
{
	SceneMgr::GetInstance()->LateUpdate();
	KeyMgr::GetInstance()->Update();
}

void MainGame::Render()
{
	// 잔상 제거용 비트맵을 백버퍼에 그린다.
	BitBlt(m_hBackBufferDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, m_hBackMemDC, 0, 0, SRCCOPY);
	SceneMgr::GetInstance()->Render(m_hBackBufferDC);

	// 후면 버퍼에 모아 그린 최종 결과를 전면 버퍼로 전송
	BitBlt(m_hDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, m_hBackBufferDC, 0, 0, SRCCOPY);
}



void MainGame::Release()
{
	ReleaseDC(g_hWnd, m_hDC);

	SceneMgr::GetInstance()->DestroyInstance();
	KeyMgr::GetInstance()->DestroyInstance();
	BmpMgr::GetInstance()->DestroyInstance();
	ImageMgr::GetInstance()->DestroyInstance();
	ScrollMgr::GetInstance()->DestroyInstance();
	TileMgr::GetInstance()->DestroyInstance();
	EffectMgr::GetInstance()->DestroyInstance();
	ObjMgr::GetInstance()->DestroyInstance();
	CollisionMgr::GetInstance()->DestroyInstance();
	ImageMgr::GetInstance()->DestroyInstance();
	SoundMgr::GetInstance()->DestroyInstance();

	_CrtDumpMemoryLeaks();
}
