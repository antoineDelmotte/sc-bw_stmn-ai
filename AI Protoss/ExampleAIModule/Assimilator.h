#pragma once
#include "StarcraftUnit.h"
class Assimilator :
	public StarcraftUnit
{
public:
	static std::vector<Assimilator*> Assimilators;
	Assimilator(BWAPI::Unit u);
	void Update();
	~Assimilator();
};

