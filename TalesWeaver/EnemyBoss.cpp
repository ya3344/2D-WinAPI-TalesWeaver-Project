#include "stdafx.h"
#include "EnemyBoss.h"
#include "AStar.h"
#include "Effect.h"
#include "Player.h"
#include "AnimationMgr.h"
#include "Bullet.h"
#include "PBullet.h"

EnemyBoss::EnemyBoss()
{
}


EnemyBoss::~EnemyBoss()
{
	Release();
}

void EnemyBoss::Initialize()
{
	mAttribute.maxHP = BOSS_MAXHP;
	mAttribute.currentHP = mAttribute.maxHP;
	mAttribute.type = TYPE_BOSS;
	mAttribute.speed = (float)2.0f;
	mAttackRange = 400;
	// Animation Init
	if (m_pAnimationMgr == nullptr)
	{
		m_pAnimationMgr = new AnimationMgr();
	}

	m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)BOSS_FRAME::STAND, L"Stand", GetTickCount(), FRAME_SPEED::STAND));
	m_pImageKey = L"Boss_Stand_LD";
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
		m_pCastingEffect = new Effect(L"BossCasting", nullptr, (size_t)EFFECT_FRAME::CASTING, (size_t)FRAME_SPEED::CASTING);
		m_pCastingEffect->Initialize();
	}


	// Collision Rect, Widht Setting
	mCollisionWidth = BOSS_STAND_WIDTH;
	mCollisionHeight = BOSS_STAND_HEIGHT;


}

void EnemyBoss::LateInit()
{
	m_pFreezeEffect = EffectMgr::GetInstance()->GetFindEffect(L"Freeze", false);
}

int EnemyBoss::Update()
{
	static bool init = false;
	static bool bAvoid = false;
	if (ObjMgr::GetInstance()->GetObj(OBJ_ENEMY).size() > 1)
		return OBJ_NORMAL;

	if (init == false)
	{
		SoundMgr::GetInstance()->StopAll();
		SoundMgr::GetInstance()->PlayBGM(L"BossScene.MP3", BGM);
		SoundMgr::GetInstance()->PlaySound(L"BossWord.wav", BOSS_SOUND);
		init = true;
	}

	mDistance = MathMgr::CaclDistance(this, m_pTarget);

	if (mAttribute.mode == MODE::AVOID)
	{
		mAttribute.mode = MODE::AVOID;
	}
	else if (mAttribute.mode == MODE::DAMAGED)
	{
		mAttribute.mode = MODE::DAMAGED;
	}
	else if (mDistance <= mAttackRange)
	{
		if (m_pAnimationMgr->GetFrameInfo().frameStart < m_pAnimationMgr->GetFrameInfo().frameEnd)
		{
			if (mAttribute.mode == MODE::ATTACK)
				mAttribute.mode = MODE::ATTACK;
			else if (mAttribute.mode == MODE::FIRE)
				mAttribute.mode = MODE::FIRE;
			else if (mAttribute.mode == MODE::SKILL)
				mAttribute.mode = MODE::SKILL;
			else
				AttackMotion();
		}
		else
		{
			AttackMotion();
		}
	}
	else if (m_pAStar->AStarStart(mTileIndex, m_pTarget->GetTileIndex()))
	{
		mAttribute.mode = MODE::MOVE;
	}
	else
	{
		//MessageBox(g_hWnd, L"AStar Fail!", nullptr, MB_OK);
	}
	

	switch (mAttribute.mode)
	{
	case MODE::DAMAGED:
		mCollisionRect = CollisionRect(mInfo.X, mInfo.Y, BOSS_STAND_WIDTH, BOSS_STAND_HEIGHT);

		if (mAttribute.currentHP <= 0)
		{
			mCurScene = (size_t)BOSS_SCENE::DEATH;
			if (m_pAnimationMgr->GetFrameInfo().frameStart >= m_pAnimationMgr->GetFrameInfo().frameEnd - 1)
			{
				mAttribute.bDie = true;
			}
		}
		else
		{
			m_pHitEffect->SetEffect(mInfo.X, mInfo.Y, 200, false);
			mAttribute.mode = MODE::MOVE;
		}
		break;
	case MODE::MOVE:
		mCollisionRect = CollisionRect(mInfo.X, mInfo.Y, BOSS_STAND_WIDTH, BOSS_STAND_HEIGHT);
		if (mCurScene == (size_t)BOSS_SCENE::ATTACK)
		{
			if (m_pAnimationMgr->GetFrameInfo().frameStart < m_pAnimationMgr->GetFrameInfo().frameEnd)
			{
				mAttribute.mode = MODE::ATTACK;
			}
			else
			{
				mCurScene = (size_t)BOSS_SCENE::WALK;
				EnemyMove();
			}
		}
		else if (mCurScene == (size_t)BOSS_SCENE::SKILL)
		{
			if (m_pAnimationMgr->GetFrameInfo().frameStart < m_pAnimationMgr->GetFrameInfo().frameEnd)
			{
				mAttribute.mode = MODE::SKILL;
			}
			else
			{
				mCurScene = (size_t)BOSS_SCENE::WALK;
				EnemyMove();
			}
		}
		else if (mCurScene == (size_t)BOSS_SCENE::FIRE)
		{
			if (m_pAnimationMgr->GetFrameInfo().frameStart < m_pAnimationMgr->GetFrameInfo().frameEnd)
			{
				mAttribute.mode = MODE::FIRE;
			}
			else
			{
				mCurScene = (size_t)BOSS_SCENE::WALK;
				EnemyMove();
			}
		}
		else
		{
			mCurScene = (size_t)BOSS_SCENE::WALK;
			EnemyMove();
		}
		break;
	case MODE::STAND:
		mCollisionRect = CollisionRect(mInfo.X, mInfo.Y, BOSS_STAND_WIDTH, BOSS_STAND_HEIGHT);
		m_pAStar->Release();
		//mCurScene = (size_t)BOSS_SCENE::STAND;
		break;
	case MODE::ATTACK:
		mCollisionRect = CollisionRect(mInfo.X, mInfo.Y, BOSS_STAND_WIDTH, BOSS_STAND_HEIGHT);
		mCurScene = (size_t)BOSS_SCENE::ATTACK;
		mPreMode = mAttribute.mode;
		break;
	case MODE::FIRE:
		mCollisionRect = CollisionRect(mInfo.X, mInfo.Y, BOSS_STAND_WIDTH, BOSS_STAND_HEIGHT);
		mCurScene = (size_t)BOSS_SCENE::FIRE;
		mPreMode = mAttribute.mode;
		break;
	case MODE::SKILL:
		mCollisionRect = CollisionRect(mInfo.X, mInfo.Y, BOSS_STAND_WIDTH, BOSS_STAND_HEIGHT);
		m_pCastingEffect->SetEffect(mInfo.X, mInfo.Y + 20, 150, false);
		m_pFreezeEffect->SetEffect(m_pTarget->GetInfo().X, m_pTarget->GetInfo().Y, 200, false);
		mCurScene = (size_t)BOSS_SCENE::SKILL;
		mPreMode = mAttribute.mode;

		if (m_bOverlapSound == false)
		{
			SoundMgr::GetInstance()->PlaySound(L"IceUp.wav", BOSS_SOUND);
			m_bOverlapSound = true;
		}
		break;
	case MODE::AVOID:
		mCollisionRect = CollisionRect(mInfo.X, mInfo.Y, BOSS_STAND_WIDTH, BOSS_STAND_HEIGHT);
		if (m_pAnimationMgr->GetFrameInfo().frameStart < m_pAnimationMgr->GetFrameInfo().frameEnd)
		{
			mCurScene = (size_t)BOSS_SCENE::AVOID;
		}
		else
		{		
			mAttribute.mode = MODE::FIRE;
			
		}
		break;
	default:
		mCollisionRect = CollisionRect(mInfo.X, mInfo.Y, BOSS_STAND_WIDTH, BOSS_STAND_HEIGHT);
		m_pAStar->Release();
		mCurScene = (size_t)BOSS_SCENE::STAND;
		bAvoid = false;
		break;

	}

	if (mAttribute.bDie)
	{
		if (mAttribute.bCollision)
		{
			mAttribute.bCollision = false;
			m_pTarget->SetCollision(false);
		}

		SoundMgr::GetInstance()->StopAll();
		SoundMgr::GetInstance()->PlaySound(L"Endding.wav", BOSS_SOUND);
		return OBJ_DIE;
	}


	return OBJ_NORMAL;
}

void EnemyBoss::LateUpdate()
{
	SceneChange();

	if (mAttribute.bDie == false)
	{
		Attack();
		Damaged();
		
	}

	/*	if (mAttribute.mode == MODE::ATTACK || mAttribute.mode == MODE::FIRE  || mAttribute.mode == MODE::)
		*/ {
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
		//	}
	}
}


void EnemyBoss::Render(HDC hdc)
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

	m_pAnimationMgr->FrmaeMove();

	//Rectangle(hdc, mCollisionRect.left, mCollisionRect.top, mCollisionRect.right, mCollisionRect.bottom);

	TCHAR test[255];
	swprintf_s(test, L"BossX: %d BossY: %d , Animation: %s HP: %d. MODE: %d", (int)mInfo.X, (int)mInfo.Y, m_pFinalImageKey, (int)mAttribute.currentHP, 
		(int)mAttribute.mode);
	//TextOut(hdc, 50, 150 , test, lstrlen(test)); // 지정한 좌표에 폰트 출력!

	TCHAR Bullet[255];
	swprintf_s(Bullet, L"BulletSize: %d", (int)ObjMgr::GetInstance()->GetObj(OBJ_BULLET).size());
	//TextOut(hdc, 50, 170, Bullet, lstrlen(Bullet)); // 지정한 좌표에 폰트 출력!
}


void EnemyBoss::EnemyMove()
{
	if (m_pAStar->GetBesRoadSpace().empty())
	{
		mAttribute.mode = MODE::STAND;
		return;
	}

	if (mDistance < mAttackRange)
	{
		AttackMotion();
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


void EnemyBoss::SceneChange()
{
	m_pSceneKey = m_pAnimationMgr->GetFrameInfo().pScenekey;

	if (mPreScene != mCurScene)
	{
		switch ((BOSS_SCENE)mCurScene)
		{
		case BOSS_SCENE::STAND:
			m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)BOSS_FRAME::STAND, L"Stand", GetTickCount(), FRAME_SPEED::STAND));
			break;
		case BOSS_SCENE::WALK:
			m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)BOSS_FRAME::WALK, L"Walk", GetTickCount(), FRAME_SPEED::WALK));
			break;
		case BOSS_SCENE::ATTACK:
			m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)BOSS_FRAME::ATTACK, L"Attack", GetTickCount(), FRAME_SPEED::ATTACK));
			break;
		case BOSS_SCENE::AVOID:
			m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)BOSS_FRAME::AVOID, L"Avoid", GetTickCount(), FRAME_SPEED::AVOID));
			break;
		case BOSS_SCENE::SKILL:
			m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)BOSS_FRAME::SKILL, L"Skill", GetTickCount(), FRAME_SPEED::SKILL));
			break;
		case BOSS_SCENE::FIRE:
			m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)BOSS_FRAME::FIRE, L"Fire", GetTickCount(), FRAME_SPEED::FIRE));
			break;
		case BOSS_SCENE::DEATH:
			m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, (size_t)BOSS_FRAME::DEATH, L"Death", GetTickCount(), FRAME_SPEED::DAMAGE));
			break;
		default:
			break;
		}
		mImageTempKey = StringMgr::stringReplace(m_pImageKey, m_pSceneKey, m_pAnimationMgr->GetFrameInfo().pScenekey, true);
		m_pImageKey = mImageTempKey.c_str();
		mPreScene = mCurScene;
	}
}

void EnemyBoss::Attack()
{
	static bool init = false;

	if (mAttribute.mode == MODE::ATTACK)
	{
		if (m_pAnimationMgr->GetFrameInfo().frameStart == 4 || m_pAnimationMgr->GetFrameInfo().frameStart == 7)
		{
			if (init == false)
			{
				SoundMgr::GetInstance()->PlaySound(L"GunShot.WAV", BOSS_SOUND);
				init = true;
			}
			if (mBulletTime + (m_pAnimationMgr->GetFrameInfo().frameSpeed / 3)  < GetTickCount())
			{
				init = false;
				mBulletTime = GetTickCount();
				if (m_pCurMotion == L"LD")
				{
					mBulletStartX = mInfo.X-60.f;
					mBulletStartY = mInfo.Y-6.f;
				}
				else if (m_pCurMotion == L"LU")
				{
					mBulletStartX = mInfo.X - 55.f;
					mBulletStartY = mInfo.Y - 20.f;
				}
				else if (m_pCurMotion == L"RD")
				{
					mBulletStartX = mInfo.X + 80.f;
					mBulletStartY = mInfo.Y + 6.f;
				}
				else if (m_pCurMotion == L"RU")
				{
					mBulletStartX = mInfo.X + 70.f;
					mBulletStartY = mInfo.Y - 25.f;
				}
				m_pBullet = ObjFactory<Bullet>::CreateObj(m_pTarget, INFO(mBulletStartX, mBulletStartY, 0.f, 0.f));
				ObjMgr::GetInstance()->AddObj(m_pBullet, OBJ_BULLET);
			}
			
		}
	}
	else if(mAttribute.mode == MODE::FIRE)
	{
		if (m_pAnimationMgr->GetFrameInfo().frameStart == 16)
		{
			if (init == false)
			{
				SoundMgr::GetInstance()->PlaySound(L"BossAttack.WAV", BOSS_SOUND);
				init = true;
			}

			if (mBulletTime + (m_pAnimationMgr->GetFrameInfo().frameSpeed)  < GetTickCount())
			{
				init = false;
				mBulletTime = GetTickCount();
				if (m_pCurMotion == L"LD")
				{
					mBulletStartX = mInfo.X - 60.f;
					mBulletStartY = mInfo.Y - 6.f;
				}
				else if (m_pCurMotion == L"LU")
				{
					mBulletStartX = mInfo.X - 55.f;
					mBulletStartY = mInfo.Y - 20.f;
				}
				else if (m_pCurMotion == L"RD")
				{
					mBulletStartX = mInfo.X + 80.f;
					mBulletStartY = mInfo.Y + 6.f;
				}
				else if (m_pCurMotion == L"RU")
				{
					mBulletStartX = mInfo.X + 70.f;
					mBulletStartY = mInfo.Y - 25.f;
				}
				m_pBullet = ObjFactory<PBullet>::CreateObj(m_pTarget, INFO(mBulletStartX, mBulletStartY, 0.f, 0.f));
				ObjMgr::GetInstance()->AddObj(m_pBullet, OBJ_BULLET);
			}

		}
	}


}

void EnemyBoss::AttackMotion()
{
	mAttackMotinoPercent = (rand() % 10000) + 1;
	if (mAttribute.currentHP >= (mAttribute.maxHP * 0.7))
	{
		mAttribute.mode = MODE::ATTACK;
	}
	else
	{
		if (mAttackMotinoPercent >= 1 && mAttackMotinoPercent <= 7000)
		{
			mAttribute.mode = MODE::FIRE;
		}
		else
		{
			if (mAttackMotionTime + (m_pAnimationMgr->GetFrameInfo().frameSpeed *m_pAnimationMgr->GetFrameInfo().frameEnd) < GetTickCount())
			{
				mAttackMotionTime = GetTickCount();
				m_bOverlapSound = false;
				mAttribute.mode = MODE::SKILL;
			}
			else
			{
				mAttribute.mode = MODE::FIRE;
			}
		}
	}
}

void EnemyBoss::Damaged()
{
	RECT rect;
	static bool bOverlap = false;

	if (IntersectRect(&rect, &m_pTarget->GetCollisionRect(), &mCollisionRect))
	{
		if (mAttackTime +( m_pAnimationMgr->GetFrameInfo().frameSpeed *m_pAnimationMgr->GetFrameInfo().frameEnd)   < GetTickCount())
		{
			mAttackTime = GetTickCount();
			mAttackPercent = (rand() % 10000) + 1;
			bOverlap = false;
		}

		if (mAttackPercent >= 1 && mAttackPercent <= 6000)
		{
			if (mCurScene == (size_t)BOSS_SCENE::ATTACK)
			{
				if (m_pAnimationMgr->GetFrameInfo().frameStart == 4 || m_pAnimationMgr->GetFrameInfo().frameStart == 7)
					return;
			}
			else if (mCurScene == (size_t)BOSS_SCENE::FIRE)
			{
				if (m_pAnimationMgr->GetFrameInfo().frameStart == 16)
					return;
			}

			if (mCurScene != (size_t)BOSS_SCENE::AVOID)
			{
				SetDamage(m_pTarget->GetAttriBute().attack);
				if (bOverlap == false)
				{
					SoundMgr::GetInstance()->PlaySound(L"Damaged.WAV", CHANNAL::DAMAGED_SOUND);
					bOverlap = true;
				}
			}

		}
		else
		{
			if (mPreMode != MODE::FIRE || mPreMode != MODE::ATTACK || mPreMode != MODE::SKILL)
			{
				mAttribute.mode = MODE::AVOID;
				mPreMode = mAttribute.mode;
			}
		}
	}

}

void EnemyBoss::Release()
{
	SafeDelete<AnimationMgr*>(m_pAnimationMgr);
	SafeDelete<AStar*>(m_pAStar);
	SafeDelete<Image*>(m_pHitEffect);
	SafeDelete<Image*>(m_pCastingEffect);
}
