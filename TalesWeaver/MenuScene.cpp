#include "stdafx.h"
#include "MenuScene.h"
#include "Button.h"
#include "Mouse.h"

MenuScene::MenuScene()
{

}

MenuScene::~MenuScene()
{
	Release();
}

void MenuScene::Initialize()
{
	BmpMgr::GetInstance()->AddBmp(L"MainMenu", L"../Resource/MenuScene/MainMenu.bmp", true);

	BmpMgr::GetInstance()->AddBmp(L"GameStart0", L"../Resource/MenuScene/GameStart0.bmp", true);
	BmpMgr::GetInstance()->AddBmp(L"GameStart1", L"../Resource/MenuScene/GameStart1.bmp", true);

	Image* pButton = ImageFactory<Button>::CreateImage(440.f, 515.f, L"GameStart0");
	ImageMgr::GetInstance()->AddImage(pButton, IMAGE_BUTTON);

	BmpMgr::GetInstance()->AddBmp(L"Opening0", L"../Resource/MenuScene/opening0.bmp", true);
	BmpMgr::GetInstance()->AddBmp(L"Opening1", L"../Resource/MenuScene/opening1.bmp", true);

	pButton = ImageFactory<Button>::CreateImage(440.f, 545.f, L"Opening0");
	ImageMgr::GetInstance()->AddImage(pButton, IMAGE_BUTTON);

	BmpMgr::GetInstance()->AddBmp(L"Credit0", L"../Resource/MenuScene/Credit0.bmp", true);
	BmpMgr::GetInstance()->AddBmp(L"Credit1", L"../Resource/MenuScene/Credit1.bmp", true);

	pButton = ImageFactory<Button>::CreateImage(440.f, 575.f, L"Credit0");
	ImageMgr::GetInstance()->AddImage(pButton, IMAGE_BUTTON);

	BmpMgr::GetInstance()->AddBmp(L"GameExit0", L"../Resource/MenuScene/GameExit0.bmp", true);
	BmpMgr::GetInstance()->AddBmp(L"GameExit1", L"../Resource/MenuScene/GameExit1.bmp", true);

	pButton = ImageFactory<Button>::CreateImage(439.f, 610.f, L"GameExit0");
	ImageMgr::GetInstance()->AddImage(pButton, IMAGE_BUTTON);



	m_pMouse = ObjMgr::GetInstance()->GetObj(OBJ_MOUSE).back();

	// Potion Effect Bmp ÁØºñ
	EffectMgr::GetInstance()->AddEffect(L"HpPotion", L"../Resource/Effect/HpPotion/%d.bmp",
		(size_t)EFFECT_FRAME::POTION, (size_t)FRAME_SPEED::POTION, IMAGE_TYPE::ONLY_VIEW, false);

	EffectMgr::GetInstance()->AddEffect(L"MpPotion", L"../Resource/Effect/MpPotion/%d.bmp",
		(size_t)EFFECT_FRAME::POTION, (size_t)FRAME_SPEED::POTION, IMAGE_TYPE::ONLY_VIEW, false);

	EffectMgr::GetInstance()->AddEffect(L"GreenPotion", L"../Resource/Effect/GreenPotion/%d.bmp",
		(size_t)EFFECT_FRAME::POTION, (size_t)FRAME_SPEED::POTION, IMAGE_TYPE::ONLY_VIEW, false);

	// Skill Effect Bmp Setting
	EffectMgr::GetInstance()->AddEffect(L"BoltIcon", L"../Resource/UI/SkillIcon/BoltIcon%d.bmp",
		0, 1000, IMAGE_TYPE::ONLY_VIEW, false);

	EffectMgr::GetInstance()->AddEffect(L"MultAttackIcon", L"../Resource/UI/SkillIcon/MultAttackIcon%d.bmp",
		0, 1000, IMAGE_TYPE::ONLY_VIEW, false);

	EffectMgr::GetInstance()->AddEffect(L"NuckBackIcon", L"../Resource/UI/SkillIcon/NuckBackIcon%d.bmp",
		0, 1000, IMAGE_TYPE::ONLY_VIEW, false);

	// Attack Effect Setting
	EffectMgr::GetInstance()->AddEffect(L"ThunderBolt", L"../Resource/Effect/ThunderBolt/ThunderBolt%d.bmp",
		(size_t)EFFECT_FRAME::THUNDERBOLT, (size_t)FRAME_SPEED::THUNDERBOLT, IMAGE_TYPE::ATTACK, false);

	EffectMgr::GetInstance()->AddEffect(L"BossSkill", L"../Resource/Effect/WaterJail/BossSkill%d.bmp",
		(size_t)EFFECT_FRAME::BOSSSKILL, (size_t)FRAME_SPEED::BOSSSKILL, IMAGE_TYPE::ATTACK, false);

	EffectMgr::GetInstance()->AddEffect(L"MultiAttack", L"../Resource/Effect/MultiAttack/MultiAttack%d.bmp",
		(size_t)EFFECT_FRAME::MULTIATTACK, (size_t)FRAME_SPEED::MULTIATTACK, IMAGE_TYPE::ATTACK, false);

	EffectMgr::GetInstance()->AddEffect(L"Hand", L"../Resource/Effect/Hand/Hand%d.bmp",
		(size_t)EFFECT_FRAME::HAND, (size_t)FRAME_SPEED::HAND, IMAGE_TYPE::ATTACK, false);

	EffectMgr::GetInstance()->AddEffect(L"Casting", L"../Resource/Effect/Casting/Casting%d.bmp",
		(size_t)EFFECT_FRAME::CASTING, (size_t)FRAME_SPEED::CASTING, IMAGE_TYPE::ONLY_VIEW, false);

	EffectMgr::GetInstance()->AddEffect(L"BotFire", L"../Resource/Effect/BotFire/BotFire%d.bmp",
		(size_t)EFFECT_FRAME::BOTFIRE, (size_t)FRAME_SPEED::BOTFIRE, IMAGE_TYPE::ATTACK, false);

	EffectMgr::GetInstance()->AddEffect(L"WizardSkill", L"../Resource/Effect/WizardSkill/WizardSkill%d.bmp",
		(size_t)EFFECT_FRAME::WIZARDSKILL, (size_t)FRAME_SPEED::WIZARDSKILL, IMAGE_TYPE::ATTACK, false);

	EffectMgr::GetInstance()->AddEffect(L"WarpGateRed", L"../Resource/Effect/WarpGateRed/WarpGate%d.bmp",
		(size_t)EFFECT_FRAME::WRAPGATE, (size_t)EFFECT_FRAME::WRAPGATE, IMAGE_TYPE::ONLY_VIEW, false);
}

void MenuScene::LateInit()
{
}

void MenuScene::Update()
{
	m_pMouse->Update();
	EffectMgr::GetInstance()->Update();

	POINT pt;


	for (Image* pImage : ImageMgr::GetInstance()->GetImage(IMAGE_BUTTON))
	{
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x = (long)(pt.x - (pImage->GetInfo().width / 2));
		pt.y = (long)(pt.y - (pImage->GetInfo().height));
		if (PtInRect(&pImage->GetRect(), pt))
		{
			if (KeyMgr::GetInstance()->KeyDown(VK_LBUTTON))
			{
				if (!lstrcmp(pImage->GetBmpKey(), L"GameStart1") )
				{
					SceneMgr::GetInstance()->ChangeScene(SCENE_ID::VILLAGE);
					SoundMgr::GetInstance()->PlaySound(L"Enter.wav", BUTTON_SOUND);
					return;
				}
				else if (!lstrcmp(pImage->GetBmpKey(), L"GameExit1"))
				{
					SoundMgr::GetInstance()->PlaySound(L"Enter.wav", BUTTON_SOUND);
					DestroyWindow(g_hWnd);

				}
			}
		}
	}
}

void MenuScene::LateUpdate()
{
	EffectMgr::GetInstance()->LateUpdate();
	ImageMgr::GetInstance()->LateUpdate();
}

void MenuScene::Render(HDC hDC)
{
	MyBmp* pBmp = BmpMgr::GetInstance()->GetFindBmp(L"MainMenu", true);

	if (pBmp == nullptr)
		return;

	BitBlt(hDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, pBmp->GetMemDC(), 0, 0, SRCCOPY);

	ImageMgr::GetInstance()->Render(hDC);
	EffectMgr::GetInstance()->Render(hDC);
}

void MenuScene::Release()
{
	for (Image* pImage : ImageMgr::GetInstance()->GetImage(IMAGE_BUTTON))
	{
		SafeDelete<Image*>(pImage);
	}

	ImageMgr::GetInstance()->GetImage(IMAGE_BUTTON).clear();
}


