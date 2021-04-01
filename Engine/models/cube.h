#pragma once
#include <vector>
#include "effects/textureEffect.h"

class cube
{
public:
	cube(float side = 0.5f)
	{
		textureEffect::vertex vertex;

		vertex.m_position = Vec3(side, side, side);
		vertex.m_uv_coordinates = Vec2(0.0f, 0.5f);
		m_vertexBuffer.emplace_back(vertex); //0

		vertex.m_position = Vec3(-side, side, side);
		vertex.m_uv_coordinates = Vec2(0.25f, 0.5f);
		m_vertexBuffer.emplace_back(vertex); //1
		
		vertex.m_position = Vec3(-side, side, -side);
		vertex.m_uv_coordinates = Vec2(0.25f, 0.75f);
		m_vertexBuffer.emplace_back(vertex); //2

		vertex.m_position = Vec3(side, side, -side);
		vertex.m_uv_coordinates = Vec2(0.0f, 0.75f);
		m_vertexBuffer.emplace_back(vertex); //3
		
		vertex.m_position = Vec3(-side, -side, side);
		vertex.m_uv_coordinates = Vec2(0.5f, 0.5f);
		m_vertexBuffer.emplace_back(vertex); //4
		
		vertex.m_position = Vec3(-side, -side, -side);
		vertex.m_uv_coordinates = Vec2(0.5f, 0.75f);
		m_vertexBuffer.emplace_back(vertex); //5
		
		vertex.m_position = Vec3(-side, side, side);
		vertex.m_uv_coordinates = Vec2(0.5f, 0.25f);
		m_vertexBuffer.emplace_back(vertex); //6
		
		vertex.m_position = Vec3(side, side, side);
		vertex.m_uv_coordinates = Vec2(0.75f, 0.25f);
		m_vertexBuffer.emplace_back(vertex); //7

		vertex.m_position = Vec3(side, -side, side);
		vertex.m_uv_coordinates = Vec2(0.75f, 0.5f);
		m_vertexBuffer.emplace_back(vertex); //8
		
		vertex.m_position = Vec3(side, -side, -side);
		vertex.m_uv_coordinates = Vec2(0.75f, 0.75f);
		m_vertexBuffer.emplace_back(vertex); //9

		vertex.m_position = Vec3(side, side, -side);
		vertex.m_uv_coordinates = Vec2(0.75f, 1.0f);
		m_vertexBuffer.emplace_back(vertex); //10

		vertex.m_position = Vec3(-side, side, -side);
		vertex.m_uv_coordinates = Vec2(0.5f, 1.0f);
		m_vertexBuffer.emplace_back(vertex); //11

		vertex.m_position = Vec3(side, side, -side);
		vertex.m_uv_coordinates = Vec2(1.0f, 0.75f);
		m_vertexBuffer.emplace_back(vertex); //12

		vertex.m_position = Vec3(side, side, side);
		vertex.m_uv_coordinates = Vec2(1.0f, 0.5f);
		m_vertexBuffer.emplace_back(vertex); //13

		setTriangleIndexBuffer();
	}

	inline std::vector<uint32_t> getIndexBuffer	() { return m_indexBuffer; }
	inline std::vector<textureEffect::vertex> getVertexBuffer() { return m_vertexBuffer; }

private:
	
	void setTriangleIndexBuffer()
	{
		m_indexBuffer = std::vector<std::uint32_t>{
				1,0,3	, 3,2,1, 
				4,1,2	, 2,5,4,
				8,4,5	, 5,9,8,
				11,10,9 , 9,5,11,
				12,13,8 , 8,9,12,
				8,7,6	, 6,4,8
		};
	}

private:
	std::vector<textureEffect::vertex> m_vertexBuffer;
	std::vector<uint32_t> m_indexBuffer;
};
