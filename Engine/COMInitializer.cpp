#include "COMInitializer.h"
#include <objbase.h>

COMInitializer::COMInitializer()
{
	hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
}

COMInitializer::~COMInitializer()
{
	if (hr == S_OK)
	{
		CoUninitialize();
	}
}
