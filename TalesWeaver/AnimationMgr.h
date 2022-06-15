#pragma once

typedef struct tagFrame
{
	tagFrame()
	{
		ZeroMemory(this, sizeof(tagFrame));
	};
	explicit tagFrame(size_t frameStart, size_t frameEnd, const TCHAR* pScenekey, DWORD frameTime, size_t frameSpeed)
	{
		this->frameStart = frameStart;
		this->frameEnd = frameEnd;
		this->pScenekey = pScenekey;
		this->frameTime = frameTime;
		this->frameSpeed = frameSpeed;
	}

	size_t frameStart = 0;
	size_t frameEnd = 0;
	const TCHAR*  pScenekey = nullptr;

	DWORD frameTime = 0;
	size_t frameSpeed = 0;
}FRAME_INFO;

typedef struct tagAnimation
{
	tagAnimation()
	{
		ZeroMemory(this, sizeof(tagAnimation));
	};
	explicit tagAnimation(ANIM_TYPE objType, const TCHAR* pImageLocation, const TCHAR* pImageKey, size_t sceneMaxNum, size_t motionMaxNUm)
	{
		this->objType = objType;
		wcscpy_s(this->imageLocation, pImageLocation);
		wcscpy_s(this->imageKey, pImageKey);
		this->sceneMaxNum = sceneMaxNum;
		this->motionMaxNum = motionMaxNUm;
	};
	TCHAR imageKey[64] = {};
	TCHAR imageLocation[256] = {};

	size_t sceneMaxNum = 0;
	size_t motionMaxNum = 0;

	ANIM_TYPE objType = ANIM_TYPE::PLAYER;

}ANIMATION_INFO;


class AnimationMgr
{
public:
	AnimationMgr();
	explicit AnimationMgr(ANIMATION_INFO animationInfo);
	explicit AnimationMgr(const TCHAR * pImageLocation, const TCHAR * pImageKey, size_t motonMaxNum);
	~AnimationMgr();

public:
	void SetFrameInfo(const FRAME_INFO frame) { mFrameInfo = frame; }
	void SetFrameStart(const int frameStart) {
		mFrameInfo.frameStart = frameStart;
	}
	void SetImageKey(const TCHAR* pImageKey);
	void FrmaeMove();

	bool ObjLoadBmp(bool bDynamic);
	bool EffectLoadBmp(bool bDynamic);

public:
	const FRAME_INFO& GetFrameInfo() { return mFrameInfo; }
	const TCHAR* GetImageKey(const TCHAR* pImageKey);

private:


private:
	FRAME_INFO mFrameInfo;
	ANIMATION_INFO mAnimationInfo;
	TCHAR mImageKey[64];
};

