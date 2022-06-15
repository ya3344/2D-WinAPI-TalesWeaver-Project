#include "stdafx.h"
#include "Obj.h"
#include "ObjMgr.h"

INIT_SINGLETON_VARIABLE(ObjMgr);

ObjMgr::ObjMgr()
{
}


ObjMgr::~ObjMgr()
{
	Release();
}

bool ObjMgr::AddObj(Obj* pObj, OBJ_ID id)
{
	if (pObj != nullptr)
	{
		mObjSpace[id].emplace_back(pObj);
		return true;
	}
		
	return false;
}

void ObjMgr::LateInit()
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : mObjSpace[i])
		{
			pObj->LateInit();
		}
	}
}

void ObjMgr::Update()
{
	int objEvent;
	for (int i = 0; i < OBJ_END; ++i)
	{
		auto iterBegin = mObjSpace[i].begin();
		for (; iterBegin != mObjSpace[i].end();)
		{
			objEvent = (*iterBegin)->Update();
			if (objEvent == OBJ_DIE)
			{
				SafeDelete<Obj*>(*iterBegin);
				iterBegin = mObjSpace[i].erase(iterBegin);
			}
			else if (objEvent == OBJ_NORMAL)
				++iterBegin;
			else
			{
				MessageBox(g_hWnd, L"ObjMgr::Update()", nullptr, MB_OK);
			}
		}
	}
}



void ObjMgr::LateUpdate()
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : mObjSpace[i])
		{
			pObj->LateUpdate();
		}
	}
}

void ObjMgr::CollisionUpdate()
{
	CollisionMgr::GetInstance()->CollisionRectEX(mObjSpace[OBJ_ENEMY], mObjSpace[OBJ_ENEMY]);
	//CollisionMgr::EnemyEnemyCollision(mObjSpace[OBJ_ENEMY], mObjSpace[OBJ_ENEMY]);
	CollisionMgr::GetInstance()->PlayerEnemyCollision(mObjSpace[OBJ_PLAYER], mObjSpace[OBJ_ENEMY]);
	//CollisionMgr::CollisionRectEX(mObjSpace[OBJ_PLAYER], mObjSpace[OBJ_ENEMY]);
	CollisionMgr::GetInstance()->CollsionEnemyEffect(mObjSpace[OBJ_PLAYER] , EffectMgr::GetInstance()->GetEffectSpace(STATIC));
	CollisionMgr::GetInstance()->CollsionPlayerEffect(mObjSpace[OBJ_ENEMY], EffectMgr::GetInstance()->GetEffectSpace(STATIC));

	//CollisionMgr::CollisionCircle(mObjSpace[OBJ_ENEMY], mObjSpace[OBJ_PLAYER]);
	CollisionMgr::GetInstance()->WeaponEnemyCollsion(mObjSpace[OBJ_PLAYER], mObjSpace[OBJ_ENEMY]);
}


void ObjMgr::Render(HDC hDC)
{
	// 모든오브젝트를 정렬공간에 Psuh
	for (int i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : mObjSpace[i])
		{
			mObjSortSpace.emplace_back(pObj);

		}
	}
	// Y좌표 기준으로 오름차순으로 정렬
	mObjSortSpace.sort(
		[](Obj* pObj1, Obj* pObj2) -> bool
		{
			return pObj1->GetInfo().Y > pObj2->GetInfo().Y;
		}
	);

	// 정렬순서대로 렌더링
	for (Obj* pObj : mObjSortSpace)
	{
		pObj->Render(hDC);
	}
	mObjSortSpace.clear();



	/*for (int i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : mObjSpace[i])
		{
			pObj->Render(hdc);
		}
	}*/
}

void ObjMgr::Release()
{
	for (auto& iterObjList : mObjSpace)
	{
		for_each(iterObjList.begin(), iterObjList.end(), SafeDelete<Obj*>);
		iterObjList.clear();
	}
}

void ObjMgr::DeleteObj(int objID)
{
	for (Obj* pObj : mObjSpace[objID])
	{
		SafeDelete<Obj*>(pObj);
	}

	mObjSpace[objID].clear();
}

