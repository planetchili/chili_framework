// gdiplus needs a lot of dumb windows shit
// enable that shit for this translation unit only#define FULL_WINTARD
#define FULL_WINTARD
#include "ChiliWin.h"
#include "GDIPlusManager.h"
// gdiplus needs min/max, but we disable that (even in
// full wintard mode), so we need to inject min/max into
// the Gdiplus namespace
#include <algorithm>
namespace Gdiplus
{
	using std::min;
	using std::max;
}
#include <gdiplus.h>

// we need to link to the gdip library somewhere
// might as well be here
#pragma comment( lib,"gdiplus.lib" )

ULONG_PTR GDIPlusManager::token = 0;
int GDIPlusManager::refCount = 0;

GDIPlusManager::GDIPlusManager()
{
	if (refCount++ == 0)
	{
		Gdiplus::GdiplusStartupInput input;
		input.GdiplusVersion = 1;
		input.DebugEventCallback = nullptr;
		input.SuppressBackgroundThread = false;
		Gdiplus::GdiplusStartup(&token, &input, nullptr);
	}
}

GDIPlusManager::~GDIPlusManager()
{
	if (--refCount == 0)
	{
		Gdiplus::GdiplusShutdown(token);
	}
}
