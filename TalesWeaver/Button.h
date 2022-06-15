#pragma once
class Button : public Image
{
public:
	Button();
	virtual ~Button();

	// Image��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	HDC hButtonDC;
	MyBmp* m_pButtonBmp = nullptr;
	float mDynamicWidth = 0.f;
	wstring mBmpKey;
};

