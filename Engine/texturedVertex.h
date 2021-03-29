#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "Vec2.h"
#include "Vec3.h"
#include "Mat3.h"

#include "pubeToScreenTransformer.h"

class texturedVertex
{
public:
	texturedVertex() = default;
	texturedVertex(Vec2 position, Vec2 uv_Coordinates);
	texturedVertex(Vec3 position, Vec2 uv_coordinates);
	
public:
	texturedVertex interpolateTo(const texturedVertex& rhs, float alpha) const;
	void transformToScreenSpace(int screenWidth = 640U,int screenHeight = 640U);

	//bool operator ==(const texturedVertex& rhs) { return ((m_position == rhs.m_position) && (m_uv_coordinates == rhs.m_uv_coordinates)); };
	texturedVertex operator-(const texturedVertex& rhs) const;
	texturedVertex operator+(const texturedVertex& rhs) const;
	texturedVertex operator*(const Mat3& rhs) const;
	

public:
	Vec3 m_position = Vec3(-99.0f,-99.0f,-99.0f);
	Vec2 m_uv_coordinates = Vec2(-100.0f,-100.0f); //this is generally expected to be in the 0.0f to 1.0f range..

	bool m_inScreenSpace = false;
};