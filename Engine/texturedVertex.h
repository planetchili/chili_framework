#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "Vec2.h"
#include "Vec3.h"

class texturedVertex
{
public:
	texturedVertex() = default;
	texturedVertex(Vec3 position, Vec2 uv_coordinates);
	
public:
	texturedVertex interpolateTo(const texturedVertex& rhs, float alpha);

public:
	Vec3 m_position;
	Vec2 m_uv_coordinates;
};