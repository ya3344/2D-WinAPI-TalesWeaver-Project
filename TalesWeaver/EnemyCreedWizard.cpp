#include "stdafx.h"
#include "EnemyCreedWizard.h"
#include "AnimationMgr.h"
#include "AStar.h"
#include "Effect.h"
#include "Player.h"

EnemyCreedWizard::EnemyCreedWizard()
{

}


EnemyCreedWizard::~EnemyCreedWizard()
{
	Release();
}

void EnemyCreedWizard::Initialize()
{
	mAttribute.maxHP = CREEDWIZARD_MAXHP;
	mAttribute.currentHP = mAttribute.maxHP;
	mAttribute.type = TYPE_CREEDWIZARD;
	mAttribute.speed = (float)1.0f;


	// Animation Init
	if (m_pAnimationMgr == nullptr)
	{
		m_pAnimationMgr = new AnimationMgr();
	}

	m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)CREEDWIZARD_FRAME::STAND, L"Stand", GetTickCount(), FRAME_SPEED::STAND));
	m_pImageKey = L"CreedWizard_Stand_LD";
	m_pCurMotion = L"LD";

	// AStar Init
	if (m_pAStar == nullptr)
	{
		m_pAStar = new AStar();
		m_pAStar->SetObjID(OBJ_ENEMY);
	}

	// Effect Init
	if (m_pHitEffect == nullptr)
	{
		m_pHitEffect = new Effect(L"MonsterHit", nullptr, (size_t)EFFECT_FRAME::MONSTER_HIT, (size_t)FRAME_SPEED::MONSTER_HIT);
		m_pHitEffect->Initialize();
	}

	if (m_pCastingEffect == nullptr)
	{
		m_pCastingEffect = new Effect(L"Casting", nullptr, (size_t)EFFECT_FRAME::CASTING, (size_t)FRAME_SPEED::CASTING);
		m_pCastingEffect->Initialize();
	}

	// Collision Rect, Widht Setting
	mCollisionWidth = CREEDWIZARD_STAND_WIDTH;
	mCollisionHeight = CREEDWIZARD_STAND_HEIGHT;
}

void EnemyCreedWizard::LateInit()
{

}

int EnemyCreedWizard::Update()
{
	mDistance = MathMgr::CaclDistance(this, m_pTarget);

	if (mAttribute.currentHP < mAttribute.maxHP)
	{
		mAttackRange = 500;
	}
	else
	{
		mAttackRange = 400;
	}

	if (mDistance <= mAttackRange && dynamic_cast<Player*>(m_pTarget)->GetStageInit() == false)
	{
		if (mAttribute.mode == MODE::DAMAGED)
		{
			mAttribute.mode = MODE::DAMAGED;
		}
		else if (mDistance <= mAttackRange / 2)
		{
			mAttribute.mode = MODE::ATTACK;
		}
		else if (m_pAStar->AStarStart(mTileIndex, m_pTarget->GetTileIndex()))
		{
			mAttribute.mode = MODE::MOVE;
		}
		else
		{
			//MessageBox(g_hWnd, L"AStar Fail!", nullptr, MB_OK);
		}
	}
	else
	{
		RandomMove();
	}

	switch (mAttribute.mode)
	{
	case MODE::DAMAGED:
		mCollisionRect = CollisionRect(mInfo.X, mInfo.Y, CREEDWIZARD_STAND_WIDTH, CREEDWIZARD_STAND_HEIGHT);

		if (mAttribute.currentHP <= 0)
		{
			mCurScene = (size_t)MONSTER_SCENE::DEATH;
			if (m_pAnimationMgr->GetFrameInfo().frameStart >= m_pAnimationMgr->GetFrameInfo().frameEnd - 1)
			{
				mAttribute.bDie = true;
			}
		}
		else
		{
			m_pHitEffect->SetEffect(mInfo.X, mInfo.Y, 200, false);
			mAttribute.mode = MODE::STAND;
		}
		break;
	case MODE::MOVE:
		if (mCurScene == (size_t)MONSTER_SCENE::ATTACK)
		{
			if (m_pAnimationMgr->GetFrameInfo().frameStart < m_pAnimationMgr->GetFrameInfo().frameEnd)
			{
				mAttribute.mode = MODE::ATTACK;
			}
			else
			{
				mCollisionRect = CollisionRect(mInfo.X, mInfo.Y, CREEDWIZARD_STAND_WIDTH, CREEDWIZARD_STAND_HEIGHT);
				mCurScene = (size_t)MONSTER_SCENE::WALK;
				EnemyMove();
			}
		}
		else
		{
			mCollisionRect = CollisionRect(mInfo.X, mInfo.Y, CREEDWIZARD_STAND_WIDTH, CREEDWIZARD_STAND_HEIGHT);
			mCurScene = (size_t)MONSTER_SCENE::WALK;
			EnemyMove();
		}

		
		break;
	case MODE::STAND:
		mCollisionRect = CollisionRect(mInfo.X, mInfo.Y, CREEDWIZARD_STAND_WIDTH, CREEDWIZARD_STAND_HEIGHT);
		m_pAStar->Release();
		mCurScene = (size_t)MONSTER_SCENE::STAND;
		break;
	case MODE::ATTACK:
		m_pCastingEffect->SetEffect(mInfo.X, mInfo.Y, 100, false);
		mCollisionRect = CollisionRect(mInfo.X, mInfo.Y, CREEDWIZARD_STAND_WIDTH, CREEDWIZARD_STAND_HEIGHT);
		mCurScene = (size_t)MONSTER_SCENE::ATTACK;
		break;
	default:
		mCollisionRect = CollisionRect(mInfo.X, mInfo.Y, CREEDWIZARD_STAND_WIDTH, CREEDWIZARD_STAND_HEIGHT);
		m_pAStar->Release();
		mCurScene = (size_t)MONSTER_SCENE::STAND;
		break;

	}

	if (mAttribute.currentHP < mAttribute.maxHP)
	{
		mAttribute.speed = 2.0f;
	}

	if (mAttribute.bDie)
	{
		if (mAttribute.bCollision)
		{
			mAttribute.bCollision = false;
			m_pTarget->SetCollision(false);
		}
		return OBJ_DIE;
	}
		

	return OBJ_NORMAL;
}


void EnemyCreedWizard::EnemyMove()
{
	if (m_pAStar->GetBesRoadSpace().empty())
	{
		mAttribute.mode = MODE::STAND;
		return;
	}

	if (mDistance < mAttackRange / 2)
	{
		mAttribute.mode = MODE::ATTACK;
		m_pAStar->Release();
		return;
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
	}
}



void EnemyCreedWizard::SceneChange()
{
	m_pSceneKey = m_pAnimationMgr->GetFrameInfo().pScenekey;

	if (mPreScene != mCurScene)
	{
		switch ((MONSTER_SCENE)mCurScene)
		{
		case MONSTER_SCENE::STAND:
			m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)CREEDWIZARD_FRAME::STAND, L"Stand", GetTickCount(), FRAME_SPEED::STAND));
			break;
		case MONSTER_SCENE::WALK:
			m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)CREEDWIZARD_FRAME::WALK, L"Walk", GetTickCount(), FRAME_SPEED::WALK));
			break;
		case MONSTER_SCENE::ATTACK:
			m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)CREEDWIZARD_FRAME::ATTACK, L"Attack", GetTickCount(), FRAME_SPEED::ATTACK));
			break;
		case MONSTER_SCENE::DEATH:
			m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)CREEDWIZARD_FRAME::DEATH, L"Death", GetTickCount(), FRAME_SPEED::DAMAGE));
			break;
		default:
			break;
		}
		mImageTempKey = StringMgr::stringReplace(m_pImageKey, m_pSceneKey, m_pAnimationMgr->GetFrameInfo().pScenekey, true);
		m_pImageKey = mImageTempKey.c_str();
		mPreScene = mCurScene;
	}
}

void EnemyCreedWizard::LateUpdate()
{
	SceneChange();

	if(mAttribute.bDie == false)
		Skill();

	if (mAttribute.mode == MODE::ATTACK)
	{
		mRadian = MathMgr::CaclRadian(this, TileMgr::GetInstance()->GetTile(m_pTarget->GetTileIndex()));

		m_pPreMotion = m_pCurMotion;

		if (MathMgr::CalMonsterRotation(ToDegree(mRadian)) != nullptr)
		{
			m_pCurMotion = MathMgr::CalMonsterRotation(ToDegree(mRadian));
		}

		if (m_pCurMotion != m_pPreMotion)
		{
			mImageTempKey = StringMgr::stringReplace(m_pImageKey, m_pPreMotion, m_pCurMotion);
			m_pImageKey = mImageTempKey.c_str();
			m_pPreMotion = m_pCurMotion;
		}
	}
}

void EnemyCreedWizard::Render(HDC hdc)
{
	float scrollX = ScrollMgr::GetInstance()->GetScrollX();
	float scrollY = ScrollMgr::GetInstance()->GetScrollY();

	m_pFinalImageKey = m_pAnimationMgr->GetImageKey(m_pImageKey);
	MyBmp* pEnemyBmp = BmpMgr::GetInstance()->GetFindBmp(m_pFinalImageKey, true);

	if (pEnemyBmp == nullptr)
	{
		MESSAGE_BOX(L"CrredWizard Find Fail!!");
	}
	HDC hEnemyDC = pEnemyBmp->GetMemDC();
	mInfo.width = (float)pEnemyBmp->GetWidth();
	mInfo.height = (float)pEnemyBmp->GetHeight();
	UpdateRect();

	GdiTransparentBlt(hdc, mRect.left, mRect.top, (int)mInfo.width, (int)mInfo.height, hEnemyDC,
		0, 0,
		(int)mInfo.width, (int)mInfo.height, RGB(255, 255, 255));

	// Multi Effect Render
	m_pHitEffect->Render(hdc);
	m_pCastingEffect->Render(hdc);

	//Rectangle(hdc, mCollisionRect.left, mCollisionRect.top, mCollisionRect.right, mCollisionRect.bottom);

	m_pAnimationMgr->FrmaeMove();
}

void EnemyCreedWizard::RandomMove()
{
	if (mDistance < mAttackRange / 2)
	{
		mAttribute.mode = MODE::ATTACK;
		m_pAStar->Release();
		return;
	}

	if (mMoveTime + mRandomTime < GetTickCount())
	{
		mRandomTime = (rand() % 500) + 700;
		mMoveTime = GetTickCount();
		switch (mMoveState)
		{
		case OBJ_MOTION::DOWN:
			mRandomTileIndex = mTileIndex + gTileNum.X;
			break;
		case OBJ_MOTION::LD:
			mRandomTileIndex = (mTileIndex + gTileNum.X) - 1;
			break;
		case OBJ_MOTION::LEFT:
			mRandomTileIndex = (mTileIndex - 1);
			break;
		case OBJ_MOTION::LU:
			mRandomTileIndex = (mTileIndex - gTileNum.X) - 1;
			break;
		case OBJ_MOTION::RD:
			mRandomTileIndex = (mTileIndex + gTileNum.X) + 1;
			break;
		case OBJ_MOTION::RIGHT:
			mRandomTileIndex = (mTileIndex + 1);
			break;
		case OBJ_MOTION::RU:
			mRandomTileIndex = (mTileIndex - gTileNum.X) + 1;
			break;
		case OBJ_MOTION::UP:
			mRandomTileIndex = (mTileIndex - gTileNum.X);
			break;
		}

		if (m_pAStar->AStarStart(mTileIndex, mRandomTileIndex))
		{
			mAttribute.mode = MODE::MOVE;
			//MessageBox(g_hWnd, L"AStar Sucess!", nullptr, MB_OK);

			mMoveState = ((rand() % OBJ_MOTION::MOTION_END));
		}
		else
		{
			if (mMoveState < (OBJ_MOTION::MOTION_END / 2))
			{
				mMoveState += (OBJ_MOTION::MOTION_END / 2);
			}
			else
			{
				mMoveState -= (OBJ_MOTION::MOTION_END / 2);
			}
			//MessageBox(g_hWnd, L"AStar Fail!", nullptr, MB_OK);
		}
	}
}


void EnemyCreedWizard::Skill()
{

	static bool bOverlap = false;

	if (mAttribute.mode == MODE::ATTACK)
	{
		if (m_pAnimationMgr->GetFrameInfo().frameStart >= m_pAnimationMgr->GetFrameInfo().frameEnd - 2)
		{
			if (mAttackTime + (FRAME_SPEED::ATTACK * 3) + 50 < GetTickCount())
			{
				mAttackTime = GetTickCount();
				mAttackPercent = (rand() % 10000) + 1;
				bOverlap = false;
			}

			if (mAttackPercent >= 1 && mAttackPercent <= 3000)
			{
				EffectMgr::GetInstance()->GetFindEffect(L"Hand", false)->SetEffect(m_pTarget->GetInfo().X, m_pTarget->GetInfo().Y - 20,
					200, false);

				if (bOverlap == false)
				{
					SoundMgr::GetInstance()->PlaySound(L"Hand.wav", EFFECT_SOUND);
					bOverlap = true;
				}
			}
			else if (mAttackPercent >= 3001 && mAttackPercent <= 6000)
			{
				EffectMgr::GetInstance()->GetFindEffect(L"WizardSkill", false)->SetEffect(m_pTarget->GetInfo().X, m_pTarget->GetInfo().Y,
					150, false);

				if (bOverlap == false)
				{
					SoundMgr::GetInstance()->PlaySound(L"WizardSkill.wav", EFFECT_SOUND);
					bOverlap = true;
				}
			}
			else
			{
				EffectMgr::GetInstance()->GetFindEffect(L"BotFire", false)->SetEffect(m_pTarget->GetInfo().X, m_pTarget->GetInfo().Y + 20,
					200, false);

				if (bOverlap == false)
				{
					SoundMgr::GetInstance()->PlaySound(L"Fire.wav", EFFECT_SOUND);
					bOverlap = true;
				}
			}
		}
	}

	
}

void EnemyCreedWizard::Release()
{
	SafeDelete<AnimationMgr*>(m_pAnimationMgr);
	SafeDelete<AStar*>(m_pAStar);
	SafeDelete<Image*>(m_pHitEffect);
	SafeDelete<Image*>(m_pCastingEffect);
}
