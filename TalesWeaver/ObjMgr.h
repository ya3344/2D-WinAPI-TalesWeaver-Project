#pragma once

class Obj;

class ObjMgr
{
	DECLARE_SINGLETON(ObjMgr);

private:
	ObjMgr();
	~ObjMgr();

public:
	bool AddObj(Obj* pObj, OBJ_ID id);
	OBJ_LIST& GetObj(OBJ_ID id) {  return mObjSpace[id]; }
	Obj* GetPlayerObj() const { return mObjSpace[OBJ_PLAYER].back(); }

public:
	void Render(HDC hDC);
	void Update();
	void LateInit();
	void LateUpdate();
	void CollisionUpdate();
	void Release();

public:
	void DeleteObj(int objID);

private:
	OBJ_LIST mObjSpace[OBJ_END];
	OBJ_LIST mObjSortSpace;

};

