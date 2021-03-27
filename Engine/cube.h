#pragma once
#include <vector>
#include "texturedVertex.h"

class cube
{
public:
	cube(float side = 0.5f)
	{
		texturedVertex vertex;

		vertex.m_position = Vec3(-side, -side, -side);
		vertex.m_uv_coordinates = Vec2(0.0f, 1.0f);
		vertexBuffer.emplace_back(vertex); //0

		vertex.m_position = Vec3(side, -side, -side);
		vertex.m_uv_coordinates = Vec2(1.0f, 1.0f);
		vertexBuffer.emplace_back(vertex); //1
		
		vertex.m_position = Vec3(-side, side, -side);
		vertex.m_uv_coordinates = Vec2(0.0f, 0.0f);
		vertexBuffer.emplace_back(vertex); //2

		vertex.m_position = Vec3(side, side, -side);
		vertex.m_uv_coordinates = Vec2(1.0f, 0.0f);
		vertexBuffer.emplace_back(vertex); //3
		
		vertex.m_position = Vec3(-side, -side, side);
		vertex.m_uv_coordinates = Vec2(1.0f, 1.0f);
		vertexBuffer.emplace_back(vertex); //4
		
		vertex.m_position = Vec3(side, -side, side);
		vertex.m_uv_coordinates = Vec2(0.0f, 1.0f);
		vertexBuffer.emplace_back(vertex); //5
		
		vertex.m_position = Vec3(-side, side, side);
		vertex.m_uv_coordinates = Vec2(1.0f, 0.0f);
		vertexBuffer.emplace_back(vertex); //6
		
		vertex.m_position = Vec3(side, side, side);
		vertex.m_uv_coordinates = Vec2(0.0f, 0.0f);
		vertexBuffer.emplace_back(vertex); //7


		setLineIndexBuffer();
		setTriangleIndexBuffer();
	}

	inline std::vector<uint32_t> getLineIndexBuffer		() { return lineIndexBuffer;	 }
	inline std::vector<uint32_t> getTriangleIndexBuffer	() { return triangleIndexBuffer; }

	inline std::vector<texturedVertex>&     getVertexBuffer() { return vertexBuffer; }

private:
	void setLineIndexBuffer()
	{
		lineIndexBuffer = std::vector<std::uint32_t>{
			0,1,  1,3,  3,2,  2,0,
			0,4,  1,5,	3,7,  2,6,
			4,5,  5,7,	7,6,  6,4 };
	}
	void setTriangleIndexBuffer()
	{
		triangleIndexBuffer = std::vector<std::uint32_t>{
				0, 2, 1, 2, 3, 1,
				1, 3, 5, 3, 7, 5,
				2, 6, 3, 3, 6, 7,
				4, 5, 7, 4, 7, 6,
				0, 4, 2, 2, 4, 6,
				0, 1, 4, 1, 5, 4 };
	}

private:
	std::vector<texturedVertex> vertexBuffer;

	std::vector<std::uint32_t> lineIndexBuffer;
	std::vector<uint32_t> triangleIndexBuffer;
};
