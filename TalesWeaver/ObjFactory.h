#pragma once

class Obj;

template <typename T> 
class ObjFactory
{
public:
	static Obj* CreateObj()
	{
		Obj* pObj = new T;
		pObj->Initialize();

		return pObj;
	}
	static Obj* CreateObj(Obj* pTarget)
	{
		Obj* pObj = new T;
		pObj->Initialize();
		pObj->SetTarget(pTarget);

		return pObj;
	}

	static Obj* CreateObj(Obj* pTarget, const INFO info)
	{
		Obj* pObj = new T;
		pObj->Initialize();
		pObj->SetTarget(pTarget);
		pObj->SetInfo(info);

		return pObj;
	}

	static Obj* CreateObj(const INFO info)
	{
		Obj* pObj = new T;
		pObj->SetInfo(info);
		pObj->Initialize();

		return pObj;
	}

	static Obj* CreateObj(const float X, const float Y, const float degree, const OBJECT_DIRECTION direction )
	{
		Obj* pObj = new T;
		pObj->Initialize();
		pObj->SetPos(X, Y);
		pObj->SetAngle(degree);
		pObj->SetDirection(direction);

		return pObj;
	}
};

