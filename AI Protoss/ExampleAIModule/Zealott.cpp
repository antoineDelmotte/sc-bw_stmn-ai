#include "Zealott.h"


std::vector<Zealott*> Zealott::Zealotts;

Zealott::Zealott(BWAPI::Unit u) : StarcraftUnit(u)
{
}


Zealott::~Zealott()
{
}

void Zealott::Update()
{
	if (!m_unit->exists())
	{
		Zealott::Zealotts.erase(std::find(Zealott::Zealotts.begin(), Zealott::Zealotts.end(), this));
		return;
	}

	if (m_unit->isIdle() && lastOrder == NULL)
	{
		MasterOrder* masterOrder = Master::FindOrder(BWAPI::Orders::Enum::Enum::AttackMove);

		if (masterOrder != NULL)
		{
			m_unit->attack(((AttackOrder*)masterOrder)->m_position);
			Master::TakeOrder(masterOrder);
			lastOrder = masterOrder;
		}
	}

}
