#include "stdafx.h"
#include "EnemyDarkCow.h"
#include "AnimationMgr.h"
#include "AStar.h"
#include "Effect.h"
#include "Player.h"

EnemyDarkCow::EnemyDarkCow()
{
}


EnemyDarkCow::~EnemyDarkCow()
{
	Release();
}

void EnemyDarkCow::Initialize()
{
	mAttribute.maxHP = DARKCOW_MAXHP;
	mAttribute.currentHP = mAttribute.maxHP;
	mAttribute.type = TYPE_DARKCOW;
	mAttribute.attack = 2.f;
	mAttribute.speed = (float)1.3f;
	mMoveState = ((rand() % OBJ_MOTION::MOTION_END));
	mRandomTime = (rand() % 500) + 700;
	// Animation Init
	if (m_pAnimationMgr == nullptr)
	{
		m_pAnimationMgr = new AnimationMgr();
	}

	m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)DARKCOW_FRAME::STAND, L"Stand", GetTickCount(), FRAME_SPEED::STAND));
	m_pImageKey = L"DarkCow_Stand_LD";
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

	if (m_pDustEffect == nullptr)
	{
		m_pDustEffect = new Effect(L"Dust", nullptr, (size_t)EFFECT_FRAME::DUST, (size_t)FRAME_SPEED::DUST);
		m_pDustEffect->Initialize();
	}

	// Collision Rect, Widht Setting
	mCollisionWidth = DARKCOW_STAND_WIDTH;
	mCollisionHeight = DARKCOW_STAND_HEIGHT;
}

void EnemyDarkCow::LateInit()
{

}

int EnemyDarkCow::Update()
{
	mDistance = MathMgr::CaclDistance(this, m_pTarget);


	if (mAttribute.currentHP < mAttribute.maxHP && mDistance <= 500 && dynamic_cast<Player*>(m_pTarget)->GetStageInit() == false)
	{
		if (mAttribute.mode == MODE::DAMAGED)
		{
			mAttribute.mode = MODE::DAMAGED;
		}
		else if (m_pAStar->GetBesRoadSpace().size() < 2 && mAttribute.bCollision)
		{
			mAttribute.mode = MODE::ATTACK;
		}
		else if (m_pAStar->AStarStart(mTileIndex, m_pTarget->GetTileIndex()) && mAttribute.bCollision == false)
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
		if (m_pAStar->GetBesRoadSpace().size() < 2 && mAttribute.bCollision)
		{
			mAttribute.mode = MODE::ATTACK;
		}
		else
			RandmoMobe();
	}

	switch (mAttribute.mode)
	{
	case MODE::DAMAGED:
		mCollisionRect = CollisionRect(mInfo.X, mInfo.Y, DARKCOW_STAND_WIDTH, DARKCOW_STAND_HEIGHT);

		if (mAttribute.currentHP <= 0)
		{
			mCurScene = (size_t)MONSTER_SCENE::DEATH;
			m_pDustEffect->SetEffect(mInfo.X, mInfo.Y, 250, false);
		}
		else
		{
			m_pHitEffect->SetEffect(mInfo.X, mInfo.Y, 200, false);
			mAttribute.mode = MODE::STAND;
		}
		break;
	case MODE::MOVE:
		mCollisionRect = CollisionRect(mInfo.X, mInfo.Y, DARKCOW_STAND_WIDTH, DARKCOW_STAND_HEIGHT);
		mCurScene = (size_t)MONSTER_SCENE::WALK;
		EnemyMove();
		break;
	case MODE::STAND:
		mCollisionRect = CollisionRect(mInfo.X, mInfo.Y, DARKCOW_STAND_WIDTH, DARKCOW_STAND_HEIGHT);
		m_pAStar->Release();
		mCurScene = (size_t)MONSTER_SCENE::STAND;
		break;
	case MODE::ATTACK:
		mCollisionRect = CollisionRect(mInfo.X, mInfo.Y, DARKCOW_STAND_WIDTH, DARKCOW_STAND_HEIGHT);
		mCurScene = (size_t)MONSTER_SCENE::ATTACK;
		break;
	default:
		mCollisionRect = CollisionRect(mInfo.X, mInfo.Y, DARKCOW_STAND_WIDTH, DARKCOW_STAND_HEIGHT);
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

void EnemyDarkCow::EnemyMove()
{
	if (m_pAStar->GetBesRoadSpace().empty())
	{
		mAttribute.mode = MODE::STAND;
		return;
	}

	if (mAttribute.bCollision && m_pAStar->GetBesRoadSpace().size() < 2)
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



void EnemyDarkCow::SceneChange()
{
	m_pSceneKey = m_pAnimationMgr->GetFrameInfo().pScenekey;

	if (mPreScene != mCurScene)
	{
		switch ((MONSTER_SCENE)mCurScene)
		{
		case MONSTER_SCENE::STAND:
			m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)DARKCOW_FRAME::STAND, L"Stand", GetTickCount(), FRAME_SPEED::STAND));
			break;
		case MONSTER_SCENE::WALK:
			m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)DARKCOW_FRAME::WALK, L"Walk", GetTickCount(), FRAME_SPEED::WALK));
			break;
		case MONSTER_SCENE::ATTACK:
			m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)DARKCOW_FRAME::ATTACK, L"Attack", GetTickCount(), FRAME_SPEED::ATTACK));
			break;
		case MONSTER_SCENE::DEATH:
			m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)DARKCOW_FRAME::DEATH, L"Death", GetTickCount(), FRAME_SPEED::DAMAGE));
			break;
		default:
			break;
		}
		mImageTempKey = StringMgr::stringReplace(m_pImageKey, m_pSceneKey, m_pAnimationMgr->GetFrameInfo().pScenekey, true);
		m_pImageKey = mImageTempKey.c_str();
		mPreScene = mCurScene;
	}
}

void EnemyDarkCow::LateUpdate()
{
	if (mAttribute.currentHP <= 0)
	{
		mAttribute.bDie = true;
	}

	if (mAttribute.bDie == false)
	{
		AttackCollision();
	}

	SceneChange();

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

void EnemyDarkCow::Render(HDC hdc)
{
	float scrollX = ScrollMgr::GetInstance()->GetScrollX();
	float scrollY = ScrollMgr::GetInstance()->GetScrollY();

	m_pFinalImageKey = m_pAnimationMgr->GetImageKey(m_pImageKey);
	MyBmp* pEnemyBmp = BmpMgr::GetInstance()->GetFindBmp(m_pFinalImageKey, true);

	if (pEnemyBmp == nullptr)
	{
		MESSAGE_BOX(L"DarkCow Find Fail!!");
	}
	HDC hEnemyDC = pEnemyBmp->GetMemDC();
	mInfo.width = (float)pEnemyBmp->GetWidth();
	mInfo.height = (float)pEnemyBmp->GetHeight();
	UpdateRect();

	GdiTransparentBlt(hdc, mRect.left, mRect.top, (int)mInfo.width, (int)mInfo.height, hEnemyDC,
		0, 0,
		(int)mInfo.width, (int)mInfo.height, RGB(255, 255, 255));

	//Rectangle(hdc, mCollisionRect.left, mCollisionRect.top, mCollisionRect.right, mCollisionRect.bottom);

	// Multi Effect Render
	m_pHitEffect->Render(hdc);

	m_pAnimationMgr->FrmaeMove();
}

void EnemyDarkCow::EffectCollision()
{


}


void EnemyDarkCow::RandmoMobe()
{
	if (mAttribute.bCollision)
		return;

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

void EnemyDarkCow::AttackCollision()
{
	RECT playerRect = {};
	RECT rect = {};
	playerRect = m_pTarget->CollisionRect(m_pTarget->GetInfo().X, m_pTarget->GetInfo().Y, PLAYER_STAND_WIDHT, PLAYER_STAND_HEIGHT);
	int percent = 0;
	static bool overLapSound = false;

	if (IntersectRect(&rect, &mCollisionRect, &playerRect))
	{
		//EffectMgr::GetInstance()->GetFindEffect(L"Block", false)->SetEffect(m_pTarget->GetInfo().X, m_pTarget->GetInfo().Y,
		//	0, false);
		if (mAttribute.mode == MODE::ATTACK)
		{
			if (m_pAnimationMgr->GetFrameInfo().frameStart >= m_pAnimationMgr->GetFrameInfo().frameEnd - 3)
			{
				if (mAttackTime + (FRAME_SPEED::ATTACK * 4) + 50 < GetTickCount())
				{
					EffectMgr::GetInstance()->GetFindEffect(L"Cray", true)->SetEffect(m_pTarget->GetInfo().X, m_pTarget->GetInfo().Y,
						200, false);
					EffectMgr::GetInstance()->GetFindEffect(L"Crash", true)->SetEffect(m_pTarget->GetInfo().X, m_pTarget->GetInfo().Y,
						200, false);
					mAttackTime = GetTickCount();
					mAttackPercent = (rand() % 10000) + 1;

					overLapSound = false;
				}

				if (overLapSound == false)
				{
					SoundMgr::GetInstance()->PlaySound(L"DarkCowAtt.wav", ENEMY_SOUND);
					overLapSound = true;
				}

				if (mAttackPercent >= 1 && mAttackPercent <= 6000)
				{
					m_pTarget->SetDamage(mAttribute.attack);
					m_pTarget->SetMode(MODE::DAMAGED);
					
					
					
				}
				else
				{
					EffectMgr::GetInstance()->GetFindEffect(L"Block", false)->SetEffect(m_pTarget->GetInfo().X, m_pTarget->GetInfo().Y,
						200, false);
				}
			}
		}

	}
}


void EnemyDarkCow::Release()
{
	SafeDelete<AnimationMgr*>(m_pAnimationMgr);
	SafeDelete<AStar*>(m_pAStar);
	SafeDelete<Image*>(m_pHitEffect);
	SafeDelete<Image*>(m_pDustEffect);
}
