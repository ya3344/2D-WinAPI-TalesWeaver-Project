#pragma once

#define DECLARE_SINGLETON(ClassName)		\
public:										\
	static ClassName* GetInstance()			\
	{										\
		if (m_pInstance == nullptr)			\
			m_pInstance = new ClassName;	\
											\
		return m_pInstance;					\
	}										\
											\
	void DestroyInstance()					\
	{										\
		if (m_pInstance)					\
		{									\
			delete m_pInstance;				\
			m_pInstance = nullptr;			\
		}									\
	}										\
private:									\
	static ClassName* m_pInstance;				


#define INIT_SINGLETON_VARIABLE(ClassName)	\
ClassName* ClassName::m_pInstance = nullptr; 


#define MESSAGE_BOX(ErrorName)						\
MessageBox(g_hWnd, ErrorName, nullptr, MB_OK);		\
DestroyWindow(g_hWnd);								\
exit(0);
																
