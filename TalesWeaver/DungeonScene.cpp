#include "stdafx.h"
#include "DungeonScene.h"
#include "Player.h"
#include "AnimationMgr.h"
#include "EnemyDarkCow.h"
#include "EnemyCreedWizard.h"


DungeonScene::DungeonScene()
{
}


DungeonScene::~DungeonScene()
{
	Release();
}

void DungeonScene::Initialize()
{
	BmpMgr::GetInstance()->AddBmp(L"Dungeon", L"../Resource/Map/Dungeon.bmp", true);

	MyBmp* pMapBmp = BmpMgr::GetInstance()->GetFindBmp(L"Dungeon", true);
	if (pMapBmp == nullptr)
	{
		MESSAGE_BOX(L"MapBmp Find Fail!!");
	}

	mMapWidth = pMapBmp->GetWidth();
	mMapHeight = pMapBmp->GetHeight();

	m_hDungeonDC = pMapBmp->GetMemDC();

	TileMgr::GetInstance()->Release();
	TileMgr::GetInstance()->LoadData(L"../Data/Dungeon.dat");
	TileMgr::GetInstance()->Initialize();
	m_pPlayer = ObjMgr::GetInstance()->GetPlayerObj();
	m_pPlayer->SetPos(1632.f, 538.f);
	//m_pPlayer->SetPos(2909.f, 545.f);

	// Enemy Bmp 준비
	if (m_pDarkCowAnimation == nullptr)
	{
		m_pDarkCowAnimation = new AnimationMgr(ANIMATION_INFO(ANIM_TYPE::DARKCOW, L"../Resource/Monster/DarkCow/%s/%s_%s/%s_%s%d.bmp",
			L"DarkCow_%s_%s%d", (size_t)MONSTER_SCENE::DEATH, (OBJ_MOTION::MOTION_END / 2)));
		m_pDarkCowAnimation->ObjLoadBmp(true);
	}

	if (m_pCreedWizardAnimation == nullptr)
	{
		m_pCreedWizardAnimation = new AnimationMgr(ANIMATION_INFO(ANIM_TYPE::CREEDWIZARD, L"../Resource/Monster/CreedWizard/%s/%s_%s/%s_%s%d.bmp",
			L"CreedWizard_%s_%s%d", (size_t)MONSTER_SCENE::DEATH, (OBJ_MOTION::MOTION_END / 2)));
		m_pCreedWizardAnimation->ObjLoadBmp(true);
	}

	for (size_t i = 0; i < TileMgr::GetInstance()->GetEnemyIndex().size(); ++i)
	{
		// EnemyObject 준비
		if ((i+1) % 4 == 0)
		{
			Obj* pObj = ObjFactory<EnemyCreedWizard>::CreateObj(m_pPlayer, TileMgr::GetInstance()->GetEnemyIndex()[i]);
			ObjMgr::GetInstance()->AddObj(pObj, OBJ_ENEMY);


		}
		else if ( i <= 4)
		{
			Obj* pObj = ObjFactory<EnemyDarkCow>::CreateObj(m_pPlayer, TileMgr::GetInstance()->GetEnemyIndex()[i]);
			ObjMgr::GetInstance()->AddObj(pObj, OBJ_ENEMY);
		}
	}

	//for (int i = 0; i < 2; ++i)
	//{
	//	// EnemyObject 준비


	//	Obj* pObj = ObjFactory<EnemyCreedWizard>::CreateObj(m_pPlayer, TileMgr::GetInstance()->GetEnemyIndex()[i]);
	//	ObjMgr::GetInstance()->AddObj(pObj, OBJ_ENEMY);


	//}

	EffectMgr::GetInstance()->AddEffect(L"Crash", L"../Resource/Effect/Crash/Crash%d.bmp",
		(size_t)EFFECT_FRAME::CRASH, (size_t)FRAME_SPEED::CRASH, IMAGE_TYPE::ATTACK, true);

	EffectMgr::GetInstance()->AddEffect(L"Cray", L"../Resource/Effect/Cray/Cray%d.bmp",
		(size_t)EFFECT_FRAME::CRAY, (size_t)FRAME_SPEED::CRAY, IMAGE_TYPE::ATTACK, true);

	SoundMgr::GetInstance()->StopAll();
	SoundMgr::GetInstance()->PlayBGM(L"Dungeon.MP3", BGM);
	//SoundMgr::GetInstance()->SetVolume(BGM, 0.5f);

}

void DungeonScene::LateInit()
{
	ObjMgr::GetInstance()->LateInit();
}

void DungeonScene::Update()
{
	ObjMgr::GetInstance()->Update();
	ScrollMgr::GetInstance()->Update();
	EffectMgr::GetInstance()->Update();
}

void DungeonScene::LateUpdate()
{
	ObjMgr::GetInstance()->CollisionUpdate();
	ObjMgr::GetInstance()->LateUpdate();
	ImageMgr::GetInstance()->LateUpdate();
	EffectMgr::GetInstance()->LateUpdate();
	TileMgr::GetInstance()->LateUpdate();
}

void DungeonScene::Render(HDC hDC)
{
	float scrollX = ScrollMgr::GetInstance()->GetScrollX();
	float scrollY = ScrollMgr::GetInstance()->GetScrollY();

	BitBlt(hDC, (int)(scrollX), (int)(scrollY), mMapWidth, mMapHeight, m_hDungeonDC, 0, 0, SRCCOPY);

	ObjMgr::GetInstance()->Render(hDC);
	ImageMgr::GetInstance()->Render(hDC);
	EffectMgr::GetInstance()->Render(hDC);
	TileMgr::GetInstance()->Render(hDC);
}

void DungeonScene::CollisionUpdate()
{
}

void DungeonScene::Release()
{
	ObjMgr::GetInstance()->DeleteObj(OBJ_ENEMY);
	SafeDelete<AnimationMgr*>(m_pDarkCowAnimation);
	SafeDelete<AnimationMgr*>(m_pCreedWizardAnimation);
}