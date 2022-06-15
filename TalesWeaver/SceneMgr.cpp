#include "stdafx.h"
#include "SceneMgr.h"
#include "LogoScene.h"
#include "MenuScene.h"
#include "VillageScene.h"
#include "FieldScene.h"
#include "DungeonScene.h"
#include "BossScene.h"

INIT_SINGLETON_VARIABLE(SceneMgr);

SceneMgr::SceneMgr()
{
}


SceneMgr::~SceneMgr()
{
	Release();
}

bool SceneMgr::ChangeScene(SCENE_ID id)
{
	mCurScene = id;
	if (mCurScene != mPreScene)
	{
		BmpMgr::GetInstance()->DeleteDynamicBmpSpace();
		EffectMgr::GetInstance()->DeleteDynamicEffectSpace();
		switch (id)
		{
		case SCENE_ID::LOGO:
			SafeDelete<Scene*>(m_pScene);
			m_pScene = new LogoScene;
			break;
		case SCENE_ID::MENU:
			SafeDelete<Scene*>(m_pScene);
			m_pScene = new MenuScene;
			break;
		case SCENE_ID::VILLAGE:
			//m_pScene = new LogoScene;
			//m_pScene->Initialize();
			//SafeDelete<Scene*>(m_pScene);

			//m_pScene = new MenuScene;
			//m_pScene->Initialize();
			SafeDelete<Scene*>(m_pScene);
			m_pScene = new VillageScene;
			break;
		case SCENE_ID::FIELD:
	/*		m_pScene = new LogoScene;
			m_pScene->Initialize();
			SafeDelete<Scene*>(m_pScene);

			m_pScene = new MenuScene;
			m_pScene->Initialize();
			SafeDelete<Scene*>(m_pScene);

			m_pScene = new VillageScene;
			m_pScene->Initialize();
			m_pScene->LateInit();*/
			SafeDelete<Scene*>(m_pScene);

			m_pScene = new FieldScene;
			break;
		case SCENE_ID::DUNGEON:
	/*		m_pScene = new VillageScene;
			m_pScene->Initialize();
			SafeDelete<Scene*>(m_pScene);

			m_pScene = new FieldScene;
			m_pScene->Initialize();*/
			SafeDelete<Scene*>(m_pScene);

			m_pScene = new DungeonScene;
			break;
		case SCENE_ID::BOSS:
			//m_pScene = new LogoScene;
			//m_pScene->Initialize();
			//SafeDelete<Scene*>(m_pScene);

			//m_pScene = new MenuScene;
			//m_pScene->Initialize();
			//SafeDelete<Scene*>(m_pScene);

			//m_pScene = new VillageScene;
			//m_pScene->Initialize();
			//m_pScene->LateInit();
			//SafeDelete<Scene*>(m_pScene);

			//m_pScene = new FieldScene;
			//m_pScene->Initialize();
			//SafeDelete<Scene*>(m_pScene);

			//m_pScene = new DungeonScene;
			//m_pScene->Initialize();
			SafeDelete<Scene*>(m_pScene);

			

			/*BmpMgr::GetInstance()->DeleteDynamicBmpSpace();
			EffectMgr::GetInstance()->DeleteDynamicEffectSpace();*/

			m_pScene = new BossScene;
			break;
		default:
			break;
		}
		if (m_pScene == nullptr)
			return false;

		m_pScene->Initialize();
		m_pScene->LateInit();
		mPreScene = mCurScene;
	}

	return true;

}

void SceneMgr::LateInit()
{
	m_pScene->LateInit();
}

void SceneMgr::Update()
{
	m_pScene->Update();
}

void SceneMgr::LateUpdate()
{
	m_pScene->LateUpdate();
}

void SceneMgr::Render(HDC hDC)
{
	m_pScene->Render(hDC);
}

void SceneMgr::Release()
{
	SafeDelete<Scene*>(m_pScene);
}
