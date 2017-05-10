#pragma once
#include "StarcraftUnit.h"
class Zealott :
	public StarcraftUnit
{
public:
	Zealott(BWAPI::Unit u);
	~Zealott();
	static std::vector<Zealott*> Zealotts;
	void Update();
};

