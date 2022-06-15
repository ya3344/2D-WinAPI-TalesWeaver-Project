#include "stdafx.h"
#include "CollisionMgr.h"
#include "Obj.h"
#include "Player.h"
#include "Effect.h"
#include "AnimationMgr.h"

INIT_SINGLETON_VARIABLE(CollisionMgr);

CollisionMgr::CollisionMgr()
{

}


CollisionMgr::~CollisionMgr()
{
	
}

void CollisionMgr::CollisionRectEX(OBJ_LIST & srcList, OBJ_LIST & destList)
{
	float moveX = 0.f;
	float moveY = 0.f;

	for (auto& pSrc : srcList)
	{
		if (pSrc->GetAttriBute().type != TYPE_JELLY)
			continue;

		if (pSrc->GetAttriBute().bDie == true)
			continue;

		for (auto& pDest : destList)
		{
			// 같은 개체끼리는 검사하지 않는다.
			if (pSrc == pDest)
				continue;

			if (pDest->GetAttriBute().type != TYPE_JELLY)
				continue;

			if (pDest->GetAttriBute().bDie == true)
				continue;

			if (CheckRect(pSrc, pDest, &moveX, &moveY))
			{
				// Player Enemy 충돌체크
	/*			if (pSrc->GetAttriBute().type == OBJ_PLAYER && pDest->GetAttriBute().type == OBJ_ENEMY)
				{
					pDest->SetCollision(true);
				}*/

				// X 축을 밀어버린다
				if (moveX < moveY)
				{
					if (pSrc->GetInfo().X > pDest->GetInfo().X)
					{
						moveX *= -1.f;
					}
					pDest->SetPos(pDest->GetInfo().X + moveX, pDest->GetInfo().Y);
				}
				else
				{
					if (pSrc->GetInfo().Y > pDest->GetInfo().Y)
					{
						moveY *= -1.f;
					}
					pDest->SetPos(pDest->GetInfo().X, pDest->GetInfo().Y + moveY);
				}
			}
		}
	}

}

void CollisionMgr::PlayerEnemyCollision(OBJ_LIST& playList, OBJ_LIST& enemyList)
{
	RECT rect;
	RECT srcRect;
	size_t targetIndex = 0;

	Obj* pPlayer = playList.back();

	if (pPlayer->GetAttriBute().bDie == true)
	{
		return;
	}

	for (auto& pEnemy : enemyList)
	{
		if (pEnemy->GetAttriBute().bDie == true)
			continue;

		srcRect = pPlayer->CollisionRect(pPlayer->GetInfo().X, pPlayer->GetInfo().Y, PLAYER_STAND_WIDHT, PLAYER_STAND_HEIGHT);


		if (IntersectRect(&rect, &srcRect, &pEnemy->GetCollisionRect()))
		{
			pPlayer->SetCollision(true);
			pEnemy->SetCollision(true);
		}
		else
		{
			if (pEnemy->GetAttriBute().bCollision)
			{
				pPlayer->SetCollision(false);
				pEnemy->SetCollision(false);
			}
			//if (pPlayer->GetAttriBute().bCollision)
			//{
			//	pPlayer->SetCollision(false);
			//}
		}
	}
}


void CollisionMgr::EnemyEnemyCollision(OBJ_LIST& srcList, OBJ_LIST& destList)
{
	RECT rect;

	for (auto& pSrc : srcList)
	{
		if (pSrc->GetAttriBute().bDie == true)
			continue;

		for (auto& pDest : destList)
		{
			if (pDest->GetAttriBute().bDie == true)
				continue;

			if (pSrc == pDest)
				continue;


			if (IntersectRect(&rect, &pSrc->GetCollisionRect(), &pDest->GetCollisionRect()))
			{
				pSrc->SetMode(MODE::EX_COLLISOIN);
				pDest->SetMode(MODE::EX_COLLISOIN);
			}
			else
			{
				if (pSrc->GetAttriBute().mode == MODE::EX_COLLISOIN)
				{
					pSrc->SetMode(MODE::STAND);
				}
				if (pDest->GetAttriBute().mode == MODE::EX_COLLISOIN)
				{
					pDest->SetMode(MODE::STAND);
				}
			}
		}
	}
}



void CollisionMgr::CollsionEnemyEffect(OBJ_LIST & playList, unordered_map<wstring, Image*> effectSpace)
{
	Obj* pPlayer = playList.back();
	RECT rect = {};
	RECT srcRect = {};

	if (pPlayer->GetAttriBute().bDie == true)
	{
		return;
	}

	for (EffectMapPair mapPair : effectSpace)
	{
		Image* pEffect = mapPair.second;

		if (pEffect->GetImageType() == IMAGE_TYPE::ATTACK)
		{
			if (pEffect->GetAlpha() == 0)
			{
				continue;
			}
			if (pEffect->GetAttack() > 0.f)
			{
				continue;
			}

			AnimationMgr*  pEffectAnimation = dynamic_cast<Effect*>(pEffect)->GetAnimationMgr();
			srcRect = pPlayer->CollisionRect(pPlayer->GetInfo().X, pPlayer->GetInfo().Y, PLAYER_STAND_WIDHT, PLAYER_STAND_HEIGHT);

			if (IntersectRect(&rect, &srcRect, &pEffect->GetRect()))
			{
				mEnemySkillBlockTime = (int)((pEffectAnimation->GetFrameInfo().frameEnd) * pEffectAnimation->GetFrameInfo().frameSpeed);
				if (mEnemySkillTime + mEnemySkillBlockTime < GetTickCount())
				{
					mEnemySkillTime = GetTickCount();
					mEnemySKillPercent = (rand() % 10000) + 1;
				}

				if ((mEnemySKillPercent >= 1 && mEnemySKillPercent <= 4500) || pEffectAnimation->GetFrameInfo().frameSpeed == (size_t)FRAME_SPEED::FREEZE )
				{
					pPlayer->SetDamage(1.5f);
				}
				else
				{
					if (pPlayer->GetAttriBute().currentHP > 0)
					{
						EffectMgr::GetInstance()->GetFindEffect(L"Block", false)->SetEffect(pPlayer->GetInfo().X, pPlayer->GetInfo().Y,
							200, false);
					}
				}
				
			}
		}
	}
}

void CollisionMgr::CollsionPlayerEffect(OBJ_LIST & enemyList, unordered_map<wstring, Image*> effectSpace)
{
	RECT rect;

	for (EffectMapPair mapPair : effectSpace)
	{
		Image* pEffect = mapPair.second;

		if (pEffect->GetAttack() > 0.f)
		{
			for (Obj* pEnemy : enemyList)
			{
				if (IntersectRect(&rect, &pEnemy->GetCollisionRect(), &pEffect->GetRect()))
				{
					AnimationMgr*  pEffectAnimation = dynamic_cast<Effect*>(pEffect)->GetAnimationMgr();

					if (pEffectAnimation->GetFrameInfo().frameSpeed == (size_t)FRAME_SPEED::BOSSSKILL || 
						pEffectAnimation->GetFrameInfo().frameSpeed == (size_t)FRAME_SPEED::MULTIATTACK)
					{
						mPlayerSkillMaxPercent = 10000;
						if (mDamagedTime + 150 < GetTickCount())
						{
							mDamagedTime = GetTickCount();
							m_bDamagedSound = false;
						}
					}
					else
					{
						mPlayerSkillMaxPercent = 7500;
					}

					if (pEnemy->GetAttriBute().type == TYPE_BOSS)
					{
						mPlayerSkillMaxPercent = 5000;
					}

		

					mPlayerSkillBlockTime = (int)((pEffectAnimation->GetFrameInfo().frameEnd) * pEffectAnimation->GetFrameInfo().frameSpeed);

					if (mPlayerSkillTime + mPlayerSkillBlockTime < GetTickCount())
					{
						mPlayerSkillTime = GetTickCount();
						mPlayerSKillPercent = (rand() % 10000) + 1;
						//m_bDamagedSound = false;
					}

		
					if ((mPlayerSKillPercent >= 1 && mPlayerSKillPercent <= mPlayerSkillMaxPercent))
					{
						pEnemy->SetDamage(pEffect->GetAttack());

						if (pEffectAnimation->GetFrameInfo().frameSpeed == (size_t)FRAME_SPEED::BOSSSKILL ||
							pEffectAnimation->GetFrameInfo().frameSpeed == (size_t)FRAME_SPEED::MULTIATTACK)
						{
							if (m_bDamagedSound == false)
							{
								SoundMgr::GetInstance()->PlaySound(L"Damaged.WAV", DAMAGED_SOUND);
								m_bDamagedSound = true;
							}
						}
					}
					else
					{
						if (pEnemy->GetAttriBute().currentHP > 0)
						{
							EffectMgr::GetInstance()->GetFindEffect(L"Block", false)->SetEffect(pEnemy->GetInfo().X, pEnemy->GetInfo().Y,
								200, false);
						}
					}

				}
			}
		}
	}
}

void CollisionMgr::WeaponEnemyCollsion(OBJ_LIST& playList, OBJ_LIST& enemyList)
{
	Obj* pPlayer = playList.back();
	RECT rect;
	static int count = 0;
	if (pPlayer->GetAttriBute().bDie == true)
	{
		return;
	}

	for (Obj* pEnemy : enemyList)
	{
		if (pEnemy->GetAttriBute().bDie == true)
			continue;

		if (pEnemy->GetAttriBute().type == TYPE_BOSS)
			continue;

		
		if (IntersectRect(&rect, &pPlayer->GetCollisionRect(), &pEnemy->GetCollisionRect()))
		{
			if (mAttackTime + ((size_t)FRAME_SPEED::HORCUT) < GetTickCount())
			{
				mAttackTime = GetTickCount();
				mAttackPercent = (rand() % 10000) + 1;

			}

			if (mDamagedTime + 500 < GetTickCount())
			{
				mDamagedTime = GetTickCount();

				m_bDamagedSound = false;
			}

			if (pEnemy->GetAttriBute().type == TYPE_JELLY)
			{
				pEnemy->SetDamage(pPlayer->GetAttriBute().attack);
				if (m_bDamagedSound == false)
				{
					SoundMgr::GetInstance()->PlaySound(L"Damaged.WAV", DAMAGED_SOUND);
					m_bDamagedSound = true;
				}

				m_bAttackSucess = true;
			}
			else
			{
				if (mAttackPercent >= 1 && mAttackPercent <= 3500)
				{
					pEnemy->SetDamage(pPlayer->GetAttriBute().attack);
					if (m_bDamagedSound == false)
					{
						SoundMgr::GetInstance()->PlaySound(L"Damaged.WAV", DAMAGED_SOUND);
						m_bDamagedSound = true;
					}
					m_bAttackSucess = true;
				}
				else
				{
					if (pEnemy->GetAttriBute().currentHP > 0)
					{
						EffectMgr::GetInstance()->GetFindEffect(L"Block", false)->SetEffect(pEnemy->GetInfo().X, pEnemy->GetInfo().Y,
							200, false);
					}
					m_bAttackSucess = false;
				}
			}

		}
		else
			count = 0;
	}
}


bool CollisionMgr::CheckRect(Obj * pSrc, Obj * pDest, float * pMoveX, float * pMoveY)
{
	int widthRadius = (pDest->GetCollisionWidht() / 2) + (pSrc->GetCollisionWidht() / 2);
	int heightRadius = (pDest->GetCollisionHeight() / 2) + (pSrc->GetCollisionHeight() / 2);

	float distanceX = abs(pDest->GetInfo().X - pSrc->GetInfo().X);
	float distanceY = abs(pDest->GetInfo().Y - pSrc->GetInfo().Y);


	if ((widthRadius >= distanceX) && (heightRadius >= distanceY))
	{
		//파고든거리를 구한다.
		*pMoveX = widthRadius - distanceX;
		*pMoveY = heightRadius - distanceY;
		return true;
	}

	return false;
}

bool CollisionMgr::CheckCircle(Obj * pSrc, Obj * pDest)
{
	float X = 0, Y = 0, radius = 0;
	float distance = 0.f;

	radius = (pSrc->GetInfo().width / 2) + (pDest->GetInfo().width / 2);

	distance = MathMgr::CaclDistance(pSrc, pDest);

	if (radius > distance)
	{
		return true;
	}

	return false;
}



void CollisionMgr::CollisionCircle(OBJ_LIST& srcList, OBJ_LIST& destList)
{
	for (auto& pSrc : srcList)
	{
		if (pSrc->GetAttriBute().bDie == true)
			continue;

		for (auto& pDest : destList)
		{
			if (pDest->GetAttriBute().bDie == true)
				continue;

			if (CheckCircle(pSrc, pDest))
			{
				/*	pDest->SetDemage(pSrc->GetAttriBute().attack);
				pSrc->SetDie(true);*/
				pSrc->SetCollision(true);
			}
			else
			{
				/*	pDest->SetDemage(pSrc->GetAttriBute().attack);
				pSrc->SetDie(true);*/
				pSrc->SetCollision(false);
			}
		}
	}
}