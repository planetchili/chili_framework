#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "dataStructure/math/Vec2.h"
#include "dataStructure/math/Vec3.h"
#include "dataStructure/math/Mat3.h"

class textureEffect
{
public:
	class vertex
	{
	public:
		vertex() = default;
		vertex(Vec3 position, Vec2 uv_coordinates)
			:
			m_position(position),
			m_uv_coordinates(uv_coordinates)
		{}

	public:
		vertex interpolateTo(const vertex& rhs, float alpha) const
		{
			vertex v;
			v.m_position = (m_position * (1 - alpha)) + (rhs.m_position * (alpha));
			v.m_uv_coordinates = (m_uv_coordinates * (1 - alpha)) + (rhs.m_uv_coordinates * (alpha));

			return v;
		}

		//bool operator ==(const texturedVertex& rhs) { return ((m_position == rhs.m_position) && (m_uv_coordinates == rhs.m_uv_coordinates)); };
		vertex operator-(const vertex& rhs) const
		{
			vertex vert;
			vert.m_position = m_position - rhs.m_position;
			vert.m_uv_coordinates = m_uv_coordinates - rhs.m_uv_coordinates;

			return vert;
		}
		vertex operator+(const vertex& rhs) const
		{
			vertex vert;
			vert.m_position = m_position + rhs.m_position;
			vert.m_uv_coordinates = m_uv_coordinates + rhs.m_uv_coordinates;

			return vert;
		}
		vertex operator*(const Mat3& rhs) const
		{
			vertex vert;
			vert.m_position = m_position * rhs;
			vert.m_uv_coordinates = m_uv_coordinates;
			return vert;
		}


	public:
		Vec3 m_position = Vec3(-99.0f, -99.0f, -99.0f); //this variable with the very same name is mandatory and the pipeline expects said variable
		Vec2 m_uv_coordinates = Vec2(-100.0f, -100.0f); 
	};
};