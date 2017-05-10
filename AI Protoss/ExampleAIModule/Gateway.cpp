#include "Gateway.h"
using namespace BWAPI;
using namespace Filter;

std::vector<Gateway*> Gateway::Gateways;

Gateway::Gateway(BWAPI::Unit u) : StarcraftUnit(u)
{
}


Gateway::~Gateway()
{
}

void Gateway::Update()
{
	MasterOrder* masterOrder;

	if ( (masterOrder = Master::FindOrder(BWAPI::Orders::Enum::Enum::Train)) != NULL)
	{
		if (m_unit->isIdle() && ((TrainOrder*)masterOrder)->m_unitType == UnitTypes::Protoss_Zealot)
		{
			m_unit->train(UnitTypes::Protoss_Zealot);
			Master::TakeOrder(masterOrder);
		}
	}
}
