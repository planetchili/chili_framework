#pragma once

#include <math.h>
#include <assert.h>

template <typename T>
class JC_Vector2
{
public:
	T x;
	T y;



public:
	inline JC_Vector2(){}
	inline JC_Vector2(T inx, T iny)
		:
		x(inx),
		y(iny)
	{}

	template<typename C>
	JC_Vector2(const JC_Vector2<C>& src)
		:
		x((T)src.x),
		y((T)src.y)
	{}
	

	template<typename T2>
	explicit operator JC_Vector2<T2>() const
	{
		return { T2 x,T2 y };
	}


	template<class PointType>
	inline JC_Vector2(const PointType& P , const PointType& Q)
		:
		JC_Vector2(P.x-Q.x , P.y-Q.y)
	{}
	


	/*Vector Vector Operators*/

	inline JC_Vector2 operator-()
	{
		return JC_Vector2(-x, -y);
	}
	/*
	inline JC_Vector2 operator= (const JC_Vector2 &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}
	*/
	
	inline JC_Vector2 operator-(const JC_Vector2 &rhs)const
	{
		return JC_Vector2(x - rhs.x , y - rhs.y);
	}
	inline JC_Vector2& operator-=(const JC_Vector2 &rhs)
	{
		return *this = *this - rhs;

	}

	inline JC_Vector2 operator+(const JC_Vector2 &rhs)const
	{
		return JC_Vector2(x + rhs.x, y + rhs.y);
	}
	inline JC_Vector2& operator+=(const JC_Vector2 &rhs)
	{
		return *this = *this + rhs;
	}

	/*Vector scalar operators*/

	inline JC_Vector2 operator*(const T rhs) const
	{
		return JC_Vector2(x * rhs, y * rhs);
	}
	inline JC_Vector2& operator*=(const T rhs)
	{
		return *this = *this * rhs;
	}

	inline JC_Vector2 operator/(const T rhs) const
	{
		return JC_Vector2(x / rhs, y / rhs);
	}
	inline JC_Vector2& operator/=(const T rhs)
	{
		return *this = *this / rhs;
	}

	/*Lenght operations*/

	inline T GetLengthSq() const
	{
		return x * x + y * y;
	}
	inline T GetLength() const
	{
		return (T)std::sqrt(GetLengthSq());
	}

	/*Normalise operations*/

	inline JC_Vector2& Normalize()
	{
		return *this = GetNormalize();
	}
	inline JC_Vector2 GetNormalize() const
	{
		const double len = GetLength();
		if (len != (T)0)
		{
			return *this * ((T)1 / len);
		}
		return len;
	}

	/*Boolean operators*/

	inline bool operator==(const JC_Vector2 &rhs)const
	{
		return (x == rhs.x && y== rhs.y) ;
	}
	
	inline bool operator!=(const JC_Vector2 &rhs)const
	{
		return (x != rhs.x || y != rhs.y);
	}

	inline double Vector_Slope(const JC_Vector2& V)
	{

		assert(V.x != 0.0f);
		float m = V.y / V.x;
		
		return m;

	}

};

typedef JC_Vector2<double> JC_Vector2d;
typedef JC_Vector2<float> JC_Vector2f;
typedef JC_Vector2<int> JC_Vector2i;


template <typename T>
class JC_Point2
{
public:
	T x;
	T y;

public:
	inline JC_Point2() {}
	inline JC_Point2(T inx, T iny)
		:
		x(inx),
		y(iny)
	{}
	inline JC_Point2(const JC_Point2& point)
		:
		JC_Point2(point.x, point.y)
	{}

	template<typename C>
	JC_Point2(const JC_Point2<C>& src)
		:
		x((T)src.x),
		y((T)src.y)
	{}

	template<typename T2>
	explicit operator JC_Point2<T2>() const
	{
		return { T2 x,T2 y };

	}




	inline JC_Point2 operator-()
	{
		return JC_Point2(-x, -y);
	}
	inline JC_Point2 operator= (const JC_Point2 &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}

	inline JC_Point2 operator+(const JC_Point2 &rhs) const
	{
		return JC_Point2(x + rhs.x, y + rhs.y);
	}
	inline JC_Point2& operator+=(const JC_Point2 &rhs)
	{
		return *this = *this + rhs;

	}


	inline JC_Point2 operator- (const JC_Point2 &rhs) const
	{
		return JC_Point2(x - rhs.x, y - rhs.y)
	}
	inline JC_Point2& operator-=(const JC_Point2 &rhs)
	{
		return *this = *this - rhs;

	}

	inline JC_Point2 operator*(const T rhs) const
	{
		return JC_Point2(x * rhs, y * rhs);
	}
	inline JC_Point2& operator*=(const T rhs)
	{
		return *this = *this * rhs;
	}

	inline JC_Point2 operator/(const T rhs) const
	{
		return JC_Point2(x / rhs, y / rhs);
	}
	inline JC_Point2& operator/=(const T rhs)
	{
		return *this = *this / rhs;
	}
	


	inline JC_Point2 operator+(const JC_Vector2<T> &rhs) const
	{
		return JC_Point2(x + rhs.x, y + rhs.y);
	}
	inline JC_Point2& operator+=(const JC_Vector2<T> &rhs)
	{
		return *this = *this + rhs;

	}


	inline JC_Point2 operator- (const JC_Vector2<T> &rhs) const
	{
		return JC_Point2(x - rhs.x, y - rhs.y)
	}
	inline JC_Point2& operator-=(const JC_Vector2<T> &rhs)
	{
		return *this = *this - rhs;

	}




	bool operator==(const JC_Point2 &rhs) const
	{
		return (x == rhs.x && y == rhs.y) ? true : false;
	}

	bool operator!=(const JC_Point2 &rhs) const
	{
		return (x != rhs.x || y != rhs.y);
	}

	bool operator<=(const JC_Point2 &rhs) const
	{
		// dictionary order
		return ((x < rhs.x) ? true : ((x == rhs.x && y <= rhs.y) ? true : false));
	}

	bool operator>=(const JC_Point2 &rhs) const
	{
		// dictionary order
		return ((x > rhs.x) ? true : ((x == rhs.x && y >= rhs.y) ? true : false));
	}

	bool operator<(const JC_Point2 &rhs) const
	{
		// dictionary order
		return ((x < rhs.x) ? true : ((x == rhs.x && y < rhs.y) ? true : false));
	}

	bool operator>(const JC_Point2 &rhs) const
	{
		// dictionary order
		return ((x > rhs.x) ? true : ((x == rhs.x && y > rhs.y) ? true : false));
	}


	inline T GetDistanceToSq( const JC_Point2 &rhs) const
	{
		return (T)((this->x-rhs.x) * (this->x - rhs.x) + (this->y - rhs.y) * (this->y - rhs.y));
	}

	inline T GetDistanceTo(const JC_Point2 &rhs) const
	{
		return (T)std::sqrt(GetDistanceToSq(rhs));
	}

	inline JC_Point2 GetMidPoint(const JC_Point2& rhs) const
	{
		return JC_Point2((*this+rhs)/2);
	}

	inline void Zero()
	{
		x = y = 0.0;
	}

	inline bool IsZero() const
	{
		return (x == 0.0 && y == 0.0);
	}

	inline bool IsNotZero() const
	{
		// the && (x == x && y == y) insures no coordinate is a Nan.
		return (x != 0.0 || y != 0.0) && (x == x && y == y);
	}


};

typedef JC_Point2<double> JC_Point2d;
typedef JC_Point2<float> JC_Point2f;
typedef JC_Point2<int> JC_Point2i;
