#include "stdafx.h"
#include "HpBar.h"


HpBar::HpBar()
{

}


HpBar::~HpBar()
{
}


void HpBar::Initialize()
{

}

void HpBar::LateInit()
{
	mInfo.width = m_pTarget->GetInfo().width * 1.3f;
	mInfo.height = m_pTarget->GetInfo().height * 0.25f;
}


void HpBar::Render(HDC hdc)
{
	float width = mInfo.width * (m_pTarget->GetAttriBute().currentHP / m_pTarget->GetAttriBute().maxHP);
	mRect.right = LONG(mRect.left + width);
	Rectangle(hdc, mRect.left, mRect.top, mRect.right, mRect.bottom);
}

int HpBar::Update()
{
	mInfo.X = m_pTarget->GetInfo().X;
	mInfo.Y = m_pTarget->GetInfo().Y - (m_pTarget->GetInfo().height * 0.6f);

	if (m_pTarget->GetAttriBute().bDie)
		mAttribute.bDie = true;

	if(mAttribute.bDie)
		return OBJ_DIE;

	return OBJ_NORMAL;
}

void HpBar::LateUpdate()
{
	UpdateRect();

	if (m_pTarget->GetAttriBute().bDie == true)
		mAttribute.bDie = true;
}

void HpBar::Release()
{

}
