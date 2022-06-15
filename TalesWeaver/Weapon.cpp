#include "stdafx.h"
#include "Weapon.h"
#include "AnimationMgr.h"
#include "Player.h"

Weapon::Weapon()
{
}


Weapon::~Weapon()
{
	Release();
}

void Weapon::Initialize()
{
	if (m_pAnimationMgr == nullptr)
	{
		m_pAnimationMgr = new AnimationMgr(ANIMATION_INFO(ANIM_TYPE::WEAPON, L"../Resource/Weapon/%s/%s_%s/%s_%s%d.bmp",
			L"Weapon_%s_%s%d", 3, OBJ_MOTION::MOTION_END));
		m_pAnimationMgr->ObjLoadBmp(false);
	}
}

void Weapon::LateInit()
{
	mInfo.X = m_pTarget->GetInfo().X;
	mInfo.Y = m_pTarget->GetInfo().Y + 1;
}

int Weapon::Update()
{
	if (mAttribute.bDie)
		return OBJ_DIE;

	mInfo.X = m_pTarget->GetInfo().X;
	mInfo.Y = m_pTarget->GetInfo().Y + 1;

	return OBJ_NORMAL;
}

void Weapon::LateUpdate()
{
	if (KeyMgr::GetInstance()->KeyDown(VK_F1))
	{
		m_bWeapon = !m_bWeapon;
		SoundMgr::GetInstance()->PlaySound(L"SwordEquip.WAV", PLAYER_SOUND);
	}

	if (m_bWeapon)
	{
		PlayerKeyConvert();
		EffectKeyConvert();
	}
		
}


void Weapon::Render(HDC hdc)
{
	if (m_bWeapon == false)
		return;

	if (m_pImageKey == nullptr)
		return;

	float scrollX = ScrollMgr::GetInstance()->GetScrollX();
	float scrollY = ScrollMgr::GetInstance()->GetScrollY();

	MyBmp* pWeaponBmp = BmpMgr::GetInstance()->GetFindBmp(m_pImageKey, false);

	if (pWeaponBmp == nullptr)
	{
		MESSAGE_BOX(L"PlayerBmp Find Fail!!");
	}
	HDC weaponDC = pWeaponBmp->GetMemDC();
	mInfo.width = (float)pWeaponBmp->GetWidth();
	mInfo.height = (float)pWeaponBmp->GetHeight();
	UpdateRect();

	GdiTransparentBlt(hdc, mRect.left, mRect.top, (int)mInfo.width, (int)mInfo.height, weaponDC,
		0, 0,
		(int)mInfo.width, (int)mInfo.height, RGB(255, 255, 255));
}


void Weapon::PlayerKeyConvert()
{
	if (dynamic_cast<Player*>(m_pTarget)->GetImageKey() == nullptr)
		return;
	
	wstring::size_type pos = 0;

	mImageTempKey = dynamic_cast<Player*>(m_pTarget)->GetImageKey();

	pos = mImageTempKey.find(L"Stand");
	if (pos == -1)
	{
		pos = mImageTempKey.find(L"HorCut");
	}
	if (pos == -1)
	{
		pos = mImageTempKey.find(L"Stab");
	}
	if (pos == -1)
	{
		pos = mImageTempKey.find(L"VerCut");
	}

	if (pos != mImageTempKey.npos)
	{
		mImageTempKey = StringMgr::stringReplace(mImageTempKey, L"Player", L"Weapon", true);
		m_pImageKey = mImageTempKey.c_str();
	}
	else
	{
		m_pImageKey = nullptr;
	}
}

void Weapon::EffectKeyConvert()
{
	if (dynamic_cast<Player*>(m_pTarget)->GetEffectKey() == nullptr)
		return;

	wstring effectTempKey = dynamic_cast<Player*>(m_pTarget)->GetEffectKey();

	if (effectTempKey.find(L"HorCut") != effectTempKey.npos)
	{
		effectTempKey = StringMgr::stringReplace(effectTempKey, L"Player_", L"", true);

		EffectMgr::GetInstance()->GetFindEffect(effectTempKey, false)->SetEffect(mInfo.X, mInfo.Y, 150, false);
		mEffectCurrentKey = effectTempKey;
	}
	else if (effectTempKey.find(L"Stab") != effectTempKey.npos)
	{
		effectTempKey = StringMgr::stringReplace(effectTempKey, L"Player_", L"", true);

		if(effectTempKey == L"Stab_D")
			EffectMgr::GetInstance()->GetFindEffect(effectTempKey, false)->SetEffect(mInfo.X, mInfo.Y + 50, 150, false);
		else if(effectTempKey == L"Stab_U")
			EffectMgr::GetInstance()->GetFindEffect(effectTempKey, false)->SetEffect(mInfo.X + 8, mInfo.Y - 50, 150, false);
		else if(effectTempKey == L"Stab_LD")
			EffectMgr::GetInstance()->GetFindEffect(effectTempKey, false)->SetEffect(mInfo.X -25, mInfo.Y + 3, 150, false);
		else if (effectTempKey == L"Stab_RD")
			EffectMgr::GetInstance()->GetFindEffect(effectTempKey, false)->SetEffect(mInfo.X + 25, mInfo.Y + 5, 150, false);
		else if (effectTempKey == L"Stab_R")
			EffectMgr::GetInstance()->GetFindEffect(effectTempKey, false)->SetEffect(mInfo.X + 25, mInfo.Y, 150, false);
		else if (effectTempKey == L"Stab_RU")
			EffectMgr::GetInstance()->GetFindEffect(effectTempKey, false)->SetEffect(mInfo.X + 8, mInfo.Y-12, 150, false);
		else if (effectTempKey == L"Stab_LU")
			EffectMgr::GetInstance()->GetFindEffect(effectTempKey, false)->SetEffect(mInfo.X - 8, mInfo.Y - 12, 150, false);
		else if (effectTempKey == L"Stab_L")
			EffectMgr::GetInstance()->GetFindEffect(effectTempKey, false)->SetEffect(mInfo.X - 23, mInfo.Y -2, 150, false);
		else
			EffectMgr::GetInstance()->GetFindEffect(effectTempKey, false)->SetEffect(mInfo.X, mInfo.Y, 150, false);

		mEffectCurrentKey = effectTempKey;
	}
	else if (effectTempKey.find(L"VerCut") != effectTempKey.npos)
	{
		effectTempKey = StringMgr::stringReplace(effectTempKey, L"Player_", L"", true);

		if (effectTempKey == L"VerCut_RD")
			EffectMgr::GetInstance()->GetFindEffect(effectTempKey, false)->SetEffect(mInfo.X + 35, mInfo.Y - 30, 150, false);
		else if (effectTempKey == L"VerCut_R")
			EffectMgr::GetInstance()->GetFindEffect(effectTempKey, false)->SetEffect(mInfo.X + 35, mInfo.Y - 20, 150, false);
		else if (effectTempKey == L"VerCut_RU")
			EffectMgr::GetInstance()->GetFindEffect(effectTempKey, false)->SetEffect(mInfo.X + 17 , mInfo.Y - 43, 150, false);
		else if (effectTempKey == L"VerCut_U")
			EffectMgr::GetInstance()->GetFindEffect(effectTempKey, false)->SetEffect(mInfo.X + 10, mInfo.Y, 150, false);
		else if (effectTempKey == L"VerCut_L")
			EffectMgr::GetInstance()->GetFindEffect(effectTempKey, false)->SetEffect(mInfo.X - 40, mInfo.Y -20, 150, false);
		else if (effectTempKey == L"VerCut_LU")
			EffectMgr::GetInstance()->GetFindEffect(effectTempKey, false)->SetEffect(mInfo.X - 17, mInfo.Y - 45, 150, false);
		else if (effectTempKey == L"VerCut_LD")
			EffectMgr::GetInstance()->GetFindEffect(effectTempKey, false)->SetEffect(mInfo.X - 36, mInfo.Y - 34, 150, false);
		else if (effectTempKey == L"VerCut_D")
			EffectMgr::GetInstance()->GetFindEffect(effectTempKey, false)->SetEffect(mInfo.X - 5 , mInfo.Y - 34, 150, false);
		else
			EffectMgr::GetInstance()->GetFindEffect(effectTempKey, false)->SetEffect(mInfo.X, mInfo.Y, 150, false);

		mEffectCurrentKey = effectTempKey;
	}
	else
	{
		if (mEffectCurrentKey != L"")
		{
			EffectMgr::GetInstance()->GetFindEffect(mEffectCurrentKey, false)->SetEffect(0, 0, 0, false);
		}
	}

}


void Weapon::Release()
{
	SafeDelete<AnimationMgr*>(m_pAnimationMgr);
}
