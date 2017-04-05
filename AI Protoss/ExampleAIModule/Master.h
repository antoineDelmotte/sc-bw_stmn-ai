#pragma once
#include <BWAPI.h>
#include <BWAPI/Client.h>

class Master
{
public:
	enum ORDER { GET_MINERAL, EXPLORE };
	static std::list<ORDER> Orders;
	static bool FindOrder(Master::ORDER order);

	Master();

	void Update();
	~Master();
};

