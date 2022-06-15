#pragma once

class AnimationMgr;

class Effect final : public Image
{
public:
	Effect();
	explicit Effect(const TCHAR* pBmpKey, const TCHAR* pImageLocation, size_t motionMaxNum, size_t frameSpeed);
	virtual ~Effect();

	// Image을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	void SetFrameStart(const int frameStart);
	void SetDynamic(const bool bDynamic) { m_bDynamic = bDynamic;}
	void SetMultiEffect(size_t motionMaxNum, const TCHAR* pStr);
	AnimationMgr* GetAnimationMgr() const { return m_pAnimationMgr;  }
	//void SetFrameInit(size_t motionMaxNum, size_t frameSpeed);

private:
	BLENDFUNCTION mBlendInfo;
	AnimationMgr* m_pAnimationMgr = nullptr;

	const TCHAR* m_pImageLocation = nullptr;
	size_t mMotionMaxNum = 0;
	size_t mFrameSpeed = 0;
	TCHAR mBmpKey[64] = L"";

private:
	HDC m_hAlphaDC;
	HDC m_hEffectDC;
	bool m_bDynamic = false;
};

