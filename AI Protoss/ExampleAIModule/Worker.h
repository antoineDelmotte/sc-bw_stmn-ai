#pragma once
#include <../BehaviorTree/bt.hpp>
#include <BWAPI.h>
#include <BWAPI/Client.h>



class Worker
{
public:
	static std::list<Worker*> Workers;
	Worker(BWAPI::Unit u);

	void Update();
	~Worker();

private :
	BWAPI::Unit m_unit;
	bool isExploring;
	//bt::BehaviorTree behaviorTree;
};

