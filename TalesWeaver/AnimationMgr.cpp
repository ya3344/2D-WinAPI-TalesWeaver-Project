#include "stdafx.h"
#include "AnimationMgr.h"


AnimationMgr::AnimationMgr()
{
}

AnimationMgr::AnimationMgr(ANIMATION_INFO animationInfo) : mAnimationInfo(animationInfo)
{
}

AnimationMgr::AnimationMgr(const TCHAR * pImageLocation, const TCHAR * pImageKey, size_t motonMaxNum)
{
	if(pImageLocation != nullptr)
		lstrcpy(mAnimationInfo.imageLocation, pImageLocation);
	lstrcpy(mAnimationInfo.imageKey, pImageKey);
	mAnimationInfo.motionMaxNum = motonMaxNum;
}


AnimationMgr::~AnimationMgr()
{
}

bool AnimationMgr::ObjLoadBmp(bool bDynamic)
{
	// Animation Bmp Setting
	TCHAR imageLocation[256] = L"";
	TCHAR imageKey[64] = L"";

	TCHAR imageScene[(size_t)ANIM_TYPE::TYPE_END][(size_t)PLAYER_SCENE::SCENE_END][32] =
	{ {L"Stand", L"Walk",  L"Run", L"HorCut", L"Stab", L"VerCut", L"Damage", L"Rest"},
	  {L"Stand", L"HorCut", L"Stab", L"VerCut"},
	  {L"Stand", L"Walk", L"Attack", L"Death"},
	  { L"Stand", L"Walk", L"Attack", L"Death" },
	  { L"Stand", L"Walk", L"Attack", L"Death" },
	  { L"Stand", L"Walk", L"Attack", L"Death" },
	  { L"Stand", L"Walk", L"Attack", L"Fire", L"Avoid", L"Skill", L"Death" },
	};

	int sceneFrameMax[(size_t)ANIM_TYPE::TYPE_END][(size_t)PLAYER_SCENE::SCENE_END] =
	{ { (size_t)PLAYER_FRAME::STAND , (size_t)PLAYER_FRAME::WALK, (size_t)PLAYER_FRAME::RUN, 
		(size_t)PLAYER_FRAME::HORCUT, (size_t)PLAYER_FRAME::STAB,  (size_t)PLAYER_FRAME::VERCUT, 
		(size_t)PLAYER_FRAME::DAMAGE, (size_t)PLAYER_FRAME::REST },

	  { (size_t)PLAYER_FRAME::STAND , (size_t)PLAYER_FRAME::HORCUT, (size_t)PLAYER_FRAME::STAB, (size_t)PLAYER_FRAME::VERCUT },

	  { (size_t)JELLY_FRAME::STAND , (size_t)JELLY_FRAME::WALK, (size_t)JELLY_FRAME::ATTACK, (size_t)JELLY_FRAME::DEATH },

	  { (size_t)WOLF_FRAME::STAND , (size_t)WOLF_FRAME::WALK, (size_t)WOLF_FRAME::ATTACK, (size_t)WOLF_FRAME::DEATH },

	  { (size_t)DARKCOW_FRAME::STAND , (size_t)DARKCOW_FRAME::WALK, (size_t)DARKCOW_FRAME::ATTACK, (size_t)DARKCOW_FRAME::DEATH },

	  { (size_t)CREEDWIZARD_FRAME::STAND , (size_t)CREEDWIZARD_FRAME::WALK, (size_t)CREEDWIZARD_FRAME::ATTACK, (size_t)CREEDWIZARD_FRAME::DEATH },

	  { (size_t)BOSS_FRAME::STAND , (size_t)BOSS_FRAME::WALK, (size_t)BOSS_FRAME::ATTACK, (size_t)BOSS_FRAME::FIRE, (size_t)BOSS_FRAME::AVOID,
		(size_t)BOSS_FRAME::SKILL, (size_t)BOSS_FRAME::DEATH }
	};

	TCHAR imageMotion[(size_t)ANIM_TYPE::TYPE_END][(size_t)OBJ_MOTION::MOTION_END][16] =
	{ {L"D", L"L",  L"LD", L"LU", L"U", L"RD", L"R", L"RU"}, 
	  {L"D", L"L",  L"LD", L"LU", L"U", L"RD", L"R", L"RU"},
	  {L"LD", L"LU", L"RD", L"RU"},
	  { L"LD", L"LU", L"RD", L"RU" },
	  { L"LD", L"LU", L"RD", L"RU" },
	  { L"LD", L"LU", L"RD", L"RU" },
	  { L"LD", L"LU", L"RD", L"RU" },
	};

	// 좌우반전 이미지 체크 변수
	size_t reverseCheckNum = 0;

	if (mAnimationInfo.motionMaxNum == OBJ_MOTION::MOTION_END)
	{
		reverseCheckNum = mAnimationInfo.motionMaxNum / 2;
	}
	else if (mAnimationInfo.motionMaxNum == OBJ_MOTION::MOTION_END / 2)
	{
		reverseCheckNum = (mAnimationInfo.motionMaxNum / 2)-1;
	}

	for (size_t i = 0; i <= mAnimationInfo.sceneMaxNum; ++i)
	{
		for (size_t j = 0; j < mAnimationInfo.motionMaxNum; ++j)
		{
			for (size_t k = 0; k <= sceneFrameMax[(size_t)mAnimationInfo.objType][i]; ++k)
			{
				if (j <= reverseCheckNum)
				{
					swprintf_s(imageLocation, mAnimationInfo.imageLocation
						, imageScene[(size_t)mAnimationInfo.objType][i], 
						imageScene[(size_t)mAnimationInfo.objType][i], imageMotion[(size_t)mAnimationInfo.objType][j],
						imageScene[(size_t)mAnimationInfo.objType][i], imageMotion[(size_t)mAnimationInfo.objType][j], k);

					swprintf_s(imageKey, mAnimationInfo.imageKey, imageScene[(size_t)mAnimationInfo.objType][i], imageMotion[(size_t)mAnimationInfo.objType][j], k);

					BmpMgr::GetInstance()->AddBmp(imageKey, imageLocation, bDynamic);
				}
				else
				{
					// 좌우 반전 이미지 추가.
					swprintf_s(imageKey, mAnimationInfo.imageKey, imageScene[(size_t)mAnimationInfo.objType][i], imageMotion[(size_t)mAnimationInfo.objType][j], k);

					BmpMgr::GetInstance()->AddBmp(imageKey, imageMotion[(size_t)mAnimationInfo.objType][j], bDynamic, true);
				}
			}
		}
	}

	return true;
}

bool AnimationMgr::EffectLoadBmp(bool bDynamic)
{
	// Animation Bmp Setting
	TCHAR imageLocation[256] = L"";
	TCHAR imageKey[64] = L"";
	TCHAR imageTempKey[64] = L"";
	TCHAR motionKey[64] = L"";

	lstrcat(mAnimationInfo.imageKey, L"%d");

	for (size_t i = 0; i <= mAnimationInfo.motionMaxNum; ++i)
	{
		swprintf_s(imageLocation, mAnimationInfo.imageLocation, i);
		swprintf_s(imageKey, mAnimationInfo.imageKey, i);

		if (lstrlen(mAnimationInfo.imageLocation) > lstrlen(L"RD"))
		{
			BmpMgr::GetInstance()->AddBmp(imageKey, imageLocation, bDynamic);
		}
		else
		{
			BmpMgr::GetInstance()->AddBmp(imageKey, imageLocation, bDynamic, true);
		}

	}
	return true;
}

void AnimationMgr::SetImageKey(const TCHAR * pImageKey)
{
	wprintf_s(mImageKey, "%s%d", pImageKey, mFrameInfo.frameStart);
}

void AnimationMgr::FrmaeMove()
{
	if (mFrameInfo.frameTime + mFrameInfo.frameSpeed < GetTickCount())
	{
		mFrameInfo.frameTime = GetTickCount();
		++mFrameInfo.frameStart;
	}

	if (mFrameInfo.frameStart > mFrameInfo.frameEnd)
		mFrameInfo.frameStart = 0;
}



const TCHAR * AnimationMgr::GetImageKey(const TCHAR* pImageKey)
{
	wsprintf(mImageKey, TEXT("%s%d"), pImageKey, mFrameInfo.frameStart);
	
	return mImageKey;
}
