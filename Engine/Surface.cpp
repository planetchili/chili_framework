#include "Surface.h"
// gdiplus needs a lot of dumb windows shit
// enable that shit for this translation unit only
#define FULL_WINTARD
#include "ChiliWin.h"
#include <algorithm>
// gdiplus needs min/max, but we disable that (even in
// full wintard mode), so we need to inject min/max into
// the Gdiplus namespace
namespace Gdiplus
{
	using std::min;
	using std::max;
}
#include <gdiplus.h>
#include <cassert>
#include <fstream>

namespace gdi = Gdiplus;

Surface::Surface(const std::wstring& filename)
{
	// filename must be at least 4 chars long
	if (filename.length() < 4)
	{
		// generate narrow string of filename
		std::string narrow(filename.begin(), filename.end());
		throw std::runtime_error("Surface::Surface bad file name: " + narrow);
	}

	// open image file with gdiplus (not only .bmp files)
	gdi::Bitmap bitmap(filename.c_str());

	// check if file loaded successfully, throw exception if didn't
	if (bitmap.GetLastStatus() != gdi::Ok)
	{
		// generate narrow string of filename
		std::string narrow(filename.begin(), filename.end());
		// throw exception with error message
		// could possibly add more info with lookup of error code name / desc
		// but I don't want to right now cuz im lazy
		throw std::runtime_error("Surface::Surface failed to load file: " + narrow);
	}

	// allocate Surface resources and set dimensions
	width = bitmap.GetWidth();
	height = bitmap.GetHeight();
	pPixels = new Color[width * height];

	// test if pixel format is alpha, and save result
	const bool isAlpha = gdi::IsAlphaPixelFormat(bitmap.GetPixelFormat()) == TRUE;

	// loop through image dimensions, copy from gdip bitmap to surface
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// need this to receive color of pixel
			gdi::Color pixel;
			// read color from gdip bitmap
			bitmap.GetPixel(x, y, &pixel);
			// write to surface (with alpha channel if exists)
			if (isAlpha)
			{
				PutPixel(x, y, { pixel.GetA(),pixel.GetR(),pixel.GetG(),pixel.GetB() });
			}
			else
			{
				PutPixel(x, y, { pixel.GetR(),pixel.GetG(),pixel.GetB() });
			}
		}
	}

	// check to see whether filename starts with "pm_"
	// (actually, being lazy so only checking if contains "pm_")
	// if so, gotta bake that alpha yo
	if (filename.find(L"pm_") != std::wstring::npos)
	{
		BakeAlpha();
	}
}

Surface::Surface(int width, int height)
	:
	width(width),
	height(height),
	pPixels(new Color[width*height])
{
}

Surface::Surface(const Surface& rhs)
	:
	Surface(rhs.width, rhs.height)
{
	const int nPixels = width * height;
	for (int i = 0; i < nPixels; i++)
	{
		pPixels[i] = rhs.pPixels[i];
	}
}

Surface::~Surface()
{
	delete[] pPixels;
	pPixels = nullptr;
}

Surface& Surface::operator=(const Surface& rhs)
{
	// prevent self assignment
	if (this != &rhs)
	{
		width = rhs.width;
		height = rhs.height;

		delete[] pPixels;
		pPixels = new Color[width*height];

		const int nPixels = width * height;
		for (int i = 0; i < nPixels; i++)
		{
			pPixels[i] = rhs.pPixels[i];
		}
	}
	return *this;
}

void Surface::PutPixel(int x, int y, Color c)
{
	assert(x >= 0);
	assert(x < width);
	assert(y >= 0);
	assert(y < height);
	pPixels[y * width + x] = c;
}

Color Surface::GetPixel(int x, int y) const
{
	assert(x >= 0);
	assert(x < width);
	assert(y >= 0);
	assert(y < height);
	return pPixels[y * width + x];
}

int Surface::GetWidth() const
{
	return width;
}

int Surface::GetHeight() const
{
	return height;
}

RectI Surface::GetRect() const
{
	return{ 0,width,0,height };
}

void Surface::BakeAlpha()
{
	const int nPixels = GetWidth() * GetHeight();
	for (int i = 0; i < nPixels; i++)
	{
		auto pix = pPixels[i];
		const int alpha = pix.GetA();
		// premulitply alpha time each channel
		pix.SetR((pix.GetR() * alpha) / 256);
		pix.SetG((pix.GetG() * alpha) / 256);
		pix.SetB((pix.GetB() * alpha) / 256);
		// write back to surface
		pPixels[i] = pix;
	}
}