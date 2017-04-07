#pragma once
#include <BWAPI.h>
#include <BWAPI/Client.h>
#include <../ExampleAIModule/StarcraftUnit.h>

class SupplyBuilder : public StarcraftUnit
{
public:
	SupplyBuilder(BWAPI::Unit u);
	~SupplyBuilder();
	static std::vector<SupplyBuilder*> SupplyBuilder::SupplyBuilders;

	void Update();

};

