#include "texturedVertex.h"

texturedVertex::texturedVertex(Vec3 position,Vec2 uv_coordinates)
	:
	m_position(position),
	m_uv_coordinates(uv_coordinates)
{}

texturedVertex texturedVertex::interpolateTo(const texturedVertex& rhs, float alpha) const
{
    texturedVertex vertex;
	vertex.m_position = (m_position * (1 - alpha)) + (rhs.m_position * (alpha));
	vertex.m_uv_coordinates = (m_uv_coordinates * (1 - alpha)) + (rhs.m_uv_coordinates * (alpha));

	return vertex;
}