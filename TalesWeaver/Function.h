#pragma once

template <typename T>
inline void SafeDelete(T& p)
{
	if (p)
	{
		delete p;
		p = nullptr;
	}
}

inline float ToRadian(float degree)
{
	return ((degree) * (PI / 180.0f));
}

inline float ToDegree(float radian)
{
	return ((radian) * ( 180.0f / PI));
}