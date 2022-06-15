#pragma once

class Effect;

class EffectMgr
{
	DECLARE_SINGLETON(EffectMgr);

private:
	EffectMgr();
	~EffectMgr();

public:
	void Initialize();
	void Update();
	void LateUpdate();
	void Render(HDC hDC);
	void Release();

public:
	Image* GetFindEffect(wstring findKey, bool bDynamic);
	Image* GetFindAllEffect(wstring imageKey);
	const unordered_map<wstring, Image*>& GetEffectSpace(int classify) const { return mEffectSpace[classify]; }
	
public:
	void AddEffect(const TCHAR* pBmpKey, const TCHAR* pImageLocation, size_t motionMaxNum, size_t frameSpeed, IMAGE_TYPE imageType, bool bDynamic);

public:
	void DeleteDynamicEffectSpace();

private:
	unordered_map<wstring, Image*> mEffectSpace[CLASSIFY_END];

	const TCHAR* m_pStageCurrentKey = nullptr;
	const TCHAR* m_pStagePreKey = nullptr;

};

