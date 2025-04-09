#pragma once
#include "LoadMode.h"

class SilentMode : public LoadMode
{
public:
	SilentMode() :LoadMode(true) {}
	~SilentMode() {}
	void Start() override
	{
		Run_If_You_Can_Load();
	}
};

