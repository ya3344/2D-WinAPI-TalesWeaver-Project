#pragma once

class Obj;

class Image abstract
{
public:
	Image();
	virtual ~Image();

public:
	virtual void Initialize() = 0;
	virtual void LateUpdate() = 0;
	virtual void Render(HDC hDC) = 0;
	virtual void Release() = 0;

public:
	void SetBmpKey(const TCHAR*  pBmpKey) { m_pBmpKey = pBmpKey; }
	void SetPos(const float X, const float Y);
	void SetDrawID(const int drawID) { mDrawID = drawID; }
	void SetEffect(const float X, const float Y, const int alpha, bool bForever, bool bScroll = true, float attack = 0.f);
	void SetImageType(const IMAGE_TYPE imageType) { mImageType = imageType; }
	void SetTarget(Obj* const pObj) { m_pTarget = pObj; }
public:
	const RECT& GetRect() const { return mRect; }
	const INFO& GetInfo() const { return mInfo; }
	const TCHAR* GetBmpKey() const { return m_pBmpKey; }
	int GetDrawID() const { return mDrawID; }
	int GetTileIndex() const { return mTileIndex; }
	IMAGE_TYPE GetImageType() const { return mImageType; }
	int GetAlpha() const { return mAlpha;  }
	float GetAttack() const { return mAttack; }

protected:
	void UpdateRect(bool bScroll = true);

protected:
	INFO mInfo;
	RECT mRect = {};
	const TCHAR* m_pBmpKey = nullptr;

	int mDrawID = 0;
	int mAlpha = 0;
	bool m_bForever = false;
	bool m_bScroll = true;
	int mTileIndex = 0;
	float mAttack = 0.f;
	IMAGE_TYPE mImageType = IMAGE_TYPE::ONLY_VIEW;

	Obj* m_pTarget = nullptr;
};

