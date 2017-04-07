#pragma once
#include <../BehaviorTree/bt.hpp>
#include <BWAPI.h>
#include <BWAPI/Client.h>
#include <../ExampleAIModule/StarcraftUnit.h>



class Worker : public StarcraftUnit
{
public:
	static std::vector<Worker*> Workers;
	Worker(BWAPI::Unit u);

    void Update();
	~Worker();

	//bt::BehaviorTree behaviorTree;
};

