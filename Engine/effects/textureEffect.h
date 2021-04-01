#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<filesystem>
#include<memory>

#include<utility>
#include<algorithm>

#include "dataStructure/math/Vec2.h"
#include "dataStructure/math/Vec3.h"
#include "dataStructure/math/Mat3.h"

#include "graphics/Surface.h"
#include "graphics/Colors.h"

class textureEffect
{
public:
	textureEffect() = delete;
	textureEffect(std::filesystem::path texturePath){ bindTexture(texturePath);	}

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


	class pixelShader
	{
	public:
		Color operator()(textureEffect::vertex vert)
		{
			return m_texture->GetPixel(	std::clamp(vert.m_uv_coordinates.x * m_texture->GetWidth(), 0.0f, (float)m_texture->GetWidth() - 1.0f),
										std::clamp(vert.m_uv_coordinates.y * m_texture->GetHeight(), 0.0f, (float)m_texture->GetHeight() - 1.0f)
			);
		}
		std::unique_ptr<Surface> m_texture = nullptr;
	};

public:
	void bindTexture(std::unique_ptr<Surface> texture)  { ps.m_texture  = std::move(texture); }
	void bindTexture(std::filesystem::path texturePath) { ps.m_texture = std::make_unique<Surface>(Surface::FromFile(texturePath)); }

public:
	pixelShader ps;
};