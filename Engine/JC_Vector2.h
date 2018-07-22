

#pragma once

#include <cmath>
#include <type_traits>

template<class T> struct JC_Point2
{
	JC_Point2() {};
	JC_Point2(T xin, T yin)
		:
		x(xin),
		y(yin)
	{}
	
	// point to point conversion equals operator
	template<class U>
	JC_Point2& operator=(const JC_Point2<U>& other)
	{
		x = T(other.x);
		y = T(other.y);
		return *this;
	}
		
	template<class U>explicit JC_Point2(const JC_Point2<U>& other)
		:
		x(T(other.x)), y(T(other.y))
	{}

	T x, y;
};
template<class T> struct JC_Vector2
{
	JC_Vector2() {};
	JC_Vector2( T xin, T yin)
		:
		x(xin),
		y(yin)
	{}
	
	// vector to vector conversion equals operator
	template<class U>
	JC_Vector2& operator=(const JC_Vector2<U>& other)
	{
		x = T(other.x);
		y = T(other.y);
		return *this;
	}


	//template<class U>explicit JC_Vector2(const JC_Vector2<U>& other)
	//	:
	//	x(T(other.x)), y(T(other.y))
	//{}


	T x, y;
};

using JC_Point2d = JC_Point2<double>;
using JC_Point2f = JC_Point2<float>;
using JC_Point2i = JC_Point2<int>;

using JC_Vector2d = JC_Vector2<double>;
using JC_Vector2f = JC_Vector2<float>;
using JC_Vector2i = JC_Vector2<int>;



template<class T> bool operator== (const JC_Point2<T>& lhs, const JC_Point2<T>& rhs)
{
	return ((lhs.x == rhs.x) && (lhs.y == rhs.y));
}
template<class T> bool operator!= (const JC_Point2<T>& lhs, const JC_Point2<T>& rhs)
{
	return ((lhs.x != rhs.x) || (lhs.y != rhs.y));
}



template<class T> JC_Vector2<T>& operator+=(JC_Vector2<T>& lhs, const JC_Vector2<T>& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;

	return lhs;
}
template<class T> JC_Point2<T>& operator+=(JC_Point2<T>& lhs, const JC_Vector2<T>& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;

	return lhs;
}
template<class T> JC_Point2<T>& operator+=(JC_Point2<T>& lhs, const JC_Point2<T>& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;

	return lhs;
}


template<class T> JC_Vector2<T>& operator-=(JC_Vector2<T>& lhs, const JC_Vector2<T>& rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;

	return lhs;
}
template<class T> JC_Point2<T>& operator-=(JC_Point2<T>& lhs, const JC_Vector2<T>& rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;

	return lhs;
}
template<class T> JC_Point2<T>& operator-=(JC_Point2<T>& lhs, const JC_Point2<T>& rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;

	return lhs;
}

template<class T> JC_Vector2<T>& operator*=(JC_Vector2<T>& v, T s)
{
	v.x *= s;
	v.y *= s;
	return v;
}
template<class T> JC_Vector2<T>& operator/=(JC_Vector2<T>& v, T s)
{
	v.x /= s;
	v.y /= s;
	return v;
}
template<class T> JC_Vector2<T> operator+(const JC_Vector2<T>& lhs, const JC_Vector2<T>& rhs)
{
	return JC_Vector2<T>(lhs) += rhs;
}
template<class T> JC_Point2<T> operator+(const JC_Point2<T>& lhs, const JC_Vector2<T>& rhs)
{
	return { lhs.x + rhs.x, lhs.y + rhs.y };

}
template<class T> T operator-(const T& lhs)
{
	return T(-lhs.x, -lhs.y);
}

template<class T> JC_Vector2<T> operator-(const JC_Vector2<T>& lhs, const JC_Vector2<T>& rhs)
{
	return JC_Vector2<T>(lhs) -= rhs;
}
template<class T> JC_Point2<T> operator-(const JC_Point2<T>& lhs, const JC_Vector2<T>& rhs)
{
	return JC_Point2<T>(lhs) -= rhs;
}
template<class T> JC_Vector2<T> operator-(const JC_Point2<T>& lhs, const JC_Point2<T>& rhs)
{
	return { lhs.x - rhs.x, lhs.y - rhs.y };
}
template<class T> JC_Vector2<T> operator*(const JC_Vector2<T>& v, T s)
{
	return { v.x * s, v.y * s };
}
template<class T> JC_Vector2<T> operator*(T s, const JC_Vector2<T>& v)
{
	return JC_Vector2<T>(v) *= s;
}
template<class T> JC_Vector2<T> operator/(const JC_Vector2<T>& v, T s)
{
	return { v.x / s,v.y / s };
}


template <class T> JC_Point2<T> operator*=(JC_Point2<T> lhs, T rhs)
{
	lhs.x *= rhs;
	lhs.y *= rhs;
	return lhs;
}
template <class T> JC_Point2<T> operator/=(JC_Point2<T> lhs, T rhs)
{
	lhs.x /= rhs;
	lhs.y /= rhs;
	return lhs;
}
template <class T> JC_Point2<T> operator*=(T lhs, JC_Point2<T> rhs)
{
	rhs.x *= lhs;
	rhs.y *= lhs;
	return rhs;
}
template <class T> JC_Point2<T> operator/=(T lhs , JC_Point2<T> rhs)
{
	rhs.x /= lhs;
	rhs.y /= lhs;
	return rhs;
}

template<class T> T dot_product(const JC_Vector2<T>& v0, const JC_Vector2<T>& v1)
{
	return (v0.x * v1.x) + (v0.y + v1.y);
}
template<class T> T length_sq(const JC_Vector2<T>& v)
{
	return dot_product(v, v);
}
template<class T> T length(const JC_Vector2<T>& v)
{
	return T(std::sqrt(double(length_sq(v))));
}
template<class T> JC_Vector2<T> normalize(const JC_Vector2<T>& v)
{
	const auto len = length(v);
	if (std::is_floating_point_v<T>)
	{
		return (len == T(0)) ? v : v * (T(1) / len);
	}

	return (len == 0) ? v : v / len;
}



template<class T> T GetDistanceToSq(const JC_Point2<T>& v0, const JC_Point2<T>& v1)
{
	return ((v1.x - v0.x)*(v1.x - v0.x)+ (v1.y - v0.y)*(v1.y - v0.y));
}
template<class T> T GetDistanceTo(const JC_Point2<T>& v0, const JC_Point2<T>& v1)
{
	return std::sqrt(GetDistanceToSq(v0, v1));
}
template<class T> JC_Point2<T> GetMidPoint(const JC_Point2<T>& v0, const JC_Point2<T>& v1)
{
	auto v = v1 - v0;
	v = (std::is_floating_point_v<T>) ?
		v * T(.5) :
		v / T(2);

	return v0 + v;
}

