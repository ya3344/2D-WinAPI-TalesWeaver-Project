#pragma once
//class Tile;

class TileMgr
{
	DECLARE_SINGLETON(TileMgr)

private:
	TileMgr();
	~TileMgr();

public:
	void Initialize();
	void LateUpdate();
	void Render(HDC hdc);
	void Release();
	
public:
	void LoadData(const TCHAR* pFilePath);

public:
	const vector<Image*>& GetTileSpace() const { return mTileSpace; }
	Image* GetTile(const size_t index) const { return mTileSpace[index]; }
	const vector<INFO>& GetEnemyIndex() const { return mEnemyIndex; }

private:
	vector<Image*> mTileSpace;
	bool m_bDraw = false;
	vector<INFO> mEnemyIndex;
};


