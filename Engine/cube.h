#pragma once
#include <vector>
#include "Vec3.h"

class cube
{
public:
	cube(float size = 0.5f)
	{
		vertexBuffer.emplace_back(Vec3{ -size,-size,-0.5f }); //0
		vertexBuffer.emplace_back(Vec3{ -size, size,-0.5f }); //1
		vertexBuffer.emplace_back(Vec3{  size, size,-0.5f }); //2
		vertexBuffer.emplace_back(Vec3{  size,-size,-0.5f }); //3
		
		vertexBuffer.emplace_back(Vec3{ -size, size, 0.5f }); //4 -> 1
		vertexBuffer.emplace_back(Vec3{ -size,-size, 0.5f }); //5 -> 0
		vertexBuffer.emplace_back(Vec3{  size, size, 0.5f }); //6 -> 2
		vertexBuffer.emplace_back(Vec3{  size,-size, 0.5f }); //7 -> 3

		setLineIndexBuffer();
		setTriangleIndexBuffer();
	}

	inline std::vector<uint32_t> getLineIndexBuffer		() { return lineIndexBuffer;	 }
	inline std::vector<uint32_t> getTriangleIndexBuffer	() { return triangleIndexBuffer; }
	inline std::vector<Vec3>&     getVertexBuffer() { return vertexBuffer; }

private:
	void setLineIndexBuffer()
	{
		//not implemented..
	}


	void setTriangleIndexBuffer()
	{
		//this is not right fix it in earnest..

		triangleIndexBuffer.emplace_back(0);
		triangleIndexBuffer.emplace_back(1);
		triangleIndexBuffer.emplace_back(2);

		triangleIndexBuffer.emplace_back(2);
		triangleIndexBuffer.emplace_back(3);
		triangleIndexBuffer.emplace_back(0);


		triangleIndexBuffer.emplace_back(6);
		triangleIndexBuffer.emplace_back(4);
		triangleIndexBuffer.emplace_back(5);

		triangleIndexBuffer.emplace_back(5);
		triangleIndexBuffer.emplace_back(7);
		triangleIndexBuffer.emplace_back(6);


		triangleIndexBuffer.emplace_back(4);
		triangleIndexBuffer.emplace_back(1);
		triangleIndexBuffer.emplace_back(0);

		triangleIndexBuffer.emplace_back(0);
		triangleIndexBuffer.emplace_back(5);
		triangleIndexBuffer.emplace_back(4);


		triangleIndexBuffer.emplace_back(3);
		triangleIndexBuffer.emplace_back(2);
		triangleIndexBuffer.emplace_back(6);

		triangleIndexBuffer.emplace_back(6);
		triangleIndexBuffer.emplace_back(7);
		triangleIndexBuffer.emplace_back(3);


		triangleIndexBuffer.emplace_back(6);
		triangleIndexBuffer.emplace_back(2);
		triangleIndexBuffer.emplace_back(1);

		triangleIndexBuffer.emplace_back(1);
		triangleIndexBuffer.emplace_back(4);
		triangleIndexBuffer.emplace_back(6);


		triangleIndexBuffer.emplace_back(0);
		triangleIndexBuffer.emplace_back(3);
		triangleIndexBuffer.emplace_back(7);

		triangleIndexBuffer.emplace_back(7);
		triangleIndexBuffer.emplace_back(5);
		triangleIndexBuffer.emplace_back(0);
	}

private:
	std::vector<Vec3> vertexBuffer;

	std::vector<std::uint32_t> lineIndexBuffer;
	std::vector<uint32_t> triangleIndexBuffer;
};
