#include "stdafx.h"
#include "Bullet.h"
#include "AnimationMgr.h"
#include "Effect.h"
Bullet::Bullet() :
	mRotAngle(90.f), mRotSpeed(50.f), mDistance(3.f)
{
	
}


Bullet::~Bullet()
{
	Release();
}

void Bullet::Initialize()
{
	mAttribute.speed = 13.f;
	mAttribute.attack = 1.f;

	if (m_pBulletAnimation == nullptr)
	{
		m_pBulletAnimation = new AnimationMgr();
		m_pBulletAnimation->SetFrameInfo(FRAME_INFO(0, (size_t)EFFECT_FRAME::BULLET, nullptr, GetTickCount(), (size_t)FRAME_SPEED::BULLET));
	}


	if (m_pPopEffect == nullptr)
	{
		m_pPopEffect = new Effect(L"Pop", nullptr, (size_t)EFFECT_FRAME::POP, (size_t)FRAME_SPEED::POP);
		m_pPopEffect->Initialize();
	}


	
}

void Bullet::LateInit()
{

}
int Bullet::Update()
{
	if (m_bInit == false)
	{
		mRadian = MathMgr::CaclRadian(this, m_pTarget);
		mCenterX = mInfo.X;
		mCenterY = mInfo.Y;
		m_bInit = true;
	}

	if (m_bBulletDie == false)
	{
		/*mInfo.X += cosf(mRadian) * mAttribute.speed;
		mInfo.Y -= sinf(mRadian) * mAttribute.speed;*/
		mCenterX += cosf(mRadian) * mAttribute.speed;
		mCenterY -= sinf(mRadian) * mAttribute.speed;

		mInfo.X = mCenterX + cosf(ToRadian(mRotAngle)) * mDistance;
		mInfo.Y = mCenterY - sinf(ToRadian(mRotAngle)) * mDistance;

		mRotAngle += mRotSpeed;
	}


	if (mAttribute.bDie)
		return OBJ_DIE;

	return OBJ_NORMAL;
}

void Bullet::LateUpdate()
{

	//if (mRect.left <= 0 || mRect.right >= WINDOW_WIDTH || mRect.top <= 0 || mRect.bottom >= WINDOW_HEIGHT)
	//	mAttribute.bDie = true;
	BulletCollision();
	if (dynamic_cast<Effect*>(m_pPopEffect)->GetAnimationMgr()->GetFrameInfo().frameStart >= dynamic_cast<Effect*>(m_pPopEffect)->GetAnimationMgr()->GetFrameInfo().frameEnd - 1)
	{
		mAttribute.bDie = true;
	}

}

void Bullet::Render(HDC hdc)
{
	m_pPopEffect->Render(hdc);

	if (m_bBulletDie)
		return;

	MyBmp* pBulletBmp = BmpMgr::GetInstance()->GetFindAllBmp(m_pBulletAnimation->GetImageKey(L"Bullet"));
	HDC hBulletDC = pBulletBmp->GetMemDC();

	mInfo.width = (float)pBulletBmp->GetWidth();
	mInfo.height = (float)pBulletBmp->GetHeight();

	UpdateRect();

	GdiTransparentBlt(hdc, mRect.left, mRect.top, (int)mInfo.width, (int)mInfo.height,
		hBulletDC, 0, 0, (int)mInfo.width, (int)mInfo.height, RGB(255, 255, 255));


	m_pBulletAnimation->FrmaeMove();

	TCHAR Bullet[255];
	swprintf_s(Bullet, L"BulletX: %d BulletY: %d" , (int)mInfo.X, (int)mInfo.Y);
	//TextOut(hdc, 50, 190, Bullet, lstrlen(Bullet)); // 지정한 좌표에 폰트 출력!


}


void Bullet::BulletCollision()
{
	//RECT playerRect = CollisionRect(m_pTarget->GetInfo.X, m_pTarget->GetInfo.Y, PLAYER_STAND_WIDHT, PLAYER_STAND_HEIGHT);

	float X = 0, Y = 0, radius = 0;
	float distance = 0.f;
	size_t tileIndex = 0;
	radius = (80 / 2) + (mInfo.width / 2);

	distance = MathMgr::CaclDistance(m_pTarget, this);

	if (radius > distance)
	{
		//m_pTarget->SetMode(MODE::DAMAGED);
		m_pTarget->SetDamage(mAttribute.attack);
		m_pPopEffect->SetEffect(mInfo.X, mInfo.Y, 200, false);
		m_bBulletDie = true;
	}

	MathMgr::PointToTileIndex(tileIndex, mInfo.X, mInfo.Y);
	if (TileMgr::GetInstance()->GetTile(tileIndex)->GetDrawID() == TILE_BLOCK)
	{
		m_pPopEffect->SetEffect(mInfo.X, mInfo.Y, 200, false);
		m_bBulletDie = true;
	}
		

	//if (mRect.left <= 0 || mRect.right >= WINDOW_WIDTH || mRect.top <= 0 || mRect.bottom >= WINDOW_HEIGHT)
	//	mAttribute.bDie = true;
	
}


void Bullet::Release()
{
	SafeDelete<AnimationMgr*>(m_pBulletAnimation);
	SafeDelete<Image*>(m_pPopEffect);
}