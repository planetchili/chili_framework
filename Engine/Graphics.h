/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.h																			  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once
#include "ChiliWin.h"
#include <d3d11.h>
#include <wrl.h>
#include "ChiliException.h"
#include "Colors.h"
#include "JezierVec2.h"
#include <math.h>


class Graphics
{
public:
	class Exception : public ChiliException
	{
	public:
		Exception( HRESULT hr,const std::wstring& note,const wchar_t* file,unsigned int line );
		std::wstring GetErrorName() const;
		std::wstring GetErrorDescription() const;
		virtual std::wstring GetFullMessage() const override;
		virtual std::wstring GetExceptionType() const override;
	private:
		HRESULT hr;
	};
private:
	// vertex format for the framebuffer fullscreen textured quad
	struct FSQVertex
	{
		float x,y,z;		// position
		float u,v;			// texcoords
	};
public:
	Graphics( class HWNDKey& key );
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;
	void EndFrame();
	void BeginFrame();
	void PutPixel( int x,int y,int r,int g,int b )
	{
		PutPixel( x,y,{ unsigned char( r ),unsigned char( g ),unsigned char( b ) } );
	}
	void PutPixel( int x,int y,Color c );
	
	/***** Start Draw Line Functions *****/

	template<typename T>
	void DrawLine(const _Vec2<T> v1, const _Vec2<T> v2, Color c)
	{
		DrawLine((float)v1.x, (float)v1.y, (float)v2.x, (float)v2.y, c);
	}
	void DrawLine(float x1, float y1, float x2, float y2, Color c);

	/***** END Draw Line Functions ****/


	/***** Start Draw Circle Functions *****/
	template<typename T2>
	void DrawCircle(_Vec2<T2> vO, T2 R, Color c)
	{
		DrawCircle((float)vO.x, (float)vO.y, (float)R, c);
	}
	//Copmponet Radius circle
	void DrawCircle(Vec2 vO,Vec2 Comp_Rad,Color c);
	void DrawCircle(float Ox, float Oy, float x1, float y1, Color c);
	void DrawCircle(float Ox, float Oy, float R, Color c);
	
	//Three point circle 
	void DrawCircle(float x1, float y1, //first point
		float x2, float y2, //second point
		float x3, float y3, //curvature direction 
		Color c); 	

	/***** END Draw Circle Functions ****/


	/*Draw Arc Functions*/
	void DrawArc(float Ox, float Oy,float R, float theta_begin,float theta_end, Color c);
	template<typename T3>
	void DrawArc(_Vec2<T3> vO, T3 R, Color c)
	{
		DrawArc((float)vO.x, (float)vO.y, (float)R, c);
	}

	/*Elipse Drawing functions*/
	void DrawFlatEllipse(float Ox, float Oy, float Rx, float Ry, Color c);
	

	~Graphics();
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain>				pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device>				pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			pImmediateContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				pSysBufferTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	pSysBufferTextureView;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			pSamplerState;
	D3D11_MAPPED_SUBRESOURCE							mappedSysBufferTexture;
	Color*                                              pSysBuffer = nullptr;
public:
	static constexpr int ScreenWidth =1440;
	static constexpr int ScreenHeight = 900;
};