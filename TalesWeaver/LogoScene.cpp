#include "stdafx.h"
#include "LogoScene.h"
#include "Mouse.h"
#include "Player.h"
#include "Weapon.h"

LogoScene::LogoScene()
{
}


LogoScene::~LogoScene()
{
	Release();
}

void LogoScene::Initialize()
{
	BmpMgr::GetInstance()->AddBmp(L"Logo", L"../Resource/Logo/Logo.bmp", true);

	// Mouse Image Bmp Setting
	EffectMgr::GetInstance()->AddEffect(L"MouseNormal", L"../Resource/Mouse/Normal/Normal_%d.bmp",
		(size_t)EFFECT_FRAME::MOUSE_NORMAL, (size_t)FRAME_SPEED::MOUSE_NORMAL, IMAGE_TYPE::ONLY_VIEW, false);
	EffectMgr::GetInstance()->AddEffect(L"MouseNoPass", L"../Resource/Mouse/NoPass/NoPass_%d.bmp",
		(size_t)EFFECT_FRAME::MOUSE_NOPASS, (size_t)FRAME_SPEED::MOUSE_NORMAL, IMAGE_TYPE::ONLY_VIEW, false);
	EffectMgr::GetInstance()->AddEffect(L"MouseGate", L"../Resource/Mouse/Gate/Gate%d.bmp",
		(size_t)EFFECT_FRAME::MOUSE_GATE, (size_t)FRAME_SPEED::MOUSE_NORMAL, IMAGE_TYPE::ONLY_VIEW, false);
	EffectMgr::GetInstance()->AddEffect(L"MouseBattle", L"../Resource/Mouse/Battle/Battle_%d.bmp",
		(size_t)EFFECT_FRAME::MOUSE_BATTLE, (size_t)FRAME_SPEED::MOUSE_NORMAL, IMAGE_TYPE::ONLY_VIEW, false);
	EffectMgr::GetInstance()->AddEffect(L"MouseMagic", L"../Resource/Mouse/Magic/%d.bmp",
		(size_t)EFFECT_FRAME::MOUSE_MAGIC, (size_t)FRAME_SPEED::MOUSE_NORMAL, IMAGE_TYPE::ONLY_VIEW, false);

	Obj* pObj = ObjFactory<Player>::CreateObj(INFO(667.f, 480.f, 0.f, 0.f));
	ObjMgr::GetInstance()->AddObj(pObj, OBJ_PLAYER);

	m_pPlayer = ObjMgr::GetInstance()->GetPlayerObj();

	pObj = ObjFactory<Weapon>::CreateObj(m_pPlayer);
	ObjMgr::GetInstance()->AddObj(pObj, OBJ_WEAPON);

	m_pMouse = ObjFactory<Mouse>::CreateObj();
	ObjMgr::GetInstance()->AddObj(m_pMouse, OBJ_MOUSE);

	// Sound
	SoundMgr::GetInstance()->PlayBGM(L"MainMenu.wav", CHANNAL::BGM);
}

void LogoScene::LateInit()
{
}

void LogoScene::Update()
{
	m_pMouse->Update();
	EffectMgr::GetInstance()->Update();

	if (KeyMgr::GetInstance()->KeyDown(VK_RETURN))
	{
		SceneMgr::GetInstance()->ChangeScene(SCENE_ID::MENU);
	}

	//if (SoundMgr::GetInstance()->IsPlayCheck(CHANNAL::BGM) != 1)
	//{
	//	SoundMgr::GetInstance()->PlayBGM(L"MainMenu.wav");
	//}
}

void LogoScene::LateUpdate()
{
	EffectMgr::GetInstance()->LateUpdate();
}

void LogoScene::Render(HDC hDC)
{
	MyBmp* pBmp = BmpMgr::GetInstance()->GetFindBmp(L"Logo", true);

	if (pBmp == nullptr)
		return;

	BitBlt(hDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, pBmp->GetMemDC(), 0, 0, SRCCOPY);

	EffectMgr::GetInstance()->Render(hDC);
}

void LogoScene::Release()
{

}
