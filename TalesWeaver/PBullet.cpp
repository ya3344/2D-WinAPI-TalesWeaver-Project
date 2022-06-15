#include "stdafx.h"
#include "PBullet.h"
#include "AnimationMgr.h"
#include "Effect.h"

PBullet::PBullet()
{
}


PBullet::~PBullet()
{
	Release();
}

void PBullet::Initialize()
{
	mAttribute.speed = 13.f;
	mAttribute.attack = 2.f;

	if (m_pBulletAnimation == nullptr)
	{
		m_pBulletAnimation = new AnimationMgr();
		m_pBulletAnimation->SetFrameInfo(FRAME_INFO(0, (size_t)EFFECT_FRAME::PBULLET, nullptr, GetTickCount(), (size_t)FRAME_SPEED::PBULLET));
	}

	m_pBombEffect = EffectMgr::GetInstance()->GetFindEffect(L"ElecShock", false);

}

void PBullet::LateInit()
{


}

int PBullet::Update()
{
	if (m_bBulletDie == false)
	{
		mRadian = MathMgr::CaclRadian(this, m_pTarget);
		mInfo.X += cosf(mRadian) * mAttribute.speed;
		mInfo.Y -= sinf(mRadian) * mAttribute.speed;
	}

	if(mAttribute.bDie)
		return OBJ_DIE;

	return OBJ_NORMAL;
}

void PBullet::LateUpdate()
{
	BulletCollision();
	
	if (dynamic_cast<Effect*>(m_pBombEffect)->GetAnimationMgr()->GetFrameInfo().frameStart >= dynamic_cast<Effect*>(m_pBombEffect)->GetAnimationMgr()->GetFrameInfo().frameEnd - 1)
	{
		mAttribute.bDie = true;
	}
}

void PBullet::Render(HDC hdc)
{
	if (m_bBulletDie)
		return;

	MyBmp* pBulletBmp = BmpMgr::GetInstance()->GetFindAllBmp(m_pBulletAnimation->GetImageKey(L"PBullet"));
	HDC hBulletDC = pBulletBmp->GetMemDC();

	mInfo.width = (float)pBulletBmp->GetWidth();
	mInfo.height = (float)pBulletBmp->GetHeight();

	UpdateRect();

	GdiTransparentBlt(hdc, mRect.left, mRect.top, (int)mInfo.width, (int)mInfo.height,
		hBulletDC, 0, 0, (int)mInfo.width, (int)mInfo.height, RGB(255, 255, 255));

	m_pBulletAnimation->FrmaeMove();

	TCHAR Bullet[255];
	swprintf_s(Bullet, L"BulletX: %d BulletY: %d", (int)mInfo.X, (int)mInfo.Y);
	//TextOut(hdc, 50, 190, Bullet, lstrlen(Bullet)); // 지정한 좌표에 폰트 출력!
}

void PBullet::BulletCollision()
{
	float X = 0, Y = 0, radius = 0;
	float distance = 0.f;
	size_t tileIndex = 0;
	
	float moveX = 0.f;
	float moveY = 0.f;

	//radius = (80 / 2) + (mInfo.width / 2);

	int widthRadius = (int)((20 / 2) + (mInfo.width / 5));
	int heightRadius = (int)((20 / 2) + (mInfo.height / 5));

	float distanceX = abs(m_pTarget->GetInfo().X - mInfo.X);
	float distanceY = abs(m_pTarget->GetInfo().Y - mInfo.Y);

	if ((widthRadius >= distanceX) && (heightRadius >= distanceY))
	{
		int count = 0;

		moveX = widthRadius - distanceX;
		moveY = heightRadius - distanceY;
		m_pBombEffect->SetEffect(mInfo.X, mInfo.Y, 200, false);
		if (moveX < moveY)
		{
			if (mInfo.X > m_pTarget->GetInfo().X)
			{
				moveX *= -1.f;
			}
			m_pTarget->SetPos(m_pTarget->GetInfo().X + moveX, m_pTarget->GetInfo().Y);
		}
		else
		{
			if (mInfo.Y > m_pTarget->GetInfo().Y)
			{
				moveY *= -1.f;
			}
			m_pTarget->SetPos(m_pTarget->GetInfo().X, m_pTarget->GetInfo().Y + moveY);
		}

		++count;
		if (dynamic_cast<Effect*>(m_pBombEffect)->GetAnimationMgr()->GetFrameInfo().frameStart >= 3)
		{
			m_bBulletDie = true;
		}
		m_pTarget->SetDamage(mAttribute.attack);
		//m_pTarget->SetMode(MODE::DAMAGED);

	}


	//distance = MathMgr::CaclDistance(m_pTarget, this);

	//if (radius > distance)
	//{
	//	m_pTarget->SetMode(MODE::DAMAGED);
	//	m_pBombEffect->SetEffect(mInfo.X, mInfo.Y, 200, false);
	//	m_bBulletDie = true;
	//}

	MathMgr::PointToTileIndex(tileIndex, mInfo.X, mInfo.Y);
	if (TileMgr::GetInstance()->GetTile(tileIndex)->GetDrawID() == TILE_BLOCK)
	{
		m_pBombEffect->SetEffect(mInfo.X, mInfo.Y, 200, false);
		m_bBulletDie = true;
	}

}

void PBullet::Release()
{
	SafeDelete<AnimationMgr*>(m_pBulletAnimation);
}


