#include "stdafx.h"
#include "Effect.h"
#include "AnimationMgr.h"


Effect::Effect()
{
}

Effect::Effect(const TCHAR* pBmpKey, const TCHAR* pImageLocation, size_t motionMaxNum, size_t frameSpeed)
{
	lstrcpy(mBmpKey, pBmpKey);
	m_pImageLocation = pImageLocation;
	mMotionMaxNum = motionMaxNum;
	mFrameSpeed = frameSpeed;
}


Effect::~Effect()
{
	Release();
}

void Effect::Initialize()
{
	if (m_pAnimationMgr == nullptr)
	{
		m_pAnimationMgr = new AnimationMgr(m_pImageLocation, mBmpKey, mMotionMaxNum);
		if (m_pImageLocation != nullptr)
		{
			if (m_pAnimationMgr->EffectLoadBmp(m_bDynamic))
			{
				m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, mMotionMaxNum, nullptr, GetTickCount(), mFrameSpeed));
			}
		}
		else
		{
			m_pAnimationMgr->SetFrameInfo(FRAME_INFO(0, mMotionMaxNum, nullptr, GetTickCount(), mFrameSpeed));
		}

		MyBmp* pMyAlphaBmp = BmpMgr::GetInstance()->GetFindBmp(L"Alpha",false);
		if (pMyAlphaBmp == nullptr)
		{
			MESSAGE_BOX(L"AlphaFind Fail!");
		}

		m_hAlphaDC = pMyAlphaBmp->GetMemDC();
	}
}

void Effect::LateUpdate()
{
	
}

void Effect::Render(HDC hDC)
{
	if (mAlpha > 0)
	{
		MyBmp* pEffectBmp = BmpMgr::GetInstance()->GetFindAllBmp(m_pAnimationMgr->GetImageKey(mBmpKey));
		HDC hEffectDC = pEffectBmp->GetMemDC();

		mInfo.width = (float)pEffectBmp->GetWidth();
		mInfo.height = (float)pEffectBmp->GetHeight();

		if(m_bScroll)
			UpdateRect();
		else
			UpdateRect(false);
		/*HDC alphaDC = CreateCompatibleDC(hEffectDC);
		HBITMAP hBitMap = CreateCompatibleBitmap(hEffectDC, (int)mInfo.width, (int)mInfo.height);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(alphaDC, hBitMap);
		BitBlt(alphaDC, 0, 0, (int)mInfo.width, (int)mInfo.height, hEffectDC, 0, 0, SRCCOPY);*/

		BitBlt(m_hAlphaDC, 0, 0, (int)WINDOW_WIDTH, (int)WINDOW_HEIGHT,
			hDC, 0, 0, SRCCOPY);


		GdiTransparentBlt(m_hAlphaDC, mRect.left, mRect.top, (int)mInfo.width, (int)mInfo.height,
			hEffectDC, 0, 0, (int)mInfo.width, (int)mInfo.height, RGB(255, 255, 255));
		

		mBlendInfo.BlendOp = AC_SRC_OVER; // 블랜딩 옵션 : 혼합옵션
		mBlendInfo.BlendFlags = 0; // 플래그 0처리
		mBlendInfo.AlphaFormat = 0;
		mBlendInfo.SourceConstantAlpha = mAlpha; // 알파채널값 0 ~ 255(완전 불투명)

		GdiAlphaBlend(hDC, mRect.left, mRect.top, (int)mInfo.width, (int)mInfo.height, m_hAlphaDC, mRect.left, mRect.top, (int)mInfo.width, (int)mInfo.height, mBlendInfo);

		m_pAnimationMgr->FrmaeMove();
		// Debug
		/*TCHAR effectStr[256] = L"";
		swprintf_s(effectStr, L"Effect: %s  Left:%d  Top:%d  Right:%d  Bottom:%d", m_pAnimationMgr->GetImageKey(mBmpKey)
			, mRect.left, mRect.top, mRect.right, mRect.bottom);
		TextOut(hDC, 50, 110, effectStr, lstrlen(effectStr));*/

		// 계속해서 생성하는 것이 아니면 알파값을 0으로 초기화하여 한번만 동작하도록 설정.
		if (m_bForever == false && m_pAnimationMgr->GetFrameInfo().frameStart >= mMotionMaxNum)
		{
			mAlpha = 0;
			mInfo = INFO(0.f, 0.f, 0.f, 0.f);
			mRect = {};
 			m_pAnimationMgr->SetFrameStart(0);
		}
			
	}
}

void Effect::Release()
{
	SafeDelete<AnimationMgr*>(m_pAnimationMgr);
}

void Effect::SetFrameStart(const int frameStart)
{
	m_pAnimationMgr->SetFrameStart(frameStart); 
}

void Effect::SetMultiEffect(size_t motionMaxNum, const TCHAR * pStr)
{
	mMotionMaxNum = motionMaxNum;
	lstrcpy(mBmpKey, pStr);
}

//void Effect::SetFrameInit(size_t motionMaxNum, size_t frameSpeed)
//{
//	mMotionMaxNum = motionMaxNum;
//	mFrameSpeed = frameSpeed;
//}
