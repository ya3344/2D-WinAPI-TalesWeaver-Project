#include "stdafx.h"
#include "VillageScene.h"
#include "Player.h"
#include "Mouse.h"
#include "Weapon.h"
#include "UI.h"

VillageScene::VillageScene()
{
}


VillageScene::~VillageScene()
{
	Release();
}

void VillageScene::Initialize()
{
	// 맵 Bmp 준비
	BmpMgr::GetInstance()->AddBmp(L"House", L"../Resource/Map/House.bmp", true);
	BmpMgr::GetInstance()->AddBmp(L"Town2", L"../Resource/Map/Town2.bmp", true);
	
	BmpMgr::GetInstance()->AddBmp(L"Tile", L"../Resource/Tile.bmp", false);

	TileMgr::GetInstance()->Release();
	TileMgr::GetInstance()->LoadData(L"../Data/House.dat");
	TileMgr::GetInstance()->Initialize();

	// Weapon Effect Bmp 준비
	EffectLoadBmp();
	m_pPlayer = ObjMgr::GetInstance()->GetPlayerObj();

	// UI Bmp 준비
	BmpMgr::GetInstance()->AddBmp(L"Interface", L"../Resource/UI/Interface.bmp", false);
	Image* pInterface = ImageFactory<UI>::CreateImage(130.f, 80.f, L"Interface");
	ImageMgr::GetInstance()->AddImage(pInterface, IMAGE_UI);

	BmpMgr::GetInstance()->AddBmp(L"QuickSlot", L"../Resource/UI/QuickSlot.bmp", false);
	Image* pQuickSlot = ImageFactory<UI>::CreateImage(70.f, 715.f, L"QuickSlot");
	ImageMgr::GetInstance()->AddImage(pQuickSlot, IMAGE_UI);

	BmpMgr::GetInstance()->AddBmp(L"HpBar", L"../Resource/UI/HpBar/HpBar0.bmp", false);
	Image* pHpBar = ImageFactory<UI>::CreateImage(m_pPlayer, 143.f, 45.f, L"HpBar");
	ImageMgr::GetInstance()->AddImage(pHpBar, IMAGE_UI);

	BmpMgr::GetInstance()->AddBmp(L"MpBar", L"../Resource/UI/MpBar/MpBar0.bmp", false);
	Image* pMpBar = ImageFactory<UI>::CreateImage(m_pPlayer, 143.f, 59.f, L"MpBar");
	ImageMgr::GetInstance()->AddImage(pMpBar, IMAGE_UI);

	BmpMgr::GetInstance()->AddBmp(L"SpBar", L"../Resource/UI/SpBar/SpBar0.bmp", false);
	Image* pSpBar = ImageFactory<UI>::CreateImage(m_pPlayer, 143.f, 74.f, L"SpBar");
	ImageMgr::GetInstance()->AddImage(pSpBar, IMAGE_UI);

	// Monster Related Bmp Setting
	EffectMgr::GetInstance()->AddEffect(L"Block", L"../Resource/Effect/Block/Block%d.bmp",
		(size_t)EFFECT_FRAME::BLOCK, (size_t)FRAME_SPEED::SMOKE, IMAGE_TYPE::ONLY_VIEW, false);
	EffectMgr::GetInstance()->AddEffect(L"Pop", L"../Resource/Effect/Pop/Pop%d.bmp",
		(size_t)EFFECT_FRAME::POP, (size_t)FRAME_SPEED::POP, IMAGE_TYPE::ATTACK, false);
	EffectMgr::GetInstance()->AddEffect(L"MonsterHit", L"../Resource/Effect/MonsterHit/MonsterHit%d.bmp",
		(size_t)EFFECT_FRAME::MONSTER_HIT, (size_t)FRAME_SPEED::MONSTER_HIT, IMAGE_TYPE::ONLY_VIEW, false);
	EffectMgr::GetInstance()->AddEffect(L"Dust", L"../Resource/Effect/Dust/Dust%d.bmp",
		(size_t)EFFECT_FRAME::DUST, (size_t)FRAME_SPEED::DUST, IMAGE_TYPE::ONLY_VIEW, false);

	// Sound
	SoundMgr::GetInstance()->StopAll();
	SoundMgr::GetInstance()->PlayBGM(L"Village.wav", BGM);
}

void VillageScene::LateInit()
{
	ObjMgr::GetInstance()->LateInit();
	Obj* pMouse = ObjMgr::GetInstance()->GetObj(OBJ_MOUSE).back();
	dynamic_cast<Mouse*>(pMouse)->SetFieldScene(true);
}

void VillageScene::Update()
{
	ObjMgr::GetInstance()->Update();
	ScrollMgr::GetInstance()->Update();
	EffectMgr::GetInstance()->Update();
}

void VillageScene::LateUpdate()
{
	ObjMgr::GetInstance()->LateUpdate();
	ImageMgr::GetInstance()->LateUpdate();
	EffectMgr::GetInstance()->LateUpdate();
	TileMgr::GetInstance()->LateUpdate();
}

void VillageScene::Render(HDC hDC)
{
	float scrollX = ScrollMgr::GetInstance()->GetScrollX();
	float scrollY = ScrollMgr::GetInstance()->GetScrollY();

	const TCHAR* pStageKey = dynamic_cast<Player*>(m_pPlayer)->GetStageKey();

	m_pStageCurrentKey = pStageKey;

	if (m_pStageCurrentKey != m_pStagePreKey)
	{
		m_pMapBmp = BmpMgr::GetInstance()->GetFindBmp(m_pStageCurrentKey, true);
		if (m_pMapBmp == nullptr)
		{
			MESSAGE_BOX(L"MapBmp Find Fail!!");
		}

		mMapWidth = m_pMapBmp->GetWidth();
		mMapHeight = m_pMapBmp->GetHeight();

		m_pStagePreKey = m_pStageCurrentKey;
	}


	HDC hHouseDC = m_pMapBmp->GetMemDC();
	BitBlt(hDC, (int)(scrollX), (int)(scrollY), mMapWidth, mMapHeight, hHouseDC, 0, 0, SRCCOPY);
	
	ObjMgr::GetInstance()->Render(hDC);
	ImageMgr::GetInstance()->Render(hDC);
	EffectMgr::GetInstance()->Render(hDC);
	TileMgr::GetInstance()->Render(hDC);
}



void VillageScene::EffectLoadBmp()
{
	// Animation Bmp Setting
	TCHAR imageLocation[256] = L"";
	TCHAR imageKey[64] = L"";
	TCHAR imageMotion[(size_t)OBJ_MOTION::MOTION_END][16] = { L"D", L"L",  L"LD", L"LU", L"U", L"RD", L"R", L"RU" };

	EffectMgr::GetInstance()->AddEffect(L"WarpGate", L"../Resource/Effect/WarpGate/WarpGate%d.bmp", 
		(size_t)EFFECT_FRAME::WRAPGATE, (size_t)EFFECT_FRAME::WRAPGATE, IMAGE_TYPE::ONLY_VIEW, false);


	for (int i = 0; i < (size_t)OBJ_MOTION::MOTION_END; ++i)
	{
		if (i <= 4)
		{
			swprintf_s(imageKey, L"HorCut_%s", imageMotion[i]);
			swprintf_s(imageLocation, L"../Resource/Effect/HorCut_%s/HorCut_%s%s", imageMotion[i], imageMotion[i], L"%d.bmp");
			EffectMgr::GetInstance()->AddEffect(imageKey, imageLocation, (size_t)PLAYER_FRAME::HORCUT, (size_t)FRAME_SPEED::HORCUT +75, IMAGE_TYPE::ONLY_VIEW, false);
		}
		// 좌우반전용 이미지는 파일위치값에 동작키를 보낸다.
		else
		{
			swprintf_s(imageKey, L"HorCut_%s", imageMotion[i]);
			EffectMgr::GetInstance()->AddEffect(imageKey, imageMotion[i], (size_t)PLAYER_FRAME::HORCUT, (size_t)FRAME_SPEED::HORCUT + 75, IMAGE_TYPE::ONLY_VIEW, false);
		}

		if (i <= 4)
		{
			swprintf_s(imageKey, L"Stab_%s", imageMotion[i]);
			swprintf_s(imageLocation, L"../Resource/Effect/Stab_%s/Stab_%s%s", imageMotion[i], imageMotion[i], L"%d.bmp");
			EffectMgr::GetInstance()->AddEffect(imageKey, imageLocation, (size_t)PLAYER_FRAME::STAB, (size_t)FRAME_SPEED::STAB + 50, IMAGE_TYPE::ONLY_VIEW, false);
		}
		// 좌우반전용 이미지는 파일위치값에 동작키를 보낸다.
		else
		{
			swprintf_s(imageKey, L"Stab_%s", imageMotion[i]);
			EffectMgr::GetInstance()->AddEffect(imageKey, imageMotion[i], (size_t)PLAYER_FRAME::STAB, (size_t)FRAME_SPEED::STAB + 50, IMAGE_TYPE::ONLY_VIEW, false);
		}

		if (i <= 4)
		{
			swprintf_s(imageKey, L"VerCut_%s", imageMotion[i]);
			swprintf_s(imageLocation, L"../Resource/Effect/VerCut_%s/VerCut_%s%s", imageMotion[i], imageMotion[i], L"%d.bmp");
			EffectMgr::GetInstance()->AddEffect(imageKey, imageLocation, (size_t)PLAYER_FRAME::VERCUT, (size_t)FRAME_SPEED::VERCUT + 30, IMAGE_TYPE::ONLY_VIEW, false);
		}
		// 좌우반전용 이미지는 파일위치값에 동작키를 보낸다.
		else
		{
			swprintf_s(imageKey, L"VerCut_%s", imageMotion[i]);
			EffectMgr::GetInstance()->AddEffect(imageKey, imageMotion[i], (size_t)PLAYER_FRAME::VERCUT, (size_t)FRAME_SPEED::VERCUT + 30, IMAGE_TYPE::ONLY_VIEW, false);
		}
	}


}

void VillageScene::Release()
{

}
