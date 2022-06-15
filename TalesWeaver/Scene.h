#pragma once

class Obj;
class Scene abstract
{
public:
	Scene();
	virtual ~Scene();

public:
	virtual void Initialize() = 0;
	virtual void LateInit() = 0;
	virtual void Update() = 0;
	virtual void LateUpdate() = 0;
	virtual void Render(HDC hDC) = 0;
	virtual void Release() = 0;

protected:
	int mMapWidth = 0;
	int mMapHeight = 0;

	Obj* m_pPlayer = nullptr;

};

