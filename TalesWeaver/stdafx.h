// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <list>
#include <algorithm>
#include <time.h>
#include <vector>
#include <Map>
#include <unordered_map>
#include <cmath>
//#include <vld.h>
#include <iostream>
#include <stack>
#include <io.h>
using namespace std;

// FMOD 헤더와 라이브러리 추가
#include "fmod.h"
#include "fmod.hpp"

#pragma comment(lib, "fmodex_vc.lib")

// Console Command
#pragma comment(linker,"/entry:wWinMainCRTStartup /subsystem:console")

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
// Header
#include "Constant.h"
#include "Struct.h"
#include "Function.h"
#include "Extern.h"
#include "Macro.h"
#include "Typedef.h"
#include "ObjFactory.h"
#include "ImageFactory.h"

// Manager
#include "CollisionMgr.h"
#include "ObjMgr.h"
#include "MathMgr.h"
#include "LIneMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "SceneMgr.h"
#include "ImageMgr.h"
#include "TileMgr.h"
#include "StringMgr.h"
#include "EffectMgr.h"
#include "SoundMgr.h"


