#pragma once
#include<utility>
#include<algorithm>
#include<memory>

#include<string>
#include<filesystem>

#include "dataStructure/math/Vec2.h"
#include "dataStructure/math/Vec3.h"
#include "dataStructure/vertex/texturedVertex.h"

#include "graphics/Graphics.h"
#include "pubeToScreenTransformer.h"

#include "graphics/Surface.h"

class Game;

template<typename effect>
class pipeline
{
public:
	using vertex =typename effect::vertex;

	static void bindTexture(std::filesystem::path texturePath)	{	m_texture = std::make_unique<Surface>(Surface::FromFile(texturePath));	}
	static void bindRotationMatrix(Mat3 rotationMat) { m_rotationMatrix = rotationMat; }
	static void translate(Vec3 translateby) { m_translation = translateby; }

	static void draw(std::vector<uint32_t> indexBuffer, std::vector<vertex> vertexBuffer)
	{
		vertexTransformer(vertexBuffer);
		triangleRasterizer(indexBuffer, vertexBuffer);
	}

private:
	static void vertexTransformer(std::vector<vertex>& vertexBuffer)
	{
		for (auto& vertex : vertexBuffer)
		{
			vertex.m_position = vertex.m_position * m_rotationMatrix;
			vertex.m_position += m_translation;
		}
	}

	static void triangleRasterizer(std::vector<uint32_t> indexBuffer, std::vector<vertex> vertexBuffer)
	{
		for (int i = 0 ; i < indexBuffer.size(); i= i +3)
		{
			Vec3 v0 = vertexBuffer[indexBuffer[i]].m_position;
			Vec3 v1 = vertexBuffer[indexBuffer[i + 1]].m_position;
			Vec3 v2 = vertexBuffer[indexBuffer[i + 2]].m_position;

			Vec3 normal = ((v1 - v0).cross(v2 - v0));
			if (!(normal.dot(v0) > 0.0f))
			{
				//donot cull this particular triangle..
				vertex v0 = vertexBuffer[indexBuffer[i]];
				vertex v1 = vertexBuffer[indexBuffer[i + 1]];
				vertex v2 = vertexBuffer[indexBuffer[i + 2]];
				
				screenSpaceTransformer(v0, v1, v2);
				triangleRasterizer(v0, v1, v2);
			}
		}
	}

	static void screenSpaceTransformer(vertex& v0, vertex& v1, vertex& v2)
	{
		v0.m_position = pubeToScreenTransformer::getCoordinatesInScreenSpace(v0.m_position, gfx->ScreenWidth, gfx->ScreenHeight);
		v1.m_position = pubeToScreenTransformer::getCoordinatesInScreenSpace(v1.m_position, gfx->ScreenWidth, gfx->ScreenHeight);
		v2.m_position = pubeToScreenTransformer::getCoordinatesInScreenSpace(v2.m_position, gfx->ScreenWidth, gfx->ScreenHeight);
	}
	static void screenSpaceTransformer(vertex& v0)
	{
		v0.m_position = pubeToScreenTransformer::getCoordinatesInScreenSpace(v0.m_position, gfx->ScreenWidth, gfx->ScreenHeight);
	}

	static void triangleRasterizer(vertex v0, vertex v1, vertex v2)
	{
		const vertex* p0 = &v0;
		const vertex* p1 = &v1;
		const vertex* p2 = &v2;

		// sorting vertices by y
		if (p0->m_position.y > p1->m_position.y) std::swap(p0, p1);
		if (p1->m_position.y > p2->m_position.y) std::swap(p1, p2);
		if (p0->m_position.y > p1->m_position.y) std::swap(p0, p1);

		///case for p1 and p2 y value equal not handled.. basically.. no natural flat bottom is being handled..
		if (p1->m_position.y == p2->m_position.y) //natural flat bottom
			drawTexturedFlatBottomTriangle(*p0, *p1, *p2);
		else if (p0->m_position.y == p1->m_position.y) //natural flat top
			drawTexturedFlatTopTriangle(*p2, *p0, *p1);
		else
		{
			//normal triangle which needs to be split into natural flat bottom and natural flat top
			float alphaSplit = (p1->m_position.y - p0->m_position.y) / (p2->m_position.y - p0->m_position.y);
			vertex splitPoint = p0->interpolateTo(*p2, alphaSplit);

			//draw flat bottom and flat top
			drawTexturedFlatBottomTriangle(*p0, *p1, splitPoint); //flat bottom triangle
			drawTexturedFlatTopTriangle(*p2, *p1, splitPoint); //flat top triangle
		}
	}


private:
	static void TriangleRasterizer()
	{

	}

	static void scanFlatBottomTriangle(const vertex& p0, const vertex& p1, const vertex& p2)
	{
		//p1 and p2 form the straight line always and p0 is to the top of p1 and p2..
		const vertex* p0 = &point1;
		const vertex* p1 = &point2;
		const vertex* p2 = &point3;

		//see that p1 is to the left of p2 always.
		if (p1->m_position.x > p2->m_position.x)
			std::swap(p1, p2);

		Vec2 leftEdge = p0->m_position.x;
		Vec2 rightEdge = p0->m_position.x;
		Vec2 leftBottomEdge = p1->m_position.x;
		Vec2 rightBottomEdge = p2->m_position.x;

		
	}
	static void scanFlatTopTriangle(const vertex& p0, const vertex& p1, const vertex& p2)
	{

	}

	static void scanTriangle(const vertex& p0, const vertex& p1, const vertex& p2,
							 vertex letfEdge,vertex rightEdge,vertex deltaLeftEdge, vertex deltaRightEdge)
	{

	}




	static void drawTexturedFlatBottomTriangle(const vertex& point1, const vertex& point2, const vertex& point3)
	{
		//p1 and p2 form the straight line always and p0 is to the top of p1 and p2..
		const vertex* p0 = &point1;
		const vertex* p1 = &point2;
		const vertex* p2 = &point3;

		//see that p1 is to the left of p2 always.
		if (p1->m_position.x > p2->m_position.x)
			std::swap(p1, p2);

		float w12 = (p1->m_position.x - p0->m_position.x) / (p1->m_position.y - p0->m_position.y); //inverse slope
		float w13 = (p2->m_position.x - p0->m_position.x) / (p2->m_position.y - p0->m_position.y); //inverse slope

		float yStart = std::floor(p0->m_position.y + 0.5f);
		float yEnd = std::floor(p1->m_position.y + 0.5f);

		float leftEdgeDelta = w12;
		float rightEdgeDelta = w13;
		int count = 0; //the count variable keeps track of how many y spaces we advanced.. so that xStart and xEnd can be calculated accordingly..

		Vec2 tcLeftEdge = p0->m_uv_coordinates;
		Vec2 tcRightEdge = p0->m_uv_coordinates;

		Vec2 tcBottomLeftEdge = p1->m_uv_coordinates;
		Vec2 tcBottomRightEdge = p2->m_uv_coordinates;

		Vec2 tcScanStepLeft = (tcBottomLeftEdge - tcLeftEdge) / (p1->m_position - p0->m_position).y;
		Vec2 tcScanStepRight = (tcBottomRightEdge - tcRightEdge) / (p2->m_position - p0->m_position).y;

		for (int y = yStart; y < yEnd; y++, tcLeftEdge += tcScanStepLeft, tcRightEdge += tcScanStepRight)
		{
			float xStart = p0->m_position.x + (count * leftEdgeDelta);
			float xEnd = p0->m_position.x + (count * rightEdgeDelta);
			count++;

			Vec2 tcScanStep = (tcRightEdge - tcLeftEdge) / (xEnd - xStart);
			Vec2 tcStart = tcLeftEdge;

			for (int x = xStart; x < xEnd; x++, tcStart += tcScanStep)
			{
				gfx->PutPixel(x, y, m_texture->GetPixel(std::clamp(tcStart.x * m_texture->GetWidth(), 0.0f, (float)m_texture->GetWidth() - 1.0f),
					std::clamp(tcStart.y * m_texture->GetHeight(), 0.0f, (float)m_texture->GetHeight() - 1.0f)
				));
			}
		}
	}
	static void drawTexturedFlatTopTriangle(const vertex& point1, const vertex& point2, const vertex& point3)
	{
		//p1 and p2 form the straight line always and p0 is to the bottom of p1 and p2..
		const vertex* p0 = &point1;
		const vertex* p1 = &point2;
		const vertex* p2 = &point3;

		//see that p1 is to the left of p2 always.
		if (p1->m_position.x > p2->m_position.x)
			std::swap(p1, p2);

		float w12 = (p1->m_position.x - p0->m_position.x) / (p1->m_position.y - p0->m_position.y); //inverse slope
		float w13 = (p2->m_position.x - p0->m_position.x) / (p2->m_position.y - p0->m_position.y); //inverse slope

		float yStart = std::ceil(p1->m_position.y - 0.5f);
		float yEnd = std::floor(p0->m_position.y + 0.5f);

		Vec2 tcEdgeLeft = p1->m_uv_coordinates;
		Vec2 tcEdgeRight = p2->m_uv_coordinates;
		Vec2 tcEdgeBottomLeft = p0->m_uv_coordinates;
		Vec2 tcEdgeBottomRight = p0->m_uv_coordinates;

		Vec2 tcScanStepLeft = (tcEdgeBottomLeft - tcEdgeLeft) / (p0->m_position - p1->m_position).y;
		Vec2 tcScanStepRight = (tcEdgeBottomRight - tcEdgeRight) / (p0->m_position - p2->m_position).y;

		float leftEdgeDelta = w12;
		float rightEdgeDelta = w13;
		int count = 0; //the count variable keeps track of how many y spaces we advanced.. so that xStart and xEnd can be calculated accordingly..

		for (int y = yStart; y < yEnd; y++, tcEdgeLeft += tcScanStepLeft, tcEdgeRight += tcScanStepRight)
		{
			float xStart = p1->m_position.x + (count * leftEdgeDelta);
			float xEnd = p2->m_position.x + (count * rightEdgeDelta);
			count++;

			Vec2 tcScanStep = (tcEdgeRight - tcEdgeLeft) / (xEnd - xStart);
			Vec2 tcStart = tcEdgeLeft;

			for (int x = xStart; x < xEnd; x++, tcStart += tcScanStep)
			{
				gfx->PutPixel(x, y, m_texture->GetPixel(std::clamp(tcStart.x * m_texture->GetWidth(), 0.0f, (float)m_texture->GetWidth() - 1.0f),
					std::clamp(tcStart.y * m_texture->GetHeight(), 0.0f, (float)m_texture->GetHeight() - 1.0f)
				));
			}
		}
	}

private:
	static std::unique_ptr<Surface> m_texture;
	static Mat3 m_rotationMatrix; //the rotation is on model centre
	static Vec3 m_translation; // the translation is with respect to the model centre
	static Graphics* gfx;

	friend class Game;
};

//static variables definition
template<typename effect> std::unique_ptr<Surface> pipeline<effect>::m_texture = nullptr;
template<typename effect> Mat3 pipeline<effect>::m_rotationMatrix = Mat3::Identity();
template<typename effect> Vec3 pipeline<effect>::m_translation = Vec3(0.0f, 0.0f, 0.0f);
template<typename effect> Graphics* pipeline<effect>::gfx = nullptr;