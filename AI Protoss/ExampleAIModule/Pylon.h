#pragma once

#include <BWAPI.h>
#include <BWAPI/Client.h>
#include <../ExampleAIModule/StarcraftUnit.h>

class Pylon : public StarcraftUnit
{
public:
	static std::vector<Pylon*> Pylons;
	Pylon(BWAPI::Unit u);
	~Pylon();

	void Update();
};

