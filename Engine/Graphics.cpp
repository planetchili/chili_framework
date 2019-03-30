/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.cpp																		  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
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
#include "MainWindow.h"
#include "Graphics.h"
#include "DXErr.h"
#include "ChiliException.h"
#include <assert.h>
#include <string>
#include <array>

#include "Chili_Rectangle.h"


// Ignore the intellisense error "cannot open source file" for .shh files.
// They will be created during the build sequence before the preprocessor runs.
namespace FramebufferShaders
{
#include "FramebufferPS.shh"
#include "FramebufferVS.shh"
}

#pragma comment( lib,"d3d11.lib" )

#define CHILI_GFX_EXCEPTION( hr,note ) Graphics::Exception( hr,note,_CRT_WIDE(__FILE__),__LINE__ )

using Microsoft::WRL::ComPtr;

Graphics::Graphics( HWNDKey& key )
{
	assert( key.hWnd != nullptr );

	//////////////////////////////////////////////////////
	// create device and swap chain/get render target view
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;
	sd.BufferDesc.Width = Graphics::ScreenWidth;
	sd.BufferDesc.Height = Graphics::ScreenHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 1;
	sd.BufferDesc.RefreshRate.Denominator = 60;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = key.hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	HRESULT				hr;
	UINT				createFlags = 0u;
#ifdef CHILI_USE_D3D_DEBUG_LAYER
#ifdef _DEBUG
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
#endif
	
	// create device and front/back buffers
	if( FAILED( hr = D3D11CreateDeviceAndSwapChain( 
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pImmediateContext ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating device and swap chain" );
	}

	// get handle to backbuffer
	ComPtr<ID3D11Resource> pBackBuffer;
	if( FAILED( hr = pSwapChain->GetBuffer(
		0,
		__uuidof( ID3D11Texture2D ),
		(LPVOID*)&pBackBuffer ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Getting back buffer" );
	}

	// create a view on backbuffer that we can render to
	if( FAILED( hr = pDevice->CreateRenderTargetView( 
		pBackBuffer.Get(),
		nullptr,
		&pRenderTargetView ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating render target view on backbuffer" );
	}


	// set backbuffer as the render target using created view
	pImmediateContext->OMSetRenderTargets( 1,pRenderTargetView.GetAddressOf(),nullptr );


	// set viewport dimensions
	D3D11_VIEWPORT vp;
	vp.Width = float( Graphics::ScreenWidth );
	vp.Height = float( Graphics::ScreenHeight );
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pImmediateContext->RSSetViewports( 1,&vp );


	///////////////////////////////////////
	// create texture for cpu render target
	D3D11_TEXTURE2D_DESC sysTexDesc;
	sysTexDesc.Width = Graphics::ScreenWidth;
	sysTexDesc.Height = Graphics::ScreenHeight;
	sysTexDesc.MipLevels = 1;
	sysTexDesc.ArraySize = 1;
	sysTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sysTexDesc.SampleDesc.Count = 1;
	sysTexDesc.SampleDesc.Quality = 0;
	sysTexDesc.Usage = D3D11_USAGE_DYNAMIC;
	sysTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	sysTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sysTexDesc.MiscFlags = 0;
	// create the texture
	if( FAILED( hr = pDevice->CreateTexture2D( &sysTexDesc,nullptr,&pSysBufferTexture ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating sysbuffer texture" );
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = sysTexDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// create the resource view on the texture
	if( FAILED( hr = pDevice->CreateShaderResourceView( pSysBufferTexture.Get(),
		&srvDesc,&pSysBufferTextureView ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating view on sysBuffer texture" );
	}


	////////////////////////////////////////////////
	// create pixel shader for framebuffer
	// Ignore the intellisense error "namespace has no member"
	if( FAILED( hr = pDevice->CreatePixelShader(
		FramebufferShaders::FramebufferPSBytecode,
		sizeof( FramebufferShaders::FramebufferPSBytecode ),
		nullptr,
		&pPixelShader ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating pixel shader" );
	}
	

	/////////////////////////////////////////////////
	// create vertex shader for framebuffer
	// Ignore the intellisense error "namespace has no member"
	if( FAILED( hr = pDevice->CreateVertexShader(
		FramebufferShaders::FramebufferVSBytecode,
		sizeof( FramebufferShaders::FramebufferVSBytecode ),
		nullptr,
		&pVertexShader ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating vertex shader" );
	}
	

	//////////////////////////////////////////////////////////////
	// create and fill vertex buffer with quad for rendering frame
	const FSQVertex vertices[] =
	{
		{ -1.0f,1.0f,0.5f,0.0f,0.0f },
		{ 1.0f,1.0f,0.5f,1.0f,0.0f },
		{ 1.0f,-1.0f,0.5f,1.0f,1.0f },
		{ -1.0f,1.0f,0.5f,0.0f,0.0f },
		{ 1.0f,-1.0f,0.5f,1.0f,1.0f },
		{ -1.0f,-1.0f,0.5f,0.0f,1.0f },
	};
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( FSQVertex ) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;
	if( FAILED( hr = pDevice->CreateBuffer( &bd,&initData,&pVertexBuffer ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating vertex buffer" );
	}

	
	//////////////////////////////////////////
	// create input layout for fullscreen quad
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};

	// Ignore the intellisense error "namespace has no member"
	if( FAILED( hr = pDevice->CreateInputLayout( ied,2,
		FramebufferShaders::FramebufferVSBytecode,
		sizeof( FramebufferShaders::FramebufferVSBytecode ),
		&pInputLayout ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating input layout" );
	}


	////////////////////////////////////////////////////
	// Create sampler state for fullscreen textured quad
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	if( FAILED( hr = pDevice->CreateSamplerState( &sampDesc,&pSamplerState ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating sampler state" );
	}

	// allocate memory for sysbuffer (16-byte aligned for faster access)
	pSysBuffer = reinterpret_cast<Color*>( 
		_aligned_malloc( sizeof( Color ) * Graphics::ScreenWidth * Graphics::ScreenHeight,16u ) );
}

Graphics::~Graphics()
{
	// free sysbuffer memory (aligned free)
	if( pSysBuffer )
	{
		_aligned_free( pSysBuffer );
		pSysBuffer = nullptr;
	}
	// clear the state of the device context before destruction
	if( pImmediateContext ) pImmediateContext->ClearState();
}

void Graphics::EndFrame()
{
	HRESULT hr;

	// lock and map the adapter memory for copying over the sysbuffer
	if( FAILED( hr = pImmediateContext->Map( pSysBufferTexture.Get(),0u,
		D3D11_MAP_WRITE_DISCARD,0u,&mappedSysBufferTexture ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Mapping sysbuffer" );
	}
	// setup parameters for copy operation
	Color* pDst = reinterpret_cast<Color*>(mappedSysBufferTexture.pData );
	const size_t dstPitch = mappedSysBufferTexture.RowPitch / sizeof( Color );
	const size_t srcPitch = Graphics::ScreenWidth;
	const size_t rowBytes = srcPitch * sizeof( Color );
	// perform the copy line-by-line
	for( size_t y = 0u; y < Graphics::ScreenHeight; y++ )
	{
		memcpy( &pDst[ y * dstPitch ],&pSysBuffer[y * srcPitch],rowBytes );
	}
	// release the adapter memory
	pImmediateContext->Unmap( pSysBufferTexture.Get(),0u );

	// render offscreen scene texture to back buffer
	pImmediateContext->IASetInputLayout( pInputLayout.Get() );
	pImmediateContext->VSSetShader( pVertexShader.Get(),nullptr,0u );
	pImmediateContext->PSSetShader( pPixelShader.Get(),nullptr,0u );
	pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	const UINT stride = sizeof( FSQVertex );
	const UINT offset = 0u;
	pImmediateContext->IASetVertexBuffers( 0u,1u,pVertexBuffer.GetAddressOf(),&stride,&offset );
	pImmediateContext->PSSetShaderResources( 0u,1u,pSysBufferTextureView.GetAddressOf() );
	pImmediateContext->PSSetSamplers( 0u,1u,pSamplerState.GetAddressOf() );
	pImmediateContext->Draw( 6u,0u );

	// flip back/front buffers
	if( FAILED( hr = pSwapChain->Present( 1u,0u ) ) )
	{
		if( hr == DXGI_ERROR_DEVICE_REMOVED )
		{
			throw CHILI_GFX_EXCEPTION( pDevice->GetDeviceRemovedReason(),L"Presenting back buffer [device removed]" );
		}
		else
		{
			throw CHILI_GFX_EXCEPTION( hr,L"Presenting back buffer" );
		}
	}
}

void Graphics::BeginFrame()
{
	// clear the sysbuffer
	memset( pSysBuffer,0u,sizeof( Color ) * Graphics::ScreenHeight * Graphics::ScreenWidth );
}

void Graphics::PutPixel( int x,int y,Color c )
{
	assert( x >= 0 );
	assert( x < int( Graphics::ScreenWidth ) );
	assert( y >= 0 );
	assert( y < int( Graphics::ScreenHeight ) );
	pSysBuffer[Graphics::ScreenWidth * y + x] = c;
}


// line with per pixel clipping
void Graphics::DrawLine(double x1, double y1, double x2, double y2, Color c)
{
	const double dx = x2 - x1;
	const double dy = y2 - y1;

	if (dx == 0.0f && dy == 0.0f)
	{
		if (x1 >= 0 && x1 < ScreenWidth && y1 >= 0 && y1  < ScreenHeight)
		PutPixel((int)x1, (int)y1, Colors::Red);
	}
	else if (abs(dx)>abs(dy))
	{
		if (dx < 0.0f)
		{
			std::swap(x1, x2);
			std::swap(y1, y2);
		}

		double m = dy / dx;
		double b = y1 - m * x1;

		for (double x = x1; x <= x2; x++)
		{

			double y = m * x + b;
			// issue! why there is need for substraction of one ?
			if (x >= 0 && x < ScreenWidth-1 && y >= 0 && y  < ScreenHeight-1)
				PutPixel((int)(x + 0.5), (int)(y + 0.5), c);
			
		}
	}
	else
	{
		if (dy < 0.0f)
		{
			std::swap(x1, x2);
			std::swap(y1, y2);
		}

		double m = dx / dy;
		double b = x1 - m * y1;

		for (double y = y1; y <= y2; y++)
		{
			double x = m * y + b;
			// issue! why there is need for substraction of one ?
			if (x >= 0 && x < ScreenWidth-1 && y >= 0 && y  < ScreenHeight-1)
				PutPixel((int)(x + 0.5), (int)(y + 0.5), c);
		}
	}
}

void Graphics::DrawPoliLine(std::vector<JC_Point2d> point_data, Color Color_in)
{
	JC_Point2d Current;
	JC_Point2d Previous;

	
	if (point_data.size() > 1)
	{
		for (int i = 1; i < point_data.size(); i++)
		{
			Current = point_data[i];
			Previous = point_data[i - 1];
			DrawLine(Previous, Current, Color_in);
		}
	}
	else
	{
		
	}
	
}


void Graphics::DrawCircle(double _ox, double _oy, double _outer_radius, const CRectangle<double>& _clip, int t,Color C) noexcept
{// For outline thickness of 1
	const auto rSq_inner = Square(_outer_radius - t);
	const auto rSq_outer = Square(_outer_radius);

	const auto outer_double = _outer_radius * 2.0;

	// Calculate the bounding rectangle of the circle
	const auto left = _ox - _outer_radius;
	const auto top = _oy - _outer_radius;
	const auto right = _ox + _outer_radius;
	const auto bottom = _oy + _outer_radius;

	// Clip the bounding rectangle to screen boundaries and translate 
	// back to -radius ( left_clip, top_clip ), +radius ( right_clip, bottom_clip )

	const auto left_clip = std::max(0.0, -left) - _outer_radius;
	const auto top_clip = std::max(0.0, -top) - _outer_radius;
	const auto right_clip = std::min(ScreenWidth - left -1.0, outer_double) - _outer_radius;
	const auto bottom_clip = std::min(ScreenHeight - top - 1.0, outer_double) - _outer_radius;

	// Loop through clipped bounding rectangle, from top to bottom,
	// left to right skipping any pixels contained in the _clip Rect passed
	// as parameter to the function
	for (double y = top_clip; y < bottom_clip; ++y)
	{
		for (double x = left_clip; x < right_clip; ++x)
		{
			const auto sqDist = Square(x) + Square(y);
			if (sqDist > rSq_inner && sqDist < rSq_outer)
			{
				const auto px = x + _ox;
				const auto py = y + _oy;

				if (!_clip.Contains(JC_Vector2<double>{ px, py }))
				{
					PutPixel(int(std::round(px)), int(std::round(py)), C);
				}
			}
		}
	}
}


void Graphics::DrawBezier(const JC_Point2d & P, const JC_Point2d & Q, const JC_Point2d & R, Color color) noexcept
{
	const auto range0 = (Q - P);
	const auto range1 = (R - Q);
	const auto range2 = (range1 - range0);
	const auto doubleRange0 = (range0 * 2.0);

	constexpr auto step = .01;
	auto prev = P;
	for (double t = step; t <= 1.0; t += step)
	{
		const auto S = P + (doubleRange0 + (range2 * t)) * t;

		DrawLine(prev, S, color);
		prev = S;
	}
}


void Graphics::DrawBezier(std::vector<JC_Point2d> point_data, Color color) noexcept
{
/*	const auto range0 = (Q - P);
	const auto range1 = (R - Q);
	const auto range2 = (range1 - range0);
	const auto doubleRange0 = (range0 * 2.0);

	constexpr auto step = .1;
	auto prev = P;
	for (double t = step; t <= 1.0; t += step)
	{
		const auto S = P + (doubleRange0 + (range2 * t)) * t;

		DrawLine(prev, S, color);
		prev = S;
	}
*/
}


/*
void Graphics::DrawCircle(double Ox, double Oy, double R, Color& c)
{
	//issue with continuity drawing for large circles
	
	for (double theta = 0; theta < 360; theta += 0.2)
	{
		double x = (double)(R * std::cos(PI_D*theta / 180));
		double y = (double)(R * std::sin(PI_D*theta / 180));

		int xi = (int)(x + 0.5f + Ox);
		int yi = (int)(y + 0.5f + Oy);

		if (xi >= 0 && xi < ScreenWidth && yi >= 0 && yi < ScreenHeight)
			PutPixel(xi, yi, c);
	}
	


	double x = 0.7071067811865475;
	
	double Rx = (x * R) + 0.5;
	
	double radsqr = R * R;
	
	//draw Circle with per pixel clipping

	for (int xi = 0; xi <= (int)Rx; xi++)
	{
		int yi = (int)(std::sqrt(radsqr - (xi*xi)) + 0.5f);


		if (Ox + xi >= 0 && Ox + xi < ScreenWidth - 1 && Oy + yi >= 0 && Oy + yi < ScreenHeight-1)
			PutPixel((int)Ox + xi, (int)Oy + yi, c);									   
																						   
		if (Ox + yi >= 0 && Ox + yi < ScreenWidth - 1 && Oy + xi >= 0 && Oy + xi < ScreenHeight-1)
			PutPixel((int)Ox + yi, (int)Oy + xi, c);									   
																						   
		if (Ox -xi >= 0 && Ox -xi < ScreenWidth - 1 && Oy + yi >= 0 && Oy + yi <   ScreenHeight-1)
			PutPixel((int)Ox - xi, (int)Oy + yi, c);									   
																						   
		if (Ox -yi >= 0 && Ox -yi < ScreenWidth - 1 && Oy + xi >= 0 && Oy + xi <   ScreenHeight-1)
			PutPixel((int)Ox - yi, (int)Oy + xi, c);									   
																						   
		if (Ox -xi >= 0 && Ox -xi < ScreenWidth - 1 && Oy -yi >= 0 && Oy -yi <     ScreenHeight-1)
			PutPixel((int)Ox - xi, (int)Oy - yi, c);									   
																						   
		if (Ox -yi >= 0 && Ox-yi < ScreenWidth - 1 && Oy -xi >= 0 && Oy -xi <	   ScreenHeight-1)
			PutPixel((int)Ox - yi, (int)Oy - xi, c);									   
																						   
		if (Ox + xi >= 0 && Ox + xi < ScreenWidth-1 && Oy -yi >= 0 && Oy -yi < ScreenHeight-1)
			PutPixel((int)Ox + xi, (int)Oy - yi, c);									   

		if (Ox + yi >= 0 && Ox + yi < ScreenWidth-1 && Oy -xi >= 0 && Oy -xi < ScreenHeight-1)
			PutPixel((int)Ox + yi, (int)Oy - xi, c);
	}
		       
	
}
*/



/*
void Graphics::DrawArc(double Ox, double Oy, double R , double theta_begin, double theta_end, Color c)
{

	bool theta_range = theta_end - theta_begin > 0.0;


	if (theta_begin == theta_end)
	{
		DrawCircle(Ox, Oy ,R,c);
	}
	else
	{
		for (double theta = theta_begin;
			theta_range ? theta < theta_end : theta > theta_end;
			theta_range ? theta += 0.2 : theta -= 0.2)
		{
			double x = (double)(R * std::cos(PI_F*theta / 180));
			double y = (double)(R * std::sin(PI_F*theta / 180));

				
			//Draw arc
			PutPixel((int)(x + 0.5f + Ox), (int)(y + 0.5f + Oy), c);
		}
	}
}
*/



//////////////////////////////////////////////////
//           Graphics Exception
Graphics::Exception::Exception( HRESULT hr,const std::wstring& note,const wchar_t* file,unsigned int line )
	:
	ChiliException( file,line,note ),
	hr( hr )
{}

std::wstring Graphics::Exception::GetFullMessage() const
{
	const std::wstring empty = L"";
	const std::wstring errorName = GetErrorName();
	const std::wstring errorDesc = GetErrorDescription();
	const std::wstring& note = GetNote();
	const std::wstring location = GetLocation();
	return    (!errorName.empty() ? std::wstring( L"Error: " ) + errorName + L"\n"
		: empty)
		+ (!errorDesc.empty() ? std::wstring( L"Description: " ) + errorDesc + L"\n"
			: empty)
		+ (!note.empty() ? std::wstring( L"Note: " ) + note + L"\n"
			: empty)
		+ (!location.empty() ? std::wstring( L"Location: " ) + location
			: empty);
}

std::wstring Graphics::Exception::GetErrorName() const
{
	return DXGetErrorString( hr );
}

std::wstring Graphics::Exception::GetErrorDescription() const
{
	std::array<wchar_t,512> wideDescription;
	DXGetErrorDescription( hr,wideDescription.data(),wideDescription.size() );
	return wideDescription.data();
}

std::wstring Graphics::Exception::GetExceptionType() const
{
	return L"Chili Graphics Exception";
}