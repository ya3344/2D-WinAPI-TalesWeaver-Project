#include "stdafx.h"
#include "Mouse.h"
#include "AnimationMgr.h"

Mouse::Mouse()
{
}


Mouse::~Mouse()
{
}

void Mouse::Initialize()
{
	ShowCursor(false);
	m_pMouseCurrentKey = L"MouseNormal";
	
}

void Mouse::LateInit()
{
	
}

int Mouse::Update()
{
	GetCursorPos(&mPt);
	ScreenToClient(g_hWnd, &mPt);

	mInfo.X = (float)(mPt.x);
	mInfo.Y = (float)(mPt.y);

	EffectMgr::GetInstance()->GetFindEffect(L"MouseNormal", false)->SetEffect(mInfo.X, mInfo.Y, 200, true, false);

	if (m_bFieldScene == false)
		return OBJ_NORMAL;

	EffectMgr::GetInstance()->GetFindEffect(L"MouseNormal", false)->SetEffect(0.f, 0.f, 0, false, false);
	float scrollX = ScrollMgr::GetInstance()->GetScrollX();
	float scrollY = ScrollMgr::GetInstance()->GetScrollY();

	MathMgr::PointToTileIndex(mTileIndex, ((mInfo.X - 16) - scrollX), ((mInfo.Y - 16) - scrollY));

	if (KeyMgr::GetInstance()->KeyDown(VK_F5))
	{
		m_bMouseMagic = !m_bMouseMagic;
	}

	if (TileMgr::GetInstance()->GetTile(mTileIndex)->GetDrawID() == TILE_AVAILABLE ||
		TileMgr::GetInstance()->GetTile(mTileIndex)->GetDrawID() == TILE_ENEMY_POSITION )
	{
		if (m_bMouseMagic)
		{
			m_pMouseCurrentKey = L"MouseMagic";
			EffectMgr::GetInstance()->GetFindEffect(L"NuckBackIcon", false)->SetEffect(0.f, 0.f, 0, false, false);
			EffectMgr::GetInstance()->GetFindEffect(L"MultAttackIcon", false)->SetEffect(0.f, 0.f, 0, false, false);
			EffectMgr::GetInstance()->GetFindEffect(L"BoltIcon", false)->SetEffect(70.f, 102.f, 255, true, false);
		}
		else
		{
			EffectMgr::GetInstance()->GetFindEffect(L"BoltIcon", false)->SetEffect(0.f, 0.f, 0, false, false);
			m_pMouseCurrentKey = L"MouseNormal";
		}
			
	}
	else if (TileMgr::GetInstance()->GetTile(mTileIndex)->GetDrawID() == TILE_BLOCK)
	{
		m_pMouseCurrentKey = L"MouseNoPass";
		m_bMouseMagic = false;
	}
	else if (TileMgr::GetInstance()->GetTile(mTileIndex)->GetDrawID() == TILE_WARP_ENTRANCE ||
		TileMgr::GetInstance()->GetTile(mTileIndex)->GetDrawID() == TILE_WARP_EXIT)
	{
		m_pMouseCurrentKey = L"MouseGate";
		m_bMouseMagic = false;
	}


	for (Obj*pObj : ObjMgr::GetInstance()->GetObj(OBJ_ENEMY))
	{
		if (mTileIndex == pObj->GetTileIndex())
		{
			if (m_bMouseMagic)
			{
				m_pMouseCurrentKey = L"MouseMagic";
			}
			else
				m_pMouseCurrentKey = L"MouseBattle";
		}
	}

	if (m_pMouseCurrentKey != m_pMousePreKey)
	{
		if (m_pMouseCurrentKey == L"MouseNormal")
		{
			EffectMgr::GetInstance()->GetFindEffect(L"MouseNoPass", false)->SetEffect(0, 0, 0, false, false);
			EffectMgr::GetInstance()->GetFindEffect(L"MouseGate", false)->SetEffect(0, 0, 0, false, false);
			EffectMgr::GetInstance()->GetFindEffect(L"MouseBattle", false)->SetEffect(0, 0, 0, false, false);
			EffectMgr::GetInstance()->GetFindEffect(L"MouseMagic", false)->SetEffect(0, 0, 0, false, false);
		}
		else if (m_pMouseCurrentKey == L"MouseNoPass")
		{
			EffectMgr::GetInstance()->GetFindEffect(L"MouseNormal", false)->SetEffect(0, 0, 0, false, false);
			EffectMgr::GetInstance()->GetFindEffect(L"MouseGate", false)->SetEffect(0, 0, 0, false, false);
			EffectMgr::GetInstance()->GetFindEffect(L"MouseBattle", false)->SetEffect(0, 0, 0, false, false);
			EffectMgr::GetInstance()->GetFindEffect(L"MouseMagic", false)->SetEffect(0, 0, 0, false, false);
		}
		else if (m_pMouseCurrentKey == L"MouseGate")
		{
			EffectMgr::GetInstance()->GetFindEffect(L"MouseNormal", false)->SetEffect(0, 0, 0, false, false);
			EffectMgr::GetInstance()->GetFindEffect(L"MouseNoPass", false)->SetEffect(0, 0, 0, false, false);
			EffectMgr::GetInstance()->GetFindEffect(L"MouseBattle", false)->SetEffect(0, 0, 0, false, false);
			EffectMgr::GetInstance()->GetFindEffect(L"MouseMagic", false)->SetEffect(0, 0, 0, false, false);
		}
		else if (m_pMouseCurrentKey == L"MouseBattle")
		{
			EffectMgr::GetInstance()->GetFindEffect(L"MouseNormal", false)->SetEffect(0, 0, 0, false, false);
			EffectMgr::GetInstance()->GetFindEffect(L"MouseNoPass", false)->SetEffect(0, 0, 0, false, false);
			EffectMgr::GetInstance()->GetFindEffect(L"MouseGate", false)->SetEffect(0, 0, 0, false, false);
			EffectMgr::GetInstance()->GetFindEffect(L"MouseMagic", false)->SetEffect(0, 0, 0, false, false);
		}
		else if (m_pMouseCurrentKey == L"MouseMagic")
		{
			EffectMgr::GetInstance()->GetFindEffect(L"MouseNormal", false)->SetEffect(0, 0, 0, false, false);
			EffectMgr::GetInstance()->GetFindEffect(L"MouseNoPass", false)->SetEffect(0, 0, 0, false, false);
			EffectMgr::GetInstance()->GetFindEffect(L"MouseBattle", false)->SetEffect(0, 0, 0, false, false);
			EffectMgr::GetInstance()->GetFindEffect(L"MouseGate", false)->SetEffect(0, 0, 0, false, false);
		}
		
		m_pMousePreKey = m_pMouseCurrentKey;
	}

	UpdateRect(false);
	EffectMgr::GetInstance()->GetFindEffect(m_pMouseCurrentKey, false)->SetEffect(mInfo.X, mInfo.Y, 200, true, false);

	return OBJ_NORMAL;
}

void Mouse::Render(HDC hdc)
{
	/*Ellipse(hdc, mRect.left, mRect.top, mRect.right, mRect.bottom);*/

	TCHAR test[128] = L"";
	swprintf_s(test, L"MouseX:%f MouseY:%f", mInfo.X, mInfo.Y);
	//TextOut(hdc, 50, 110, test, lstrlen(test));
}

void Mouse::LateUpdate()
{
	
}

void Mouse::Release()
{
}
