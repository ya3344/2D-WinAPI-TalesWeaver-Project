#include "stdafx.h"
#include "BossScene.h"
#include "Player.h"
#include "AnimationMgr.h"
#include "EnemyCreedWizard.h"
#include "EnemyBoss.h"

BossScene::BossScene()
{
}


BossScene::~BossScene()
{
	Release();
}

void BossScene::Initialize()
{
	BmpMgr::GetInstance()->AddBmp(L"BossRoom", L"../Resource/Map/BossRoom.bmp", true);

	MyBmp* pMapBmp = BmpMgr::GetInstance()->GetFindBmp(L"BossRoom", true);
	if (pMapBmp == nullptr)
	{
		MESSAGE_BOX(L"MapBmp Find Fail!!");
	}

	mMapWidth = pMapBmp->GetWidth();
	mMapHeight = pMapBmp->GetHeight();

	m_hBossRoomDC = pMapBmp->GetMemDC();

	TileMgr::GetInstance()->Release();
	TileMgr::GetInstance()->LoadData(L"../Data/BossRoom.dat");
	TileMgr::GetInstance()->Initialize();

	m_pPlayer = ObjMgr::GetInstance()->GetPlayerObj();
	m_pPlayer->SetPos(816.f, 432.f);

	// Enemy Bmp ÁØºñ
	if (m_pCreedWizardAnimation == nullptr)
	{
		m_pCreedWizardAnimation = new AnimationMgr(ANIMATION_INFO(ANIM_TYPE::CREEDWIZARD, L"../Resource/Monster/CreedWizard/%s/%s_%s/%s_%s%d.bmp",
			L"CreedWizard_%s_%s%d", (size_t)MONSTER_SCENE::DEATH, (OBJ_MOTION::MOTION_END / 2)));
		m_pCreedWizardAnimation->ObjLoadBmp(true);
	}

	if (m_pBossAnimation == nullptr)
	{
		m_pBossAnimation = new AnimationMgr(ANIMATION_INFO(ANIM_TYPE::BOSS, L"../Resource/Monster/Boss/%s/%s_%s/%s_%s%d.bmp",
			L"Boss_%s_%s%d", (size_t)BOSS_SCENE::DEATH, (OBJ_MOTION::MOTION_END / 2)));
		m_pBossAnimation->ObjLoadBmp(true);
	}

	for (size_t i = 1; i < TileMgr::GetInstance()->GetEnemyIndex().size(); ++i)
	{
		if (i <= 2)
		{
			Obj* pObj = ObjFactory<EnemyCreedWizard>::CreateObj(m_pPlayer, TileMgr::GetInstance()->GetEnemyIndex()[i]);
			ObjMgr::GetInstance()->AddObj(pObj, OBJ_ENEMY);

		}
		else
		{
			Obj* pObj = ObjFactory<EnemyBoss>::CreateObj(m_pPlayer, TileMgr::GetInstance()->GetEnemyIndex()[i]);
			ObjMgr::GetInstance()->AddObj(pObj, OBJ_ENEMY);
		}
	}

	//Obj* pObj = ObjFactory<EnemyBoss>::CreateObj(m_pPlayer, TileMgr::GetInstance()->GetEnemyIndex()[0]);
	//ObjMgr::GetInstance()->AddObj(pObj, OBJ_ENEMY);
	// Effect Bmp Setting
	EffectMgr::GetInstance()->AddEffect(L"BossCasting", L"../Resource/Effect/BossCasting/BossCasting%d.bmp",
		(size_t)EFFECT_FRAME::CASTING, (size_t)FRAME_SPEED::CASTING, IMAGE_TYPE::ONLY_VIEW, true);

	EffectMgr::GetInstance()->AddEffect(L"Bullet", L"../Resource/Effect/Bullet/Bullet%d.bmp",
		(size_t)EFFECT_FRAME::BULLET, (size_t)FRAME_SPEED::BULLET, IMAGE_TYPE::ATTACK, true);

	EffectMgr::GetInstance()->AddEffect(L"PBullet", L"../Resource/Effect/PBullet/PBullet%d.bmp",
		(size_t)EFFECT_FRAME::PBULLET, (size_t)FRAME_SPEED::PBULLET, IMAGE_TYPE::ATTACK, true);

	EffectMgr::GetInstance()->AddEffect(L"ElecShock", L"../Resource/Effect/ElecShock/ElecShock%d.bmp",
		(size_t)EFFECT_FRAME::ELECSHOCK, (size_t)FRAME_SPEED::ELECSHOCK, IMAGE_TYPE::ATTACK, false);

	EffectMgr::GetInstance()->AddEffect(L"Freeze", L"../Resource/Effect/Freeze/Freeze%d.bmp",
		(size_t)EFFECT_FRAME::FREEZE, (size_t)FRAME_SPEED::FREEZE, IMAGE_TYPE::ATTACK, false);

	SoundMgr::GetInstance()->StopAll();
	SoundMgr::GetInstance()->PlayBGM(L"BossScenePrev.MP3", BGM);
}

void BossScene::LateInit()
{
	ObjMgr::GetInstance()->LateInit();
}

void BossScene::Update()
{
	ObjMgr::GetInstance()->Update();
	ScrollMgr::GetInstance()->Update();
	EffectMgr::GetInstance()->Update();

}

void BossScene::LateUpdate()
{
	ObjMgr::GetInstance()->CollisionUpdate();
	ObjMgr::GetInstance()->LateUpdate();
	ImageMgr::GetInstance()->LateUpdate();
	EffectMgr::GetInstance()->LateUpdate();
	TileMgr::GetInstance()->LateUpdate();
}

void BossScene::Render(HDC hDC)
{
	float scrollX = ScrollMgr::GetInstance()->GetScrollX();
	float scrollY = ScrollMgr::GetInstance()->GetScrollY();

	BitBlt(hDC, (int)(scrollX), (int)(scrollY), mMapWidth, mMapHeight, m_hBossRoomDC, 0, 0, SRCCOPY);

	ObjMgr::GetInstance()->Render(hDC);
	ImageMgr::GetInstance()->Render(hDC);
	EffectMgr::GetInstance()->Render(hDC);
	TileMgr::GetInstance()->Render(hDC);
}

void BossScene::Release()
{
	ObjMgr::GetInstance()->DeleteObj(OBJ_ENEMY);
	SafeDelete<AnimationMgr*>(m_pCreedWizardAnimation);
	SafeDelete<AnimationMgr*>(m_pBossAnimation);
}
