#include "Master.h"





std::vector<MasterOrder> Master::Orders;
//Find order and delete it from the list
bool Master::FindOrder(BWAPI::Orders::Enum::Enum type, MasterOrder* findOrder)
{
	for (int i =0; i < Orders.size(); i++)
	{
		if (Orders[i].m_type == type)
		{
			findOrder = &Orders[i];
			Orders.erase(Orders.begin() + i);
			return true;
		}
	}

	return false;
}

Master::Master()
{
	Orders.push_back(MasterOrder(BWAPI::Orders::Enum::Enum::AIPatrol, BWAPI::Position(rand() % 5000, rand() % 5000)));
}

void Master::Update()
{

}

Master::~Master()
{
}

MasterOrder::MasterOrder(BWAPI::Orders::Enum::Enum type, BWAPI::Position position)
{
	m_type = type;
	m_position = position;
}

