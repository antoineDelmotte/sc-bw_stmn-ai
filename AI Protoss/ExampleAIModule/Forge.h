#pragma once
#include "StarcraftUnit.h"
class Forge :
	public StarcraftUnit
{
public:
	static std::vector<Forge*> Forges;
	Forge(BWAPI::Unit u);
	~Forge();

	void Update();
};

