#pragma once
#include "rtscom.h"
class CTest2 :
	public IStylusSyncPlugin
{
public:
	CTest2(void);
	~CTest2(void);
	int testfunc(int a);
};

