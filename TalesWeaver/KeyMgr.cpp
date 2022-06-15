#include "stdafx.h"
#include "KeyMgr.h"

INIT_SINGLETON_VARIABLE(KeyMgr);

KeyMgr::KeyMgr()
{
	ZeroMemory(m_bKey, sizeof(m_bKey));
}


KeyMgr::~KeyMgr()
{
}
