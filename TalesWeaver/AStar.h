#pragma once

class Image;

typedef struct tagAStarNode
{
	tagAStarNode()
	{
		ZeroMemory(this, sizeof(tagAStarNode));
	};
	explicit tagAStarNode(float cost, int index, tagAStarNode* pParent, const TCHAR* pMotion)
	{
		this->cost = cost;
		this->index = index;
		this->pParent = pParent;
		this->pMotion = pMotion;
	}

	float cost = 0.f;
	int index = 0;
	tagAStarNode* pParent = nullptr;
	const TCHAR* pMotion = nullptr;

}ASTAR_NODE;

class AStar
{
public:
	AStar();
	~AStar();

public:
	stack<ASTAR_NODE*>& GetBesRoadSpace(void) { return mBestRoadSpace; }
	void Release(void);

public:
	bool AStarStart(size_t startIndex, size_t goalIndex);
public:
	void SetObjID(const int objID) { mObjID = objID; }

private:
	void FindRoute(const vector<Image*>& tileSpace);
	ASTAR_NODE* CreateNode(ASTAR_NODE* pParent, int index, const vector<Image*>& tileSpace, const TCHAR* pMotion);
	bool CheckList(size_t index);
	static bool Compare(ASTAR_NODE* pSrcNode, ASTAR_NODE* pDestNode);
	bool MonsterIndex(int index);

private:
	list<ASTAR_NODE*> mOpenList;
	list<ASTAR_NODE*> mCloseList;

	stack<ASTAR_NODE*> mBestRoadSpace;

private:
	size_t mStartIndex = 0;
	size_t mGoalIndex = 0;

	// Motion Related
	TCHAR mMotion[OBJ_ENEMY+1][OBJ_MOTION::MOTION_END][16] = { { L"L", L"LD",  L"LU", L"U", L"R", L"RU", L"RD", L"D" },
															   { L"LD", L"LD", L"LU", L"LU", L"RU", L"RU", L"RD", L"RD" } };
	int mObjID = 0;


	
};

