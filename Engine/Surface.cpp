#include "Surface.h"
#include "ChiliWin.h"
#include <cassert>
#include <fstream>


Surface::Surface(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary);

	assert(file);

	BITMAPFILEHEADER bmFileHeader;
	file.read(reinterpret_cast<char*>(&bmFileHeader), sizeof(bmFileHeader));

	BITMAPINFOHEADER bmInfoHeader;
	file.read(reinterpret_cast<char*>(&bmInfoHeader), sizeof(bmInfoHeader));

	assert(bmInfoHeader.biBitCount == 24 || bmInfoHeader.biBitCount == 32); // only 24 bit bitmaps (rgb)
	assert(bmInfoHeader.biCompression == BI_RGB); // no compression

	const bool is32b = bmInfoHeader.biBitCount == 32;

	width = bmInfoHeader.biWidth;

	int yStart;
	int yEnd;
	int dy;

	// check negative height and set iteration parameters accordingly
	if (bmInfoHeader.biHeight < 0)
	{
		height = -bmInfoHeader.biHeight;
		yStart = 0;
		yEnd = height;
		dy = 1;
	}
	else
	{
		height = bmInfoHeader.biHeight;
		yStart = height - 1;
		yEnd = -1;
		dy = -1;
	}

	pPixels = new Color[height * width];

	// load data from the start
	file.seekg(bmFileHeader.bfOffBits);
	const int bmpPadding = (4 - (width * 3) % 4) % 4;

	// bmp starts from the bottom
	for (int y = yStart; y != yEnd; y += dy)
	{
		for (int x = 0; x < width; ++x)
		{
			PutPixel(x, y, Color(file.get(), file.get(), file.get()));
			if (is32b)
			{
				file.seekg(1, std::ios::cur);
			}
		}

		if (!is32b) // no need for padding for a 32bit image
		{
			// seek forward padding
			file.seekg(bmpPadding, std::ios::cur);
		}
	}
}

Surface::Surface(int width, int height)
	:
	width(width),
	height(height),
	pPixels(new Color[width * height])
{
}

Surface::Surface(const Surface& rhs)
	:
	Surface(rhs.width, rhs.height)

{
	// deep copy pixels
	const int nPixels = width * height;
	for (int i = 0; i < nPixels; ++i)
	{
		pPixels[i] = rhs.pPixels[i];
	}
}

Surface& Surface::operator=(const Surface& rhs)
{
	width = rhs.width;
	height = rhs.height;

	delete[] pPixels;
	pPixels = new Color[width * height];

	// deep copy pixels
	const int nPixels = width * height;
	for (int i = 0; i < nPixels; ++i)
	{
		pPixels[i] = rhs.pPixels[i];
	}

	return *this;
}

Surface::~Surface()
{
	delete[] pPixels;
	pPixels = nullptr;
}

void Surface::PutPixel(int x, int y, Color c) const
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
	return { 0, width, 0, height };
}
