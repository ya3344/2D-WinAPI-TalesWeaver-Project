#include "stdafx.h"
#include "FieldScene.h"
#include "Obj.h"
#include "EnemyJelly.h"
#include "EnemyWolf.h"
#include "AnimationMgr.h"


FieldScene::FieldScene()
{
}


FieldScene::~FieldScene()
{
	Release();
}

void FieldScene::Initialize()
{
	BmpMgr::GetInstance()->AddBmp(L"Field", L"../Resource/Map/Field.bmp", true);

	MyBmp* pMapBmp = BmpMgr::GetInstance()->GetFindBmp(L"Field", true);
	if (pMapBmp == nullptr)
	{
		MESSAGE_BOX(L"MapBmp Find Fail!!");
	}

	mMapWidth = pMapBmp->GetWidth();
	mMapHeight = pMapBmp->GetHeight();

	m_hFieldDC = pMapBmp->GetMemDC();

	TileMgr::GetInstance()->Release();
	TileMgr::GetInstance()->LoadData(L"../Data/Field.dat");
	TileMgr::GetInstance()->Initialize();
	m_pPlayer = ObjMgr::GetInstance()->GetPlayerObj();
	//m_pPlayer->SetPos(1000.f, 400.f);
	m_pPlayer->SetPos(1670.f, 867.f);

	// Enemy Bmp 준비
	if (m_pJellyAnimation == nullptr)
	{
		m_pJellyAnimation = new AnimationMgr(ANIMATION_INFO(ANIM_TYPE::JELLY, L"../Resource/Monster/Jelly/%s/%s_%s/%s_%s%d.bmp",
			L"Jelly_%s_%s%d", (size_t)MONSTER_SCENE::DEATH, (OBJ_MOTION::MOTION_END / 2) ));
		m_pJellyAnimation->ObjLoadBmp(true);
	}

	if (m_pWolfAnimation == nullptr)
	{
		m_pWolfAnimation = new AnimationMgr(ANIMATION_INFO(ANIM_TYPE::WOLF, L"../Resource/Monster/BlueWolf/%s/%s_%s/%s_%s%d.bmp",
			L"Wolf_%s_%s%d", (size_t)MONSTER_SCENE::DEATH, (OBJ_MOTION::MOTION_END / 2)));
		m_pWolfAnimation->ObjLoadBmp(true);
	}


	for (size_t i = 0; i < TileMgr::GetInstance()->GetEnemyIndex().size(); ++i)
	{
		// EnemyObject 준비
		if (i % 5 == 0)
		{
			Obj* pObj = ObjFactory<EnemyWolf>::CreateObj(m_pPlayer, TileMgr::GetInstance()->GetEnemyIndex()[i]);
			ObjMgr::GetInstance()->AddObj(pObj, OBJ_ENEMY);
		}
		else if ( i<= 18)
		{
			Obj* pObj = ObjFactory<EnemyJelly>::CreateObj(m_pPlayer, TileMgr::GetInstance()->GetEnemyIndex()[i]);
			ObjMgr::GetInstance()->AddObj(pObj, OBJ_ENEMY);
		}
		
	}
	//for (INFO info : TileMgr::GetInstance()->GetEnemyIndex())
	//{
	//	// EnemyObject 준비
	//	Obj* pObj = ObjFactory<EnemyWolf>::CreateObj(m_pPlayer, info);
	//	ObjMgr::GetInstance()->AddObj(pObj, OBJ_ENEMY);
	//}

	// Sound
	SoundMgr::GetInstance()->StopAll();
	SoundMgr::GetInstance()->PlayBGM(L"Field.wav", CHANNAL::BGM);


}

void FieldScene::LateInit()
{
	ObjMgr::GetInstance()->LateInit();
}

void FieldScene::Update()
{
	ObjMgr::GetInstance()->Update();
	ScrollMgr::GetInstance()->Update();
	EffectMgr::GetInstance()->Update();
}

void FieldScene::LateUpdate()
{
	ObjMgr::GetInstance()->CollisionUpdate();
	ObjMgr::GetInstance()->LateUpdate();
	ImageMgr::GetInstance()->LateUpdate();
	EffectMgr::GetInstance()->LateUpdate();
	TileMgr::GetInstance()->LateUpdate();
}

void FieldScene::Render(HDC hDC)
{
	float scrollX = ScrollMgr::GetInstance()->GetScrollX();
	float scrollY = ScrollMgr::GetInstance()->GetScrollY();

	BitBlt(hDC, (int)(scrollX), (int)(scrollY), mMapWidth, mMapHeight, m_hFieldDC, 0, 0, SRCCOPY);

	ObjMgr::GetInstance()->Render(hDC);
	ImageMgr::GetInstance()->Render(hDC);
	EffectMgr::GetInstance()->Render(hDC);
	TileMgr::GetInstance()->Render(hDC);

	// Debug
	TCHAR szName[256] = L"";
	int count = 0;
	//for (Obj* pObj : ObjMgr::GetInstance()->GetObj(OBJ_ENEMY))
	//{
	//	
	//	if (count <= 3)
	//	{
	//		swprintf_s(szName, L"WolfX: %d WolfY: %d , WolfMaxNum: %d, Animation: %s",
	//			(int)pObj->GetInfo().X, (int)pObj->GetInfo().Y, TileMgr::GetInstance()->GetEnemyIndex().size(),
	//			dynamic_cast<EnemyWolf*>(pObj)->GetImageKey());
	//		TextOut(hDC, 50, 130 + (count * 20), szName, lstrlen(szName)); // 지정한 좌표에 폰트 출력!
	//	}

	//	++count;
	//}
}

void FieldScene::Release()
{
	SafeDelete<AnimationMgr*>(m_pJellyAnimation);
	SafeDelete<AnimationMgr*>(m_pWolfAnimation);

	ObjMgr::GetInstance()->DeleteObj(OBJ_ENEMY);
}

void FieldScene::CollisionUpdate()
{
	
}
