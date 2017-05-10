#pragma once
#include "StarcraftUnit.h"
#include <BWAPI.h>
#include "Master.h"
#include <BWAPI/Client.h>

class Gateway :
	public StarcraftUnit
{
public:
	static std::vector<Gateway*> Gateway::Gateways;
	Gateway(BWAPI::Unit u);
	~Gateway();
	void Update();
};

