#include "stdafx.h"
#include "AStar.h"
#include "Obj.h"

AStar::AStar()
{
	mOpenList.clear();
	mCloseList.clear();
}

AStar::~AStar()
{
	Release();
}

bool AStar::AStarStart(size_t startIndex, size_t goalIndex)
{
	if (startIndex == goalIndex)
	{
		return false;
	}

	const vector<Image*>& tileSpace = TileMgr::GetInstance()->GetTileSpace();

	if (goalIndex < 0 || goalIndex >= tileSpace.size())
		return false;

	if (tileSpace[goalIndex]->GetDrawID() == TILE_BLOCK)
		return false;

	mStartIndex = startIndex;
	mGoalIndex = goalIndex;

	Release();
	
	FindRoute(tileSpace);

	if (mBestRoadSpace.empty())
		return false;

	return true;
}

void AStar::FindRoute(const vector<Image*>& tileSpace)
{
	// 여덟 방향을 조사한다.
	ASTAR_NODE* pParent = new ASTAR_NODE(0.f, (int)mStartIndex, nullptr, L"Me");
	mCloseList.emplace_back(pParent);

	ASTAR_NODE* pNode = nullptr;
	int count = 0;

	while (true)
	{
		if (count >= 15)
		{
			Release();
			return;
		}

		// 위
		int index = pParent->index - gTileNum.X;

		if (pParent->index >= gTileNum.X && tileSpace[index]->GetDrawID() != TILE_BLOCK
			&& CheckList(index))
		{
			pNode = CreateNode(pParent, index, tileSpace, mMotion[mObjID][OBJ_MOTION::UP]);
			mOpenList.emplace_back(pNode);
		}

		// 오른쪽 위
		index = (pParent->index - gTileNum.X) + 1;

		if (pParent->index >= gTileNum.X && (pParent->index % gTileNum.X) != gTileNum.X - 1
			&& tileSpace[index]->GetDrawID() != TILE_BLOCK && CheckList(index))
		{
			pNode = CreateNode(pParent, index, tileSpace, mMotion[mObjID][OBJ_MOTION::RU]);
			mOpenList.emplace_back(pNode);
		}

		// 오른쪽
		index = pParent->index + 1;

		if ((pParent->index % gTileNum.X) != gTileNum.X - 1
			&& tileSpace[index]->GetDrawID() != TILE_BLOCK && CheckList(index))
		{
			pNode = CreateNode(pParent, index, tileSpace, mMotion[mObjID][OBJ_MOTION::RIGHT]);
			mOpenList.emplace_back(pNode);
		}

		// 오른쪽 아래 
		index = (pParent->index + gTileNum.X) + 1;

		if ((pParent->index / gTileNum.X) < gTileNum.X - 1  && (pParent->index % gTileNum.X) != gTileNum.X - 1
			&& tileSpace[index]->GetDrawID() != TILE_BLOCK && CheckList(index))
		{
			pNode = CreateNode(pParent, index, tileSpace, mMotion[mObjID][OBJ_MOTION::RD]);
			mOpenList.emplace_back(pNode);
		}

		// 아래
		index = pParent->index + gTileNum.X;

		if ((pParent->index / gTileNum.X) < gTileNum.X - 1
			&& tileSpace[index]->GetDrawID() != TILE_BLOCK && CheckList(index))
		{
			pNode = CreateNode(pParent, index, tileSpace, mMotion[mObjID][OBJ_MOTION::DOWN]);
			mOpenList.emplace_back(pNode);
		}

		// 왼쪽 아래
		index = (pParent->index + gTileNum.X) - 1;

		if ((pParent->index / gTileNum.X) < gTileNum.X - 1 && (pParent->index % gTileNum.X) > 0
			&& tileSpace[index]->GetDrawID() != TILE_BLOCK && CheckList(index))
		{
			pNode = CreateNode(pParent, index, tileSpace, mMotion[mObjID][OBJ_MOTION::LD]);
			mOpenList.emplace_back(pNode);
		}

		// 왼쪽
		index = pParent->index - 1;

		if ((pParent->index % gTileNum.X) > 0
			&& tileSpace[index]->GetDrawID() != TILE_BLOCK && CheckList(index))
		{
			pNode = CreateNode(pParent, index, tileSpace, mMotion[mObjID][OBJ_MOTION::LEFT]);
			mOpenList.emplace_back(pNode);
		}

		// 왼쪽 위 
		index = (pParent->index - gTileNum.X) - 1;
		if ((pParent->index % gTileNum.X) > 0 && pParent->index >= gTileNum.X
			&& tileSpace[index]->GetDrawID() != TILE_BLOCK && CheckList(index))
		{
			pNode = CreateNode(pParent, index, tileSpace, mMotion[mObjID][OBJ_MOTION::LU]);
			mOpenList.emplace_back(pNode);
		}

		if (mOpenList.empty())
		{
			return;
		}
			

		// 가중치 기준으로 소팅한다.
		mOpenList.sort(Compare);

		auto& iterOpenList = mOpenList.begin();
		mCloseList.emplace_back(*iterOpenList);

		pParent = *iterOpenList;
		mOpenList.erase(iterOpenList);

		if (pParent->index == mGoalIndex)
		{
			while (true)
			{
				// 경로를 생성해준다.
				if (pParent->pParent == nullptr)
					break;

				mBestRoadSpace.emplace(pParent);
				pParent = pParent->pParent;
			}
			break;
		}

		++count;
	}
}

ASTAR_NODE * AStar::CreateNode(ASTAR_NODE* pParent, int index, const vector<Image*>& tileSpace, const TCHAR* pMotion)
{
	float G = MathMgr::CaclDistance(tileSpace[pParent->index], tileSpace[index]);
	float H = MathMgr::CaclDistance(tileSpace[index], tileSpace[mGoalIndex]);

	ASTAR_NODE* pNode = new ASTAR_NODE(G+H, index, pParent, pMotion);

	return pNode;
}

bool AStar::CheckList(size_t index)
{
	for (ASTAR_NODE* pNode : mOpenList)
	{
		if (pNode->index == index)
			return false;
	}

	for (ASTAR_NODE* pNode : mCloseList)
	{
		if (pNode->index == index)
			return false;
	}

	return true;
}

bool AStar::Compare(ASTAR_NODE * pSrcNode, ASTAR_NODE * pDestNode)
{
	return pSrcNode->cost < pDestNode->cost;
}

bool AStar::MonsterIndex(int index)
{
	/*for (Obj* pObj : ObjMgr::GetInstance()->GetObj(OBJ_ENEMY))
	{
		if (pObj->GetTileIndex() == index)
			return false;
	}*/
	return true;
}

void AStar::Release(void)
{
	for_each(mOpenList.begin(), mOpenList.end(), SafeDelete<ASTAR_NODE*>);
	mOpenList.clear();

	for_each(mCloseList.begin(), mCloseList.end(), SafeDelete<ASTAR_NODE*>);
	mCloseList.clear();
	
	while (mBestRoadSpace.empty() == false)
	{
		mBestRoadSpace.pop();
	}
}
