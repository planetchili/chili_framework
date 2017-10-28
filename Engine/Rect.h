#pragma once

#include "Vec2.h"

class Rect
{
public:
	Rect() = default;
	Rect( float left_in,float right_in,float top_in,float bottom_in );
	Rect( const Vec2& topLeft,const Vec2& bottomRight );
	Rect( const Vec2& topLeft,float width,float height );
	bool IsOverlappingWith( const Rect& other ) const;
	bool IsContainedBy( const Rect& other ) const;
	static Rect FromCenter( const Vec2& center,float halfWidth,float halfHeight );
	Rect GetExpanded( float offset ) const;
	Vec2 GetCenter() const;
public:
	float left;
	float right;
	float top;
	float bottom;
};