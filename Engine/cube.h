#pragma once
#include <vector>
#include "Vec3.h"

class cube
{
public:
	cube(float size = 0.5f)
	{
		vertexBuffer.emplace_back(Vec3{ -size,-size,-0.5f });
		vertexBuffer.emplace_back(Vec3{ -size, size,-0.5f });
		vertexBuffer.emplace_back(Vec3{ -size, size,0.5f });
		vertexBuffer.emplace_back(Vec3{  size, size,0.5f });
		vertexBuffer.emplace_back(Vec3{  size,-size,0.5f });
		vertexBuffer.emplace_back(Vec3{  size,-size,-0.5f });
		vertexBuffer.emplace_back(Vec3{ -size,-size,0.5f });
		vertexBuffer.emplace_back(Vec3{  size, size,-0.5f });

		setLineIndexBuffer();
		setTriangleIndexBuffer();
	}

	inline std::vector<uint32_t> getLineIndexBuffer		() { return lineIndexBuffer;	 }
	inline std::vector<uint32_t> getTriangleIndexBuffer	() { return triangleIndexBuffer; }
	inline std::vector<Vec3>&     getVertexBuffer() { return vertexBuffer; }

private:
	void setLineIndexBuffer()
	{
		lineIndexBuffer.emplace_back(0);
		lineIndexBuffer.emplace_back(1);

		lineIndexBuffer.emplace_back(1);
		lineIndexBuffer.emplace_back(2);

		lineIndexBuffer.emplace_back(2);
		lineIndexBuffer.emplace_back(3);

		lineIndexBuffer.emplace_back(3);
		lineIndexBuffer.emplace_back(4);

		lineIndexBuffer.emplace_back(4);
		lineIndexBuffer.emplace_back(5);

		lineIndexBuffer.emplace_back(5);
		lineIndexBuffer.emplace_back(0);

		lineIndexBuffer.emplace_back(0);
		lineIndexBuffer.emplace_back(6);

		lineIndexBuffer.emplace_back(6);
		lineIndexBuffer.emplace_back(4);

		lineIndexBuffer.emplace_back(1);
		lineIndexBuffer.emplace_back(7);

		lineIndexBuffer.emplace_back(7);
		lineIndexBuffer.emplace_back(5);

		lineIndexBuffer.emplace_back(7);
		lineIndexBuffer.emplace_back(3);

		lineIndexBuffer.emplace_back(2);
		lineIndexBuffer.emplace_back(6);
	}
	void setTriangleIndexBuffer()
	{
		triangleIndexBuffer.emplace_back(0);
		triangleIndexBuffer.emplace_back(1);
		triangleIndexBuffer.emplace_back(5);

		triangleIndexBuffer.emplace_back(1);
		triangleIndexBuffer.emplace_back(7);
		triangleIndexBuffer.emplace_back(5);


		triangleIndexBuffer.emplace_back(1);
		triangleIndexBuffer.emplace_back(2);
		triangleIndexBuffer.emplace_back(7);

		triangleIndexBuffer.emplace_back(7);
		triangleIndexBuffer.emplace_back(2);
		triangleIndexBuffer.emplace_back(3);

		triangleIndexBuffer.emplace_back(2);
		triangleIndexBuffer.emplace_back(6);
		triangleIndexBuffer.emplace_back(4);

		triangleIndexBuffer.emplace_back(2);
		triangleIndexBuffer.emplace_back(3);
		triangleIndexBuffer.emplace_back(4);

		triangleIndexBuffer.emplace_back(0);
		triangleIndexBuffer.emplace_back(6);
		triangleIndexBuffer.emplace_back(5);

		triangleIndexBuffer.emplace_back(4);
		triangleIndexBuffer.emplace_back(5);
		triangleIndexBuffer.emplace_back(6);

		triangleIndexBuffer.emplace_back(0);
		triangleIndexBuffer.emplace_back(1);
		triangleIndexBuffer.emplace_back(2);

		triangleIndexBuffer.emplace_back(2);
		triangleIndexBuffer.emplace_back(6);
		triangleIndexBuffer.emplace_back(0);

		triangleIndexBuffer.emplace_back(7);
		triangleIndexBuffer.emplace_back(5);
		triangleIndexBuffer.emplace_back(4);

		triangleIndexBuffer.emplace_back(3);
		triangleIndexBuffer.emplace_back(4);
		triangleIndexBuffer.emplace_back(7);
	}

private:
	std::vector<Vec3> vertexBuffer;

	std::vector<std::uint32_t> lineIndexBuffer;
	std::vector<uint32_t> triangleIndexBuffer;
};
