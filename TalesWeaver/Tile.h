#pragma once


class Tile final: public Image
{
public:
	Tile();
	explicit Tile(const INFO& info);
	~Tile();

	// Image을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	BLENDFUNCTION mBlendInfo;
	HDC m_hTileDC;
	HDC m_hAlphaDC;
};

