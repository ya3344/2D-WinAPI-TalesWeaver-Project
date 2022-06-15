#pragma once
class ScrollMgr
{
	DECLARE_SINGLETON(ScrollMgr);

private:
	ScrollMgr();
	~ScrollMgr();

public:
	float GetScrollX() const { return mScrollX;  }
	float GetScrollY() const { return mScrollY; }

public:
	void Update();

private:
	float mScrollX = 0.f;
	float mScrollY = 0.f;

private:
	static constexpr int SCROLL_RANGE = 300;
};

