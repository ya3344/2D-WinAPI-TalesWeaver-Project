// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
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

// FMOD ����� ���̺귯�� �߰�
#include "fmod.h"
#include "fmod.hpp"

#pragma comment(lib, "fmodex_vc.lib")

// Console Command
#pragma comment(linker,"/entry:wWinMainCRTStartup /subsystem:console")

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
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


