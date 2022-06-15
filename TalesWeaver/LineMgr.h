#pragma once

class Line;


class LineMgr
{
	DECLARE_SINGLETON(LineMgr);

private:
	LineMgr();
	~LineMgr();

public:
	void Initialize();
	void Render(HDC hdc);
	void Release();

public:
	bool LinearCollision(float targetX, float* pOutY);
	const vector<Line*>& GetLineSpace() { return mLineSpace; }

private:
	void LoadData();

private:
	vector<Line*> mLineSpace;
	int mLineMaxNum = 0;
};

