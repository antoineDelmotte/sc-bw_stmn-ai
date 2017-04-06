#pragma once
#include <BWAPI.h>
#include <BWAPI/Client.h>
#include <../ExampleAIModule/StarcraftUnit.h>

struct MasterOrder
{
public:
	MasterOrder(BWAPI::Orders::Enum::Enum type, BWAPI::Position position);

	BWAPI::Orders::Enum::Enum m_type;
	BWAPI::Position m_position;

};


class Master
{
public:
	static std::vector<MasterOrder> Orders;
	static bool FindOrder(BWAPI::Orders::Enum::Enum order, MasterOrder* findOrder);

	Master();

	void Update();
	~Master();
};


