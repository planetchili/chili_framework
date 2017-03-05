#pragma once

#include "ChiliWin.h"

class COMInitializer
{
public:
	COMInitializer();
	~COMInitializer();
private:
	HRESULT hr;
};