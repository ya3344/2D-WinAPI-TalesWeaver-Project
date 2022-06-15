#include "stdafx.h"
#include "Tile.h"


Tile::Tile()
{
}

Tile::Tile(const INFO& info) 
{
	mInfo = info;
}


Tile::~Tile()
{
	Release();
}

void Tile::Initialize()
{
	MyBmp* pMyTileBmp = BmpMgr::GetInstance()->GetFindBmp(L"Tile", false);
	if (pMyTileBmp == nullptr)
	{
		MESSAGE_BOX(L"TileFind Fail!");
	}


	m_hTileDC = pMyTileBmp->GetMemDC();

	MyBmp* pMyAlphaBmp = BmpMgr::GetInstance()->GetFindBmp(L"Alpha", false);
	if (pMyAlphaBmp == nullptr)
	{
		MESSAGE_BOX(L"AlphaFind Fail!");
	}

	m_hAlphaDC = pMyAlphaBmp->GetMemDC();
}

void Tile::LateUpdate()
{
	UpdateRect();
}

void Tile::Render(HDC hDC)
{
	GdiTransparentBlt(m_hAlphaDC, mRect.left, mRect.top, (int)mInfo.width, (int)mInfo.height,
		m_hTileDC, (int)(mDrawID * mInfo.height), 0,
		(int)mInfo.width, (int)mInfo.height, RGB(255, 255, 255));

	mBlendInfo.BlendOp = AC_SRC_OVER; // ���� �ɼ� : ȥ�տɼ�
	mBlendInfo.BlendFlags = 0; // �÷��� 0ó��
	mBlendInfo.AlphaFormat = 0;
	mBlendInfo.SourceConstantAlpha = 150; // ����ä�ΰ� 0 ~ 255(���� ������)

	GdiAlphaBlend(hDC, mRect.left, mRect.top, (int)mInfo.width, (int)mInfo.height, m_hAlphaDC, mRect.left, mRect.top, (int)mInfo.width, (int)mInfo.height, mBlendInfo);
}

void Tile::Release()
{

}
