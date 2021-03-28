/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
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
#include <functional>

#include "mathFunctions.h"

// Ignore the intellisense error "cannot open source file" for .shh files.
// They will be created during the build sequence before the preprocessor runs.
namespace FramebufferShaders
{
#include "FramebufferPS.shh"
#include "FramebufferVS.shh"
}

#pragma comment( lib,"d3d11.lib" )

using Microsoft::WRL::ComPtr;

Graphics::Graphics( HWNDKey& key )
	:
	sysBuffer( ScreenWidth,ScreenHeight )
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

	D3D_FEATURE_LEVEL	featureLevelsRequested = D3D_FEATURE_LEVEL_9_1;
	UINT				numLevelsRequested = 1;
	D3D_FEATURE_LEVEL	featureLevelsSupported;
	HRESULT				hr;
	UINT				createFlags = 0u;
#ifdef _DEBUG
#ifdef USE_DIRECT3D_DEBUG_RUNTIME
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
#endif
	
	// create device and front/back buffers
	if( FAILED( hr = D3D11CreateDeviceAndSwapChain( 
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createFlags,
		&featureLevelsRequested,
		numLevelsRequested,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pDevice,
		&featureLevelsSupported,
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
}

Graphics::~Graphics()
{
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
	// perform the copy line-by-line
	sysBuffer.Present( mappedSysBufferTexture.RowPitch,
		reinterpret_cast<BYTE*>(mappedSysBufferTexture.pData) );
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
		throw CHILI_GFX_EXCEPTION( hr,L"Presenting back buffer" );
	}
}

void Graphics::BeginFrame()
{
	sysBuffer.Clear( Colors::Red );
}


void Graphics::DrawTriangle(const Vec2& point1, const Vec2& point2, const Vec2& point3, Color c)
{
	const Vec2* p1 = &point1;
	const Vec2* p2 = &point2;
	const Vec2* p3 = &point3;

	//by design.. I decided that p1 and p2 should be the long line which makes the triangle either flat bottom or flat top
	if (p2->y == p3->y)
		std::swap(p1, p3);
	else if (p1->y == p3->y)
		std::swap(p2, p3);
	
	if (p1->y == p2->y)
	{
		if (p3->y < p1->y)
			drawFlatBottomTriangle(*p1, *p3, *p2, c); //the design of drawFlatBottomTriangle states that p1->y and p3->y are the same.. with p2 either to the top or bottom 
		else if (p3->y > p1->y)
			drawFlatTopTriangle(*p1, *p3, *p2, c); //the design of drawFlatTopTriangle states that p1->y and p3->y are the same.. with p2 either to the top or bottom 
	}
	else
	{
		//standrad triangle..
		float dist12 = abs(p1->y - p2->y);
		float dist23 = abs(p2->y - p3->y);
		float dist31 = abs(p3->y - p1->y);

		//by design.. I decided that p1 and p2 should be the long line which needs to be interrupted..
		if ((dist23 > dist12) && (dist23 > dist31))
			std::swap(p1, p3);

		//by design.. I decided that p1 and p2 should be the long line which needs to be interrupted..
		if ((dist31 > dist12) && (dist31 > dist23))
			std::swap(p2, p3);
		
		Vec2 splitPoint = { -99.0f,p3->y };
		splitPoint.x = p1->x + (((splitPoint.y - p1->y) / (p2->y - p1->y)) * (p2->x - p1->x)); // this formulae was copied from the internet..

		if (p1->y > p2->y)
			std::swap(p1, p2);

		if(p2->x > splitPoint.x)
		{
			drawFlatBottomTriangle(*p3,*p1,splitPoint,c);
			drawFlatTopTriangle(*p3, *p2, splitPoint, c);
		}
		else
		{
			drawFlatBottomTriangle(splitPoint, *p1, *p3, c);
			drawFlatTopTriangle(splitPoint, *p2 ,*p3, c);
		}
		
	}



	//if its  a normal triangle.. then check the point of split draw both a flat bottom and flat top after accounting for left and right scew
}
void Graphics::drawFlatBottomTriangle(const Vec2& point1, const Vec2& point2, const Vec2& point3, const Color& c)
{
	const Vec2* p1 = &point1;
	const Vec2* p2 = &point2;
	const Vec2* p3 = &point3;

	//this is not working for a completely flat top triangle for some reason
	assert(p1->y == p3->y); 

	if (p1->x > p3->x)
		std::swap(p1, p3);

	float m1Inv = ((p2->x - p1->x) / (p2->y - p1->y));
	float m3Inv = ((p2->x - p3->x) / (p2->y - p3->y));

	//the flat bottom triangle cannot draw the top edge as the top left rule states :- A top edge, is an edge that is exactly horizontal and is above the other edges.
	float yEnd = ceil(p1->y - 0.5f);
	float yStart = ceil(p2->y - 0.5f);

	for (float i = p2->y; i < p1->y; i = i+ 1.0f)
	{
		float xStart = (m1Inv * (i - p1->y)) + p1->x;
		float xEnd = (m3Inv * (i - p3->y)) + p3->x;

		//this snippet is included to follow the top left rule..
		//if the centre of the pixel lies inside or on the left line.. then okay draw it else noo..
		xStart = floor(xStart - 0.5f); //any decimal value greater than 0.5 means the pixel should not be drawn right.. so ceil will basically avoid that pixel..
		xEnd = ceilf(xEnd - 0.5f); //the final edge must have the centre within itself or it cannot draw the pixel.. the ceilf ensures that..

		for (float j = xStart; j < xEnd; j = j + 1.0f)
		{
			PutPixel(j, i, c);
		}
	}
}
void Graphics::drawFlatTopTriangle(const Vec2& point1, const Vec2& point2, const Vec2& point3, const Color& c)
{
	const Vec2* p1 = &point1;
	const Vec2* p2 = &point2;
	const Vec2* p3 = &point3;

	//this is not working for a completely flat top triangle for some reason
	assert(p1->y == p3->y);

	if (p1->x > p3->x)
		std::swap(p1, p3);

	float m1Inv = ((p2->x - p1->x) / (p2->y - p1->y));
	float m3Inv = ((p2->x - p3->x) / (p2->y - p3->y));

	//the flat bottom triangle can draw the top edge as the top left rule states :- A top edge, is an edge that is exactly horizontal and is above the other edges.
	float yEnd = ceil(p2->y - 0.5f);
	float yStart = ceil(p1->y - 0.5f);

	for (float i = p1->y; i < yEnd; i = i + 1.0f)
	{
		float xStart = (m1Inv * (i - p1->y)) + p1->x;
		float xEnd = (m3Inv * (i - p3->y)) + p3->x;

		//this snippet is included to follow the top left rule..
		//if the centre of the pixel lies inside or on the left line.. then okay draw it else noo..
		xStart = floor(xStart - 0.5f); //any decimal value greater than 0.5 means the pixel should not be drawn right.. so ceil will basically avoid that pixel..
		xEnd = ceilf(xEnd - 1.0f); //the final edge must have the centre within itself or it cannot draw the pixel.. the ceilf ensures that..

		for (float j = xStart; j <xEnd ; j = j + 1.0f)
		{
			PutPixel(j, i, c);
		}
	}
}

void Graphics::DrawTriangle(const texturedVertex& point1,const texturedVertex& point2,const texturedVertex& point3,const Surface& texture)
{
	const texturedVertex* p0 = &point1;
	const texturedVertex* p1 = &point2;
	const texturedVertex* p2 = &point3;

	// sorting vertices by y
	if (p0->m_position.y > p1->m_position.y) std::swap(p0, p1);
	if (p1->m_position.y > p2->m_position.y) std::swap(p1, p2);
	if (p0->m_position.y > p1->m_position.y) std::swap(p0, p1);

	///case for p1 and p2 y value equal not handled.. basically.. no natural flat bottom is being handled..
	if (p1->m_position.y == p2->m_position.y)
	{
		//natural flat bottom
		drawTexturedFlatBottomTriangle(*p0, *p1, *p2, texture);
	}
	if (p0->m_position.y == p1->m_position.y)
	{
		if (p2->m_position.y > p0->m_position.y)
		{
			//natural flat bottom
			drawTexturedFlatBottomTriangle(*p2, *p0, *p1, texture);
		}
		else //for now donot handle not a triangle case..
		{
			//natural flat top
			drawTexturedFlatTopTriangle(*p2, *p0, *p1, texture);
		}
	}
	else
	{
		//normal triangle which needs to be split into natural flat bottom and natural flat top

		float alphaSplit = (p1->m_position.y - p0->m_position.y)/(p2->m_position.y - p0->m_position.y);
		texturedVertex splitPoint = p0->interpolateTo(*p2, alphaSplit);

		//draw flat bottom and flat top
		drawTexturedFlatBottomTriangle	(*p0, * p1, splitPoint, texture);
		drawTexturedFlatTopTriangle		(*p2, *p1, splitPoint, texture);
	}
}
void Graphics::drawTexturedFlatBottomTriangle(const texturedVertex& point1, const texturedVertex& point2, const texturedVertex& point3, const Surface& texture)
{
	//p1 and p2 form the straight line always and p0 is to the top of p1 and p2..
	const texturedVertex* p0 = &point1;
	const texturedVertex* p1 = &point2;
	const texturedVertex* p2 = &point3;

	//see that p1 is to the left of p2 always.
	if (p1->m_position.x > p2->m_position.x)
		std::swap(p0, p1);

	float w12 = (p1->m_position.x - p0->m_position.x) / (p1->m_position.y - p0->m_position.y); //inverse slope
	float w13 = (p2->m_position.x - p0->m_position.x) / (p2->m_position.y - p0->m_position.y); //inverse slope

	float yStart = p0->m_position.y;
	float yEnd	 = p1->m_position.y;

	for (int y = yStart; y < yEnd; y++)
	{
		float xStart = (y * w12) + p0->m_position.x;
		float xEnd = (y * w13) + p0->m_position.x;

		for (int x = xStart; x < xEnd; x++)
		{
			PutPixel(x, y, Colors::Red);
		}
	}
}
void Graphics::drawTexturedFlatTopTriangle(const texturedVertex& point1, const texturedVertex& point2, const texturedVertex& point3, const Surface& texture)
{}


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

void Graphics::DrawLine( float x1,float y1,float x2,float y2,Color c )
{
	const float dx = x2 - x1;
	const float dy = y2 - y1;

	if( dy == 0.0f && dx == 0.0f )
	{
		PutPixel( int( x1 ),int( y1 ),c );
	}
	else if( abs( dy ) > abs( dx ) )
	{
		if( dy < 0.0f )
		{
			std::swap( x1,x2 );
			std::swap( y1,y2 );
		}

		const float m = dx / dy;
		float y = y1;
		int lastIntY;
		for( float x = x1; y < y2; y += 1.0f,x += m )
		{
			lastIntY = int( y );
			PutPixel( int( x ),lastIntY,c );
		}
		if( int( y2 ) > lastIntY )
		{
			PutPixel( int( x2 ),int( y2 ),c );
		}
	}
	else
	{
		if( dx < 0.0f )
		{
			std::swap( x1,x2 );
			std::swap( y1,y2 );
		}

		const float m = dy / dx;
		float x = x1;
		int lastIntX;
		for( float y = y1; x < x2; x += 1.0f,y += m )
		{
			lastIntX = int( x );
			PutPixel( lastIntX,int( y ),c );
		}
		if( int( x2 ) > lastIntX )
		{
			PutPixel( int( x2 ),int( y2 ),c );
		}
	}
}