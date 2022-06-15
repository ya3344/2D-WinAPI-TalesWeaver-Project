#pragma once

typedef struct tagAttribute
{
	tagAttribute()
	{
		ZeroMemory(this, sizeof(tagAttribute));
	};


	float maxHP = 0.f;
	float currentHP = 0.f;
	float maxMP = 0.f;
	float currentMP = 0.f;
	float maxSP = 0.f;
	float currentSP = 0.f;
	float degree = 0.f;
	float speed = 0.f;
	float attack = 0.f;
	MODE mode;
	bool bDie = false;
	bool bCollision = false;

	int type = 0;

}ATTRIBUTE;

class Obj abstract
{
public:
	Obj();
	virtual ~Obj();

public:
	virtual void Initialize() = 0;
	virtual void LateInit() = 0;
	virtual int Update() = 0;
	virtual void Render(HDC hdc) = 0;
	virtual void LateUpdate() = 0;
	virtual void Release() = 0;

public:
	const RECT& GetRect() const { return mRect; }
	const RECT& GetCollisionRect() const { return mCollisionRect; }
	const INFO& GetInfo() const { return mInfo; }

	const ATTRIBUTE& GetAttriBute() const { return mAttribute; }
	size_t GetTileIndex() const { return mTileIndex; }
	size_t GetCurScene() const { return mCurScene; }
	size_t GetPreScene() const { return mPreScene; }
	RECT CollisionRect(float X, float Y, int Width, int Height);
	int GetCollisionWidht() const { return mCollisionWidth; }
	int GetCollisionHeight() const { return mCollisionHeight; }

public:
	void SetInfo(const INFO info) { mInfo = info; }
	void SetDie(const bool bDie) { mAttribute.bDie = bDie; }
	void SetAngle(const float degree) { mAttribute.degree = degree; }
	void SetPos(const float X, const float Y);
	void SetDirection(const OBJECT_DIRECTION direction) { mDirection = direction; }
	void SetDamage(const float attack);
	void SetTarget(Obj* const pObj) { m_pTarget = pObj; }
	void SetCurScene(const size_t curScene) { mCurScene = curScene; }
	void SetCollision(const bool bCollision) { mAttribute.bCollision = bCollision; }
	void SetMode(const MODE mode) { mAttribute.mode = mode; }
protected:
	void UpdateRect(bool bScroll = true);
	
	
protected:
	INFO mInfo;
	RECT mRect = {};
	ATTRIBUTE mAttribute;

	OBJECT_DIRECTION mDirection;

protected:
	Obj* m_pTarget = nullptr;
	size_t mTileIndex = 0;
	RECT mCollisionRect = {};
	int mCollisionWidth = 0;
	int mCollisionHeight = 0;

protected:
	// Animatino Related
	size_t mCurScene = 0;
	size_t mPreScene = 0;

};

