#include "stdafx.h"
#include "EffectMgr.h"
#include "Effect.h"
#include "Player.h"

INIT_SINGLETON_VARIABLE(EffectMgr);

EffectMgr::EffectMgr()
{
}


EffectMgr::~EffectMgr()
{
	Release();
}

void EffectMgr::Initialize()
{


}

void EffectMgr::Update()
{
	const TCHAR* pStageKey = dynamic_cast<Player*>(ObjMgr::GetInstance()->GetPlayerObj())->GetStageKey();

	m_pStageCurrentKey = pStageKey;
	size_t warpTileIndex = 0;

	// 이펙트 초기화
/*	EffectMgr::GetInstance()->GetFindEffect(L"WarpGate", false)->SetEffect(0.f, 0.f, 0, false);*/
	if (!lstrcmp(pStageKey, L"House"))
	{

	}
	else if (!lstrcmp(pStageKey, L"Town2"))
	{
		if (m_pStagePreKey != m_pStageCurrentKey)
		{
			EffectMgr::GetInstance()->GetFindEffect(L"WarpGate", false)->SetEffect(674.f, 989.f, 200, true);
			m_pStagePreKey = m_pStageCurrentKey;
		}
	}
	else if (!lstrcmp(pStageKey, L"Field"))
	{
		if (ObjMgr::GetInstance()->GetObj(OBJ_ENEMY).size() == 0 || KeyMgr::GetInstance()->KeyUp(L'M'))
		{
			EffectMgr::GetInstance()->GetFindEffect(L"WarpGate", false)->SetEffect(1670.f, 867.f, 200, true);
			MathMgr::PointToTileIndex(warpTileIndex, 1670.f, 867.f);
			TileMgr::GetInstance()->GetTile(warpTileIndex)->SetDrawID(TILE_WARP_ENTRANCE);
			TileMgr::GetInstance()->GetTile(warpTileIndex - 1)->SetDrawID(TILE_WARP_ENTRANCE);
			TileMgr::GetInstance()->GetTile(warpTileIndex + gTileNum.X)->SetDrawID(TILE_WARP_ENTRANCE);
			TileMgr::GetInstance()->GetTile(warpTileIndex + gTileNum.X - 1)->SetDrawID(TILE_WARP_ENTRANCE);
		}
		else
		{
			if (m_pStagePreKey != m_pStageCurrentKey)
			{
				EffectMgr::GetInstance()->GetFindEffect(L"WarpGate", false)->SetEffect(0.f, 0.f, 0, true);
				m_pStagePreKey = m_pStageCurrentKey;
			}
		}
	}
	else if (!lstrcmp(pStageKey, L"Dungeon"))
	{
		if (ObjMgr::GetInstance()->GetObj(OBJ_ENEMY).size() == 0 || KeyMgr::GetInstance()->KeyUp(L'M'))
		{
			EffectMgr::GetInstance()->GetFindEffect(L"WarpGateRed", false)->SetEffect(2909.f , 505.f, 200, true);
			MathMgr::PointToTileIndex(warpTileIndex, 2909.f , 505.f);
			TileMgr::GetInstance()->GetTile(warpTileIndex)->SetDrawID(TILE_WARP_ENTRANCE);
			TileMgr::GetInstance()->GetTile(warpTileIndex - 1)->SetDrawID(TILE_WARP_ENTRANCE);
			TileMgr::GetInstance()->GetTile(warpTileIndex + gTileNum.X)->SetDrawID(TILE_WARP_ENTRANCE);
			TileMgr::GetInstance()->GetTile(warpTileIndex + gTileNum.X - 1)->SetDrawID(TILE_WARP_ENTRANCE);
		}
		else
		{
			if (m_pStagePreKey != m_pStageCurrentKey)
			{
				EffectMgr::GetInstance()->GetFindEffect(L"WarpGate", false)->SetEffect(0, 0, 0, false);
				MathMgr::PointToTileIndex(warpTileIndex, 1670.f, 867.f);
				TileMgr::GetInstance()->GetTile(warpTileIndex)->SetDrawID(TILE_AVAILABLE);
				TileMgr::GetInstance()->GetTile(warpTileIndex - 1)->SetDrawID(TILE_AVAILABLE);
				TileMgr::GetInstance()->GetTile(warpTileIndex + gTileNum.X)->SetDrawID(TILE_AVAILABLE);
				TileMgr::GetInstance()->GetTile(warpTileIndex + gTileNum.X - 1)->SetDrawID(TILE_AVAILABLE);
				m_pStagePreKey = m_pStageCurrentKey;
			}
		}
	}
	else if (!lstrcmp(pStageKey, L"BossRoom"))
	{
		if (m_pStagePreKey != m_pStageCurrentKey)
		{
			EffectMgr::GetInstance()->GetFindEffect(L"WarpGateRed", false)->SetEffect(0, 0, 0, false);
			MathMgr::PointToTileIndex(warpTileIndex, 2909.f, 505.f);
			TileMgr::GetInstance()->GetTile(warpTileIndex)->SetDrawID(TILE_AVAILABLE);
			TileMgr::GetInstance()->GetTile(warpTileIndex - 1)->SetDrawID(TILE_AVAILABLE);
			TileMgr::GetInstance()->GetTile(warpTileIndex + gTileNum.X)->SetDrawID(TILE_AVAILABLE);
			TileMgr::GetInstance()->GetTile(warpTileIndex + gTileNum.X - 1)->SetDrawID(TILE_AVAILABLE);
			m_pStagePreKey = m_pStageCurrentKey;
		}
	}
}


void EffectMgr::LateUpdate()
{
	for (int i = 0; i < CLASSIFY_END; ++i)
	{
		for (EffectMapPair effectPair : mEffectSpace[i])
		{
			effectPair.second->LateUpdate();
		}
	}
	
}

void EffectMgr::Render(HDC hDC)
{
	for (int i = 0; i < CLASSIFY_END; ++i)
	{
		for (EffectMapPair effectPair : mEffectSpace[i])
		{
			effectPair.second->Render(hDC);
		}
	}
}

void EffectMgr::Release()
{
	for (int i = 0; i < CLASSIFY_END; ++i)
	{
		for_each(mEffectSpace[i].begin(), mEffectSpace[i].end(),
			[](EffectMapPair mapPair)
		{
			if (mapPair.second)
			{
				delete mapPair.second;
				mapPair.second = nullptr;
			}
		}
		);

		mEffectSpace[i].clear();
	}
}

Image * EffectMgr::GetFindEffect(wstring findKey, bool bDynamic)
{
	unordered_map<wstring, Image*>::iterator iterFind;
	if (bDynamic)
	{
		iterFind = mEffectSpace[DYNAMIC].find(findKey);

		if (iterFind == mEffectSpace[DYNAMIC].end())
		{
			MESSAGE_BOX(L"Dynamic Effect Find fail!!");
		}
	}
	else
	{
		iterFind = mEffectSpace[STATIC].find(findKey);

		if (iterFind == mEffectSpace[STATIC].end())
		{
			MESSAGE_BOX(L"Static Effect Find fail!!");
		}
	}


	return iterFind->second;
}

Image * EffectMgr::GetFindAllEffect(wstring imageKey)
{
	unordered_map<wstring, Image*>::iterator iterFind;

	iterFind = mEffectSpace[DYNAMIC].find(imageKey);

	if (iterFind != mEffectSpace[DYNAMIC].end())
	{
		return iterFind->second;
	}
	else
	{
		iterFind = mEffectSpace[STATIC].find(imageKey);

		if (iterFind == mEffectSpace[STATIC].end())
		{
			return nullptr;
		}
	}


	return iterFind->second;
}

void EffectMgr::AddEffect(const TCHAR * pBmpKey, const TCHAR * pImageLocation, size_t motionMaxNum, size_t frameSpeed, IMAGE_TYPE imageType, bool bDynamic)
{
	Image* pEffect = new Effect(pBmpKey, pImageLocation, motionMaxNum, frameSpeed);
	dynamic_cast<Effect*>(pEffect)->SetDynamic(bDynamic);
	pEffect->SetImageType(imageType);
	pEffect->Initialize();

	if (bDynamic)
	{
		mEffectSpace[DYNAMIC].emplace(EffectMapPair(pBmpKey, pEffect));
	}
	else
	{
		mEffectSpace[STATIC].emplace(EffectMapPair(pBmpKey, pEffect));
	}
		
}

void EffectMgr::DeleteDynamicEffectSpace()
{
	for (EffectMapPair mapPair : mEffectSpace[DYNAMIC])
	{
		if (mapPair.second)
		{
			delete mapPair.second;
			mapPair.second = nullptr;
		}
	}

	mEffectSpace[DYNAMIC].clear();
}
