#include "stdafx.h"
#include "EnemyJelly.h"
#include "AnimationMgr.h"
#include "AStar.h"
#include "Effect.h"
#include "Player.h"

EnemyJelly::EnemyJelly()
{

}


EnemyJelly::~EnemyJelly()
{
	Release();
}

void EnemyJelly::Initialize()
{
	mAttribute.maxHP = JELLY_MAXHP;
	mAttribute.currentHP = mAttribute.maxHP;
	mAttribute.type = TYPE_JELLY;
	mAttribute.speed = (float)2.0f;
	mAttribute.attack = 1.0f;
	mMoveState = ((rand()%OBJ_MOTION::MOTION_END));
	mRandomTime = (rand() % 500) + 700;
	// Animation Init
	if (m_pAnimationMgr == nullptr)
	{
		m_pAnimationMgr = new AnimationMgr();
	}

	m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)JELLY_FRAME::STAND, L"Stand", GetTickCount(), FRAME_SPEED::STAND));
	m_pImageKey = L"Jelly_Stand_LD";
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

	if (m_pPopEffect == nullptr)
	{
		m_pPopEffect = new Effect(L"Pop", nullptr, (size_t)EFFECT_FRAME::POP, (size_t)FRAME_SPEED::POP);
		m_pPopEffect->Initialize();
	}
}

void EnemyJelly::LateInit()
{

}

int EnemyJelly::Update()
{
	mDistance = MathMgr::CaclDistance(this, m_pTarget);

	if (mDistance <= 300 && dynamic_cast<Player*>(m_pTarget)->GetStageInit() == false)
	{
		if (mAttribute.mode == MODE::DAMAGED)
		{
			mAttribute.mode = MODE::DAMAGED;
		}
		else if (m_pAStar->GetBesRoadSpace().size() < 2 && mAttribute.bCollision)
		{
			mAttribute.mode = MODE::STAND;
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
				mRandomTileIndex = (mTileIndex + gTileNum.X)-1;
				break;
			case OBJ_MOTION::LEFT:
				mRandomTileIndex = (mTileIndex - 1);
				break;
			case OBJ_MOTION::LU:
				mRandomTileIndex = (mTileIndex - gTileNum.X) -1;
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

	if (mAttribute.currentHP <= 0)
	{
		mAttribute.mode = MODE::ATTACK;
	}
	

	switch (mAttribute.mode)
	{
	case MODE::DAMAGED:
		mCurScene = (size_t)MONSTER_SCENE::DEATH;
		if (m_pAnimationMgr->GetFrameInfo().frameStart >= m_pAnimationMgr->GetFrameInfo().frameEnd)
		{
			
			mAttribute.mode = MODE::STAND;
		}

		if(mAttribute.currentHP > 0)
			m_pHitEffect->SetEffect(mInfo.X, mInfo.Y, 200, false);
		break;
	case MODE::MOVE:
		mCurScene = (size_t)MONSTER_SCENE::WALK;
		EnemyMove();
		break;
	case MODE::STAND:
		m_pAStar->Release();
		mCurScene = (size_t)MONSTER_SCENE::STAND;
		break;
	case MODE::ATTACK:
		mCurScene = (size_t)MONSTER_SCENE::ATTACK;
		if (mAttribute.currentHP <= 0)
		{
			if (m_pAnimationMgr->GetFrameInfo().frameStart >= m_pAnimationMgr->GetFrameInfo().frameEnd - 1)
			{

				m_pPopEffect->SetEffect(mInfo.X, mInfo.Y, 200, false);

			}
		}
		break;
	default:
		m_pAStar->Release();
		mCurScene = (size_t)MONSTER_SCENE::STAND;
		break;

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


void EnemyJelly::EnemyMove()
{
	if (m_pAStar->GetBesRoadSpace().empty())
	{
		mAttribute.mode = MODE::STAND;
		return;
	}

	if (mAttribute.bCollision && m_pAStar->GetBesRoadSpace().size() < 2)
	{
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


void EnemyJelly::LateUpdate()
{
	if (dynamic_cast<Effect*>(m_pPopEffect)->GetAnimationMgr()->GetFrameInfo().frameStart >= dynamic_cast<Effect*>(m_pPopEffect)->GetAnimationMgr()->GetFrameInfo().frameEnd - 1)
	{
		mAttribute.bDie = true;
	}

	if (mAttribute.mode == MODE::STAND || mAttribute.mode == MODE::ATTACK)
	{
		if (mAttribute.bCollision)
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


	SceneChange();
	EffectCollision();
}

void EnemyJelly::Render(HDC hdc)
{
	float scrollX = ScrollMgr::GetInstance()->GetScrollX();
	float scrollY = ScrollMgr::GetInstance()->GetScrollY();

	m_pFinalImageKey = m_pAnimationMgr->GetImageKey(m_pImageKey);
	MyBmp* pJellyBmp = BmpMgr::GetInstance()->GetFindBmp(m_pFinalImageKey, true);

	if (pJellyBmp == nullptr)
	{
		MESSAGE_BOX(L"JellyBmp Find Fail!!");
	}
	HDC hJellyDC = pJellyBmp->GetMemDC();
	mInfo.width = (float)pJellyBmp->GetWidth();
	mInfo.height = (float)pJellyBmp->GetHeight();
	mCollisionWidth = pJellyBmp->GetWidth();
	mCollisionHeight = pJellyBmp->GetHeight();
	UpdateRect();
	mCollisionRect = CollisionRect(mInfo.X, mInfo.Y, (int)mInfo.width, (int)mInfo.height);

	GdiTransparentBlt(hdc, mRect.left, mRect.top, (int)mInfo.width, (int)mInfo.height, hJellyDC,
		0, 0,
		(int)mInfo.width, (int)mInfo.height, RGB(255, 255, 255));

	// Multi Effect Render
	m_pHitEffect->Render(hdc);
	m_pPopEffect->Render(hdc);

	m_pAnimationMgr->FrmaeMove();
}


void EnemyJelly::SceneChange()
{
	m_pSceneKey = m_pAnimationMgr->GetFrameInfo().pScenekey;

	if (mPreScene != mCurScene)
	{
		switch ((MONSTER_SCENE)mCurScene)
		{
		case MONSTER_SCENE::STAND:
			m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)JELLY_FRAME::STAND, L"Stand", GetTickCount(), FRAME_SPEED::STAND));
			break;
		case MONSTER_SCENE::WALK:
			m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)JELLY_FRAME::WALK, L"Walk", GetTickCount(), FRAME_SPEED::WALK));
			break;
		case MONSTER_SCENE::ATTACK:
			m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)JELLY_FRAME::ATTACK, L"Attack", GetTickCount(), FRAME_SPEED::ATTACK));
			break;
		case MONSTER_SCENE::DEATH:
			m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)JELLY_FRAME::DEATH, L"Death", GetTickCount(), FRAME_SPEED::DAMAGE));
			break;
		default:
			break;
		}
		mImageTempKey = StringMgr::stringReplace(m_pImageKey, m_pSceneKey, m_pAnimationMgr->GetFrameInfo().pScenekey, true);
		m_pImageKey = mImageTempKey.c_str();
		mPreScene = mCurScene;
	}
}

void EnemyJelly::EffectCollision()
{
	RECT rect;
	if (m_pTarget == nullptr || m_pTarget->GetAttriBute().bDie == true)
		return;
	RECT playerRect = CollisionRect(m_pTarget->GetInfo().X, m_pTarget->GetInfo().Y, PLAYER_STAND_WIDHT, PLAYER_STAND_HEIGHT);

	if (IntersectRect(&rect, &playerRect, &m_pPopEffect->GetRect()))
	{
		m_pTarget->SetDamage(mAttribute.attack);
		//m_pTarget->SetMode(MODE::DAMAGED);
	}

}

void EnemyJelly::Release()
{
	SafeDelete<AnimationMgr*>(m_pAnimationMgr);
	SafeDelete<AStar*>(m_pAStar);
	SafeDelete<Image*>(m_pHitEffect);
	SafeDelete<Image*>(m_pPopEffect);
}
