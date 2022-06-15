#pragma once
class Scene;

class SceneMgr
{
	DECLARE_SINGLETON(SceneMgr);
private:
	SceneMgr();
	~SceneMgr();

public:
	bool ChangeScene(SCENE_ID id);
	void LateInit();
	void Update();
	void LateUpdate();
	void Render(HDC hDC);
	void Release();

private:
	Scene* m_pScene = nullptr;
	SCENE_ID mCurScene = SCENE_ID::END;
	SCENE_ID mPreScene = SCENE_ID::END;
};

