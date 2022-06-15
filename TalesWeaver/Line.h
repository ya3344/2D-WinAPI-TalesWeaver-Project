#pragma once

typedef struct tagLineInfo
{
	tagLineInfo()
	{
		ZeroMemory(this, sizeof(tagLineInfo));
	};
	explicit tagLineInfo(float startX, float startY, float endX, float endY)
	{
		this->startX = startX;
		this->startY = startY;
		this->endX = endX;
		this->endY = endY;
	}

	float startX = 0.f;
	float startY = 0.f;
	float endX = 0.f;
	float endY = 0.f;

}LINE_INFO;


class Line
{
public:
	Line();
	explicit Line(LINE_INFO& lineInfo);
	~Line();

public:
	const LINE_INFO& GetLineInfo() { return mInfo; }

public:
	void Render(HDC hdc);
private:
	LINE_INFO mInfo;
};

