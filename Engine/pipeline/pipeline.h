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

	pipeline(effect obj,Graphics* gfx)
		: m_effectFunctor(std::move(obj)), gfx(gfx)
	{}
	
	void bindRotationMatrix(Mat3 rotationMat) { m_rotationMatrix = rotationMat; }
	void bindTranslation(Vec3 translateby) { m_translation = translateby; }

	void draw(std::vector<uint32_t> indexBuffer, std::vector<vertex> vertexBuffer)
	{
		vertexTransformer(vertexBuffer);
		triangleRasterizer(indexBuffer, vertexBuffer);
	}

private:
	void vertexTransformer(std::vector<vertex>& vertexBuffer)
	{
		for (auto& vertex : vertexBuffer)
		{
			vertex.m_position = vertex.m_position * m_rotationMatrix;
			vertex.m_position += m_translation;
		}
	}

	void triangleRasterizer(std::vector<uint32_t> indexBuffer, std::vector<vertex> vertexBuffer)
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

	void screenSpaceTransformer(vertex& v0, vertex& v1, vertex& v2)
	{
		v0.m_position = pubeToScreenTransformer::getCoordinatesInScreenSpace(v0.m_position, gfx->ScreenWidth, gfx->ScreenHeight);
		v1.m_position = pubeToScreenTransformer::getCoordinatesInScreenSpace(v1.m_position, gfx->ScreenWidth, gfx->ScreenHeight);
		v2.m_position = pubeToScreenTransformer::getCoordinatesInScreenSpace(v2.m_position, gfx->ScreenWidth, gfx->ScreenHeight);
	}
	void screenSpaceTransformer(vertex& v0)
	{
		v0.m_position = pubeToScreenTransformer::getCoordinatesInScreenSpace(v0.m_position, gfx->ScreenWidth, gfx->ScreenHeight);
	}

	void triangleRasterizer(vertex v0, vertex v1, vertex v2)
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
			drawFlatBottomTriangle(*p0, *p1, *p2);
		else if (p0->m_position.y == p1->m_position.y) //natural flat top
			drawFlatTopTriangle(*p2, *p0, *p1);
		else
		{
			//normal triangle which needs to be split into natural flat bottom and natural flat top
			float alphaSplit = (p1->m_position.y - p0->m_position.y) / (p2->m_position.y - p0->m_position.y);
			vertex splitPoint = p0->interpolateTo(*p2, alphaSplit);

			//draw flat bottom and flat top
			drawFlatBottomTriangle(*p0, *p1, splitPoint); //flat bottom triangle
			drawFlatTopTriangle(*p2, *p1, splitPoint); //flat top triangle
		}
	}


private:
	void drawFlatBottomTriangle(const vertex& vertex0, const vertex& vertex1, const vertex& vertex2)
	{
		//v1 and v2 form the straight line always and v0 is to the top of p1 and p2..
		const vertex* v0 = &vertex0;
		const vertex* v1 = &vertex1;
		const vertex* v2 = &vertex2;

		//see that p1 is to the left of p2 always.
		if (v1->m_position.x > v2->m_position.x)
			std::swap(v1, v2);

		vertex leftEdge		= *v0;
		vertex rightEdge	= *v0;
		vertex leftEdgeEnd	= *v1;
		vertex rightEdgeEnd = *v2;

		drawFlatTriangle(leftEdge, rightEdge, leftEdgeEnd, rightEdgeEnd);
	}
	void drawFlatTopTriangle(const vertex& vertex0, const vertex& vertex1, const vertex& vertex2)
	{
		//p1 and p2 form the straight line always and p0 is to the bottom of p1 and p2..
		const vertex* v0 = &vertex0;
		const vertex* v1 = &vertex1;
		const vertex* v2 = &vertex2;

		//see that p1 is to the left of p2 always.
		if (v1->m_position.x > v2->m_position.x)
			std::swap(v1, v2);

		vertex leftEdge		= *v1;
		vertex rightEdge	= *v2;
		vertex leftEdgeEnd	= *v0;
		vertex rightEdgeEnd = *v0;

		drawFlatTriangle(leftEdge, rightEdge, leftEdgeEnd, rightEdgeEnd);
	}
	void drawFlatTriangle(vertex leftEdge,vertex rightEdge,vertex leftEdgeEnd,vertex rightEdgeEnd)
	{
		vertex leftEdgeDelta = (leftEdgeEnd - leftEdge) / (leftEdgeEnd.m_position - leftEdge.m_position).y;
		vertex rightEdgeDelta = (rightEdgeEnd - rightEdge) / (rightEdgeEnd.m_position - rightEdge.m_position).y;

		//prestepping to stop division by zero
		leftEdge += leftEdgeDelta;
		rightEdge += rightEdgeDelta;

		for (; leftEdge.m_position.y <= leftEdgeEnd.m_position.y; leftEdge += leftEdgeDelta, rightEdge += rightEdgeDelta)
		{
			vertex yStart = leftEdge;
			vertex yEnd = rightEdge;

			vertex deltaX = (yEnd - yStart) / (yEnd.m_position - yStart.m_position).x;

			for (; yStart.m_position.x <= yEnd.m_position.x; yStart += deltaX)
			{
				gfx->PutPixel(yStart.m_position.x, yStart.m_position.y, m_effectFunctor.ps(yStart));
			}
		}
	}

private:
	Mat3 m_rotationMatrix; //the rotation is on model centre
	Vec3 m_translation; // the translation is with respect to the model centre
	Graphics* gfx;

	effect m_effectFunctor;

	friend class Game;
};