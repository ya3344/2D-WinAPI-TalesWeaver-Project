#include "stdafx.h"
#include "Player.h"
#include "Bullet.h"
#include "Satellite.h"
#include "Line.h"
#include "AnimationMgr.h"
#include "AStar.h"
#include "Mouse.h"
#include "Effect.h"

Player::Player() 
{

}

Player::~Player()
{
	Release();
}

void Player::Initialize()
{
	mAttribute.speed = PLAYER_SPEED;
	mAttribute.degree = 90.f;
	mBarrelX = mInfo.X + ( BARREL_DISTANCE *(cosf(ToRadian(mAttribute.degree))));
	mBarrelY = mInfo.Y - ( BARREL_DISTANCE *(sinf(ToRadian(mAttribute.degree))));

	mAttribute.type = TYPE_PLAYER;
	mAttribute.attack = 1.f;

	mAttribute.maxHP = PALYER_MAXHP;
	mAttribute.currentHP = mAttribute.maxHP;

	mAttribute.maxMP = PALYER_MAXMP;
	mAttribute.currentMP = mAttribute.maxMP;

	mAttribute.maxSP = PALYER_MAXSP;
	mAttribute.currentSP = mAttribute.maxSP;

	// Animation Init
	if (m_pAnimationMgr == nullptr)
	{
		m_pAnimationMgr = new AnimationMgr(ANIMATION_INFO(ANIM_TYPE::PLAYER, L"../Resource/Player/%s/%s_%s/%s_%s%d.bmp",
			L"Player_%s_%s%d", (size_t)PLAYER_SCENE::REST, (size_t)OBJ_MOTION::MOTION_END));
		m_pAnimationMgr->ObjLoadBmp(false);
	}
		

	m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)PLAYER_FRAME::STAND, L"Stand", GetTickCount(), (size_t)FRAME_SPEED::STAND));
	m_pImageKey = L"Player_Stand_D";
	m_pCurMotion = L"D";

	// AStar Init
	if (m_pAStar == nullptr)
	{
		m_pAStar = new AStar();
		m_pAStar->SetObjID(OBJ_PLAYER);
	}
		
	// Stage Init
	m_pStageKey = L"House";
}

void Player::LateInit()
{
	//Obj* pObj = ObjFactory<Satellite>::CreateObj(ObjMgr::GetInstance()->GetObj(OBJ_PLAYER).back());
	//ObjMgr::GetInstance()->AddObj(pObj, OBJ_SATELLITE);
	m_pMouse = ObjMgr::GetInstance()->GetObj(OBJ_MOUSE).back();
	m_pThunderBolt = EffectMgr::GetInstance()->GetFindEffect(L"ThunderBolt", false);
	m_pBossSkill = EffectMgr::GetInstance()->GetFindEffect(L"BossSkill", false);
	m_pMultiAttack = EffectMgr::GetInstance()->GetFindEffect(L"MultiAttack", false);
}

int Player::Update()
{
	static bool bOverlapSkillSound = false;

	if (m_pBossSkill->GetAlpha() > 0)
	{
		mAttribute.degree += mAttribute.speed * 3.f;

		float X = mInfo.X + 100 * (cosf(ToRadian(mAttribute.degree)));
		float Y = mInfo.Y - 100 * (sinf(ToRadian(mAttribute.degree)));

		mRadian = MathMgr::CaclRadian(this, INFO(X, Y, 0 , 0));

		PlayRotation(mRadian);
		
		HorCut();
		Stab();
		VerCut();
		CollisionEX();

		if (dynamic_cast<Effect*>(m_pBossSkill)->GetAnimationMgr()->GetFrameInfo().frameStart == 1)
		{
			if (bOverlapSkillSound == false)
			{
				SoundMgr::GetInstance()->PlaySound(L"MultiAttack1.WAV", PLAYER_SOUND);
				bOverlapSkillSound = true;
			}
		}
		else
		{
			bOverlapSkillSound = false;
		}

	}

	if (m_pMultiAttack->GetAlpha() > 0)
	{
		mAttribute.degree += mAttribute.speed * 4;

		float X = mInfo.X + 100 * (cosf(ToRadian(mAttribute.degree)));
		float Y = mInfo.Y - 100 * (sinf(ToRadian(mAttribute.degree)));

		mRadian = MathMgr::CaclRadian(this, INFO(X, Y, 0, 0));

		PlayRotation(mRadian);

		Stab();


		if (dynamic_cast<Effect*>(m_pMultiAttack)->GetAnimationMgr()->GetFrameInfo().frameStart == 1)
		{
			if (bOverlapSkillSound == false)
			{
				SoundMgr::GetInstance()->PlaySound(L"MultiAttack3.WAV", PLAYER_SOUND);
				bOverlapSkillSound = true;
			}
		}
		else
		{
			bOverlapSkillSound = false;
		}

		if (dynamic_cast<Effect*>(m_pMultiAttack)->GetAnimationMgr()->GetFrameInfo().frameStart == 20)
		{
			if (bOverlapSkillSound == false)
			{
				SoundMgr::GetInstance()->PlaySound(L"MultiAttack2.WAV", PLAYER_SOUND);
				bOverlapSkillSound = true;
			}
		}
		else
		{
			bOverlapSkillSound = false;
		}

	}


	if (KeyMgr::GetInstance()->KeyDown(VK_LBUTTON))
	{
		mTargetIndex = MathMgr::CalTargetIndex();

		if (dynamic_cast<Mouse*>(m_pMouse)->GetMouseMagic())
		{
			POINT target = MathMgr::CalPoint();
			m_pThunderBolt->SetEffect((float)target.x, (float)target.y, 200, false, true, 3.0f);
			mAttribute.currentMP -= 100;
			if (mAttribute.currentMP < 0)
				mAttribute.currentMP = 0;

			SoundMgr::GetInstance()->PlaySound(L"ThunderBolt.WAV", PLAYER_SOUND);
			dynamic_cast<Mouse*>(m_pMouse)->SetMouseMagic(false);

			mTargetIndex = MathMgr::CalTargetIndex();
			if (mTargetIndex != -1)
			{
				mRadian = MathMgr::CaclRadian(this, TileMgr::GetInstance()->GetTile(mTargetIndex));
				PlayRotation(mRadian);
			}

		}
		else if (mTargetIndex != -1)
		{
			if (m_pAStar->AStarStart(mTileIndex, mTargetIndex))
			{
				mAttribute.mode = MODE::MOVE;
				//MessageBox(g_hWnd, L"AStar Sucess!", nullptr, MB_OK);
			}
			else
			{
				//MessageBox(g_hWnd, L"AStar Fail!", nullptr, MB_OK);
			}
		}
	}


	if (KeyMgr::GetInstance()->KeyDown(VK_RBUTTON))
	{
		mTargetIndex = MathMgr::CalTargetIndex();

		if (mTargetIndex != -1)
		{
			mRadian = MathMgr::CaclRadian(this, TileMgr::GetInstance()->GetTile(mTargetIndex));
			PlayRotation(mRadian);
		}
	}

	// Collision Radian Cal
	if (m_pCurMotion == L"LD")
	{
		mCollisionRadian = -2.6f;
	}
	else if (m_pCurMotion == L"LU")
	{
		mCollisionRadian = 2.3f;
	}
	else if (m_pCurMotion == L"RD")
	{
		mCollisionRadian = -0.4f;
	}
	else
	{
		mCollisionRadian = mRadian;
	}
	static bool damagedSound = false;
	switch (mAttribute.mode)
	{
	case MODE::DAMAGED:
		mCurScene = (size_t)PLAYER_SCENE::DAMAGE;
		break;
	case MODE::ATTACK:
		break;
	case MODE::MOVE:
		// CollisionCheck
		if (mAttribute.bCollision && m_pAStar->GetBesRoadSpace().size() < 2)
		{
			mAttribute.mode = MODE::STAND;
			m_pAStar->Release();
		}

		PlayerMove();
		break;
	case MODE::REST:
		mCurScene = (size_t)PLAYER_SCENE::REST;
		break;
	case MODE::STAND:
		m_pAStar->Release();
		mCurScene = (size_t)PLAYER_SCENE::STAND;
		damagedSound = false;
		break;
	default:
		m_pAStar->Release();
		mCurScene = (size_t)PLAYER_SCENE::STAND;
		break;

	}
	
	// Test
	if (KeyMgr::GetInstance()->KeyUp(L'G'))
	{
		m_bCollisionDraw = !m_bCollisionDraw;
	}

	if (mAttribute.mode == MODE::DAMAGED)
	{
		if (  m_pAnimationMgr->GetFrameInfo().frameEnd == m_pAnimationMgr->GetFrameInfo().frameStart)
		{
			//EffectMgr::GetInstance()->GetFindEffect(L"MonsterHit", true)->SetEffect(mInfo.X, mInfo.Y, 150, false);
			mAttribute.mode = MODE::STAND;
		}
	}
	else if (KeyMgr::GetInstance()->KeyPressing(L'W'))
	{
		HorCut();
	
		if (m_pAnimationMgr->GetFrameInfo().frameStart == 2)
		{
			if (m_bOverlapSound == false)
			{
				SoundMgr::GetInstance()->PlaySound(L"Stab.WAV", PLAYER_SOUND);
				m_bOverlapSound = true;
			}
		}
		else
		{
			m_bOverlapSound = false;
		}

		
	}
	else if (KeyMgr::GetInstance()->KeyPressing(L'Q'))
	{
		Stab();
		
		if (m_pAnimationMgr->GetFrameInfo().frameStart == 0)
		{
			if (m_bOverlapSound == false)
			{
				SoundMgr::GetInstance()->PlaySound(L"Stab.WAV", PLAYER_SOUND);
				m_bOverlapSound = true;
			}
		}
		else
		{
			m_bOverlapSound = false;
		}
	}
	else if (KeyMgr::GetInstance()->KeyPressing(L'E'))
	{
		VerCut();

		if (m_pAnimationMgr->GetFrameInfo().frameStart == 2)
		{
			if (m_bOverlapSound == false)
			{
				SoundMgr::GetInstance()->PlaySound(L"Stab.WAV", PLAYER_SOUND);
				m_bOverlapSound = true;
			}
		}
		else
		{
			m_bOverlapSound = false;
		}
	}
	else if (KeyMgr::GetInstance()->KeyPressing(L'X'))
	{
		mAttribute.mode = MODE::REST;
	}
	else
	{
		if(m_pAStar->GetBesRoadSpace().size() == 0)
			mAttribute.mode = MODE::STAND;
		else
			mAttribute.mode = MODE::MOVE;

		mCollisionRect = {};
	}



	return OBJ_NORMAL;
}


void Player::PlayerMove()
{
	if (m_pAStar->GetBesRoadSpace().empty())
	{
		mAttribute.mode = MODE::STAND;
		return;
	}
	if (mAttribute.currentSP > 0)
	{
		mCurScene = (size_t)PLAYER_SCENE::RUN;
		// 1.4
		mAttribute.speed = PLAYER_SPEED * 1.3f;
	}
	else
	{
		mCurScene = (size_t)PLAYER_SCENE::WALK;
		mAttribute.speed = PLAYER_SPEED;
	}

	m_pPreMotion = m_pCurMotion;
	ASTAR_NODE* pAStarNode = m_pAStar->GetBesRoadSpace().top();

	m_pCurMotion = pAStarNode->pMotion;

	if (m_pCurMotion != m_pPreMotion)
	{
		mImageTempKey = StringMgr::stringReplace(m_pImageKey, m_pPreMotion, m_pCurMotion);
		m_pImageKey = mImageTempKey.c_str();
		m_pPreMotion = m_pCurMotion;
	}

	mRadian = MathMgr::CaclRadian(this, TileMgr::GetInstance()->GetTile(pAStarNode->index));
	mInfo.X += cosf(mRadian) * mAttribute.speed;
	mInfo.Y -= sinf(mRadian) * mAttribute.speed;

	float distance = MathMgr::CaclDistance(this, TileMgr::GetInstance()->GetTile(pAStarNode->index));

	if (distance <= 5.f)
	{
		m_pAStar->GetBesRoadSpace().pop();
		m_bStageInit = false;
	}
}



void Player::LateUpdate()
{
	SceneChange();
	StageChange();

	if (m_pAnimationMgr->GetFrameInfo().pScenekey == L"Run")
	{
		mAttribute.currentSP -= 0.5f;
	}

	if (KeyMgr::GetInstance()->KeyDown(VK_F2))
	{
		EffectMgr::GetInstance()->GetFindEffect(L"HpPotion", false)->SetEffect(mInfo.X, mInfo.Y, 200, false);
		mAttribute.currentHP += (mAttribute.maxHP * 0.25f);
		SoundMgr::GetInstance()->PlaySound(L"PotionDrug.wav", PLAYER_SOUND);
		if (mAttribute.currentHP > mAttribute.maxHP)
			mAttribute.currentHP = mAttribute.maxHP;
	}

	if (KeyMgr::GetInstance()->KeyDown(VK_F3))
	{
		EffectMgr::GetInstance()->GetFindEffect(L"MpPotion", false)->SetEffect(mInfo.X, mInfo.Y, 200, false);
		mAttribute.currentMP += (mAttribute.maxMP * 0.25f);
		SoundMgr::GetInstance()->PlaySound(L"PotionDrug.wav", PLAYER_SOUND);
		if (mAttribute.currentMP > mAttribute.maxMP)
			mAttribute.currentMP = mAttribute.maxMP;
	}

	if (KeyMgr::GetInstance()->KeyDown(VK_F4))
	{
		EffectMgr::GetInstance()->GetFindEffect(L"GreenPotion", false)->SetEffect(mInfo.X, mInfo.Y, 200, false);
		mAttribute.currentSP += (mAttribute.maxSP * 0.25f);
		SoundMgr::GetInstance()->PlaySound(L"PotionDrug.wav", PLAYER_SOUND);
		if (mAttribute.currentSP > mAttribute.maxSP)
			mAttribute.currentSP = mAttribute.maxSP;
	}

	if (KeyMgr::GetInstance()->KeyPressing(VK_F6))
	{
		m_pBossSkill->SetEffect(mInfo.X, mInfo.Y, 150, false, true, 1.5f);
		mAttribute.currentMP -= mAttribute.maxMP;

		EffectMgr::GetInstance()->GetFindEffect(L"BoltIcon", false)->SetEffect(0.f, 0.f, 0, false, false);
		EffectMgr::GetInstance()->GetFindEffect(L"MultAttackIcon", false)->SetEffect(0.f, 0.f, 0, false, false);
		EffectMgr::GetInstance()->GetFindEffect(L"NuckBackIcon", false)->SetEffect(70.f, 102.f, 255, true, false);
		if (mAttribute.currentMP < 0)
			mAttribute.currentMP = 0;
	}

	if (KeyMgr::GetInstance()->KeyPressing(VK_F7))
	{
		m_pMultiAttack->SetEffect(mInfo.X, mInfo.Y, 150, false, true, 1.8f);
		mAttribute.currentMP -= (mAttribute.maxMP * 0.5f);

		EffectMgr::GetInstance()->GetFindEffect(L"BoltIcon", false)->SetEffect(0.f, 0.f, 0, false, false);
		EffectMgr::GetInstance()->GetFindEffect(L"NuckBackIcon", false)->SetEffect(0.f, 0.f, 0, false, false);
		EffectMgr::GetInstance()->GetFindEffect(L"MultAttackIcon", false)->SetEffect(70.f, 102.f, 255, true, false);
		if (mAttribute.currentMP < 0)
			mAttribute.currentMP = 0;
	}
}

void Player::Render(HDC hdc)
{
	float scrollX = ScrollMgr::GetInstance()->GetScrollX();
	float scrollY = ScrollMgr::GetInstance()->GetScrollY();

	m_pFinalImageKey = m_pAnimationMgr->GetImageKey(m_pImageKey);
	MyBmp* pPlayerBmp = BmpMgr::GetInstance()->GetFindBmp(m_pFinalImageKey, false);

	if (pPlayerBmp == nullptr)
	{
		MESSAGE_BOX(L"PlayerBmp Find Fail!!");
	}
	HDC playerDC = pPlayerBmp->GetMemDC();
	mInfo.width = (float)pPlayerBmp->GetWidth();
	mInfo.height = (float)pPlayerBmp->GetHeight();
	UpdateRect();

	GdiTransparentBlt(hdc, mRect.left, mRect.top, (int)mInfo.width, (int)mInfo.height, playerDC,
		0, 0,
		(int)mInfo.width, (int)mInfo.height, RGB(255, 255, 255));
	//BitBlt(hdc, mRect.left, mRect.top, (int)mInfo.width, (int)mInfo.height, playerDC, 0, 0, SRCCOPY);
	if (m_bCollisionDraw)
	{
		Rectangle(hdc, mCollisionRect.left, mCollisionRect.top, mCollisionRect.right, mCollisionRect.bottom);
		RECT playerRect = m_pTarget->CollisionRect(mInfo.X, mInfo.Y, PLAYER_STAND_WIDHT, PLAYER_STAND_HEIGHT);
		Rectangle(hdc, playerRect.left, playerRect.top, playerRect.right, playerRect.bottom);
	}
		
	// Debug
	TCHAR szName2[128] = L"";
	swprintf_s(szName2, L"Radian:%f, Degree:%f SP: %f HP : %f MP : %f Combo : %d", mRadian, ToDegree(mRadian), mAttribute.currentSP, mAttribute.currentHP, mAttribute.currentMP, CollisionMgr::GetInstance()->GetComboCount());
	//TextOut(hdc, 50, 130, szName2, lstrlen(szName2));

	// Debug
	TCHAR szName[128] = L"";
	swprintf_s(szName, L"PlayerX: %d PlayerY: %d PlayerLeft: %d PlayerRight: %d Animation: %s Collision : %d", (int)mInfo.X, (int)mInfo.Y, (int)mRect.left, (int)mRect.right, m_pAnimationMgr->GetImageKey(m_pImageKey), (int)mAttribute.bCollision);
	TCHAR szTile[128] = L"";
	swprintf_s(szTile, L"TileIndex: %d TileTargetIndex: %d", (int)mTileIndex, (int)mTargetIndex);
	//TextOut(hdc, 50, 70, szName, lstrlen(szName)); // 지정한 좌표에 폰트 출력!
	//TextOut(hdc, 50, 90, szTile, lstrlen(szTile)); // 지정한 좌표에 폰트 출력!

	m_pAnimationMgr->FrmaeMove();
}

void Player::SceneChange()
{
	m_pSceneKey = m_pAnimationMgr->GetFrameInfo().pScenekey;

	if (mPreScene != mCurScene)
	{
		switch ((PLAYER_SCENE)mCurScene)
		{
		case PLAYER_SCENE::STAND:
			m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)PLAYER_FRAME::STAND, L"Stand", GetTickCount(), (size_t)FRAME_SPEED::STAND));
			break;
		case PLAYER_SCENE::WALK:
			m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)PLAYER_FRAME::WALK, L"Walk", GetTickCount(), (size_t)FRAME_SPEED::WALK));
			break;
		case PLAYER_SCENE::RUN:
			m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)PLAYER_FRAME::RUN, L"Run", GetTickCount(), (size_t)FRAME_SPEED::RUN));
			break;
		case PLAYER_SCENE::HORCUT:
			m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)PLAYER_FRAME::HORCUT, L"HorCut", GetTickCount(), (size_t)FRAME_SPEED::HORCUT));
			break;
		case PLAYER_SCENE::STAB:
			m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)PLAYER_FRAME::STAB, L"Stab", GetTickCount(), (size_t)FRAME_SPEED::STAB));
			break;
		case PLAYER_SCENE::VERCUT:
			m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)PLAYER_FRAME::VERCUT, L"VerCut", GetTickCount(), (size_t)FRAME_SPEED::VERCUT));
			break;
		case PLAYER_SCENE::DAMAGE:
			m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)PLAYER_FRAME::DAMAGE, L"Damage", GetTickCount(), (size_t)FRAME_SPEED::DAMAGE));
			break;
		case PLAYER_SCENE::REST:
			m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)PLAYER_FRAME::REST, L"Rest", GetTickCount(), (size_t)FRAME_SPEED::REST));
			break;
		default:
			break;
		}
		mImageTempKey = StringMgr::stringReplace(m_pImageKey, m_pSceneKey, m_pAnimationMgr->GetFrameInfo().pScenekey, true);
		m_pImageKey = mImageTempKey.c_str();
		mPreScene = mCurScene;
	}
}


void Player::StageChange()
{
	if (TileMgr::GetInstance()->GetTile(mTileIndex)->GetDrawID() == TILE_WARP_ENTRANCE)
	{
		
		if (m_bStageInit == false)
		{
			if (!lstrcmp(m_pStageKey, L"House"))
			{
				StageInit(603.f, 96.f, L"Town2", L"../Data/Town2.dat");	
			}
			else if (!lstrcmp(m_pStageKey, L"Town2"))
			{
				StageInit(222.f, 673.f, L"House", L"../Data/House.dat");
			}
			else if (!lstrcmp(m_pStageKey, L"Field"))
			{
				m_pStageKey = L"Dungeon";
				SceneMgr::GetInstance()->ChangeScene(SCENE_ID::DUNGEON);
	
			}
			else if (!lstrcmp(m_pStageKey, L"Dungeon"))
			{
				m_pStageKey = L"BossRoom";
				SceneMgr::GetInstance()->ChangeScene(SCENE_ID::BOSS);
			}
			m_bStageInit = true;
			SoundMgr::GetInstance()->PlaySound(L"Gate.WAV", PLAYER_SOUND);
			m_pAStar->Release();
		}
	}
	else if (TileMgr::GetInstance()->GetTile(mTileIndex)->GetDrawID() == TILE_WARP_EXIT)
	{
		if (m_bStageInit == false)
		{
			if (!lstrcmp(m_pStageKey, L"Town2"))
			{
				SceneMgr::GetInstance()->ChangeScene(SCENE_ID::FIELD);
				m_pStageKey = L"Field";
			}
			m_bStageInit = true;
			SoundMgr::GetInstance()->PlaySound(L"Gate.wav", PLAYER_SOUND);
			m_pAStar->Release();
		}
	}
}

void Player::StageInit(float X, float Y, const TCHAR* pStageKey, const TCHAR* pFilePath)
{
	m_pStageKey = pStageKey;
	mInfo.X = X;
	mInfo.Y = Y;
	m_pAStar->Release();

	TileMgr::GetInstance()->Release();
	TileMgr::GetInstance()->LoadData(pFilePath);
	TileMgr::GetInstance()->Initialize();
}

void Player::ComboSystem(size_t curScene)
{
	switch (curScene)
	{
	case (size_t)PLAYER_SCENE::HORCUT:
		if (mPreScene == (size_t)PLAYER_SCENE::STAB && m_pAnimationMgr->GetFrameInfo().frameStart >= 2)
			++mComboCount;
		else if (mPreScene == (size_t)PLAYER_SCENE::VERCUT && m_pAnimationMgr->GetFrameInfo().frameStart >= 4)
			++mComboCount;
		else
			mComboCount = 0;
		break;
	case (size_t)PLAYER_SCENE::STAB:
		if (mPreScene == (size_t)PLAYER_SCENE::HORCUT && m_pAnimationMgr->GetFrameInfo().frameStart >= 2)
			++mComboCount;
		else if (mPreScene == (size_t)PLAYER_SCENE::VERCUT && m_pAnimationMgr->GetFrameInfo().frameStart >= 4)
			++mComboCount;
		else
			mComboCount = 0;
		break;
	case (size_t)PLAYER_SCENE::VERCUT:
		if (mPreScene == (size_t)PLAYER_SCENE::STAB && m_pAnimationMgr->GetFrameInfo().frameStart >= 2)
			++mComboCount;
		else if (mPreScene == (size_t)PLAYER_SCENE::HORCUT && m_pAnimationMgr->GetFrameInfo().frameStart >= 2)
			++mComboCount;
		else
			mComboCount = 0;
		break;
	default:
		break;
	}
}

void Player::PlayRotation(float radian)
{
	mDegree = ToDegree(radian);

	m_pPreMotion = m_pCurMotion;

	if (MathMgr::CalPlayRotation(mDegree) != nullptr)
	{
		m_pCurMotion = MathMgr::CalPlayRotation(mDegree);
	}

	if (m_pCurMotion != m_pPreMotion)
	{
		mImageTempKey = StringMgr::stringReplace(m_pImageKey, m_pPreMotion, m_pCurMotion);
		m_pImageKey = mImageTempKey.c_str();
		m_pPreMotion = m_pCurMotion;
	}
}

void Player::HorCut()
{
	mCurScene = (size_t)PLAYER_SCENE::HORCUT;
	mAttribute.mode = MODE::ATTACK;

	mCollisionX = cosf(mCollisionRadian) * 30;
	mCollisionY = sinf(mCollisionRadian) * 30;

	int width = 50;
	int height = 50;
	if ((mRadian >= -4 && mRadian <= -2.5) || mRadian >= -0.5 && mRadian <= 0.3 || mRadian >= 3.0 && mRadian <= 3.2)
	{
		height = (int)(height * 2.5f);
	}
	else
	{
		width = (int)(width * 2.5f);
	}

	mAttribute.attack = 1.2f;

	//ComboSystem(mCurScene);

	//if (m_pAnimationMgr->GetFrameInfo().frameStart >= 2)
		mCollisionRect = CollisionRect(mInfo.X + mCollisionX, mInfo.Y - mCollisionY, width, height);
}

void Player::VerCut()
{
	mCollisionX = cosf(mCollisionRadian) * 80;
	mCollisionY = sinf(mCollisionRadian) * 80;
	mAttribute.attack = 1.6f;

	mCurScene = (size_t)PLAYER_SCENE::VERCUT;
	mAttribute.mode = MODE::ATTACK;

	//if (m_pAnimationMgr->GetFrameInfo().frameStart >= 1)
		mCollisionRect = CollisionRect(mInfo.X + mCollisionX, mInfo.Y - mCollisionY, 50, 50);
}

void Player::Stab()
{
	mCurScene = (size_t)PLAYER_SCENE::STAB;
	mAttribute.mode = MODE::ATTACK;
	mAttribute.attack = 1.4f;

	mCollisionX = cosf(mCollisionRadian) * 80;
	mCollisionY = sinf(mCollisionRadian) * 80;

	mCollisionRect = CollisionRect(mInfo.X + mCollisionX, mInfo.Y - mCollisionY, 50, 50);
}

void Player::CollisionEX()
{
	float moveX = 0.f;
	float moveY = 0.f;

	for (Obj* pEnemy : ObjMgr::GetInstance()->GetObj(OBJ_ENEMY))
	{
		if (EXChecRect(pEnemy, &moveX, &moveY))
		{
			if (pEnemy->GetAttriBute().mode == MODE::DAMAGED)
			{
				if (moveX < moveY)
				{
					if (mInfo.X > pEnemy->GetInfo().X)
					{
						moveX *= -1.f;
					}
					pEnemy->SetPos(pEnemy->GetInfo().X + moveX, pEnemy->GetInfo().Y);
				}
				else
				{
					if (mInfo.Y > pEnemy->GetInfo().Y)
					{
						moveY *= -1.f;
					}
					pEnemy->SetPos(pEnemy->GetInfo().X, pEnemy->GetInfo().Y + moveY);
				}
			}
			
		}
	}
}

bool Player::EXChecRect(Obj * pDest, float * pMoveX, float * pMoveY)
{
	int widthRadius = (pDest->GetCollisionWidht() / 2) + (150);
	int heightRadius = (pDest->GetCollisionHeight() / 2) + (150);

	float distanceX = abs(pDest->GetInfo().X - mInfo.X);
	float distanceY = abs(pDest->GetInfo().Y - mInfo.Y);


	if ((widthRadius >= distanceX) && (heightRadius >= distanceY))
	{
		//파고든거리를 구한다.
		*pMoveX = widthRadius - distanceX;
		*pMoveY = heightRadius - distanceY;
		return true;
	}

	return false;
}

void Player::Release()
{
	SafeDelete<AnimationMgr*>(m_pAnimationMgr);
	SafeDelete<AStar*>(m_pAStar);
}

Obj* Player::CreateBullet()
{
	Obj* pBullet = ObjFactory<Bullet>::CreateObj(mBarrelX, mBarrelY, mAttribute.degree, OBJECT_DIRECTION::DIRECTION_BARREL);
	pBullet->LateInit();
	return pBullet;
}
