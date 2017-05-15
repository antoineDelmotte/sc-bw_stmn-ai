#include "Assimilator.h"
#include <BWAPI.h>
#include "Master.h"

std::vector<Assimilator*> Assimilator::Assimilators;
Assimilator::Assimilator(BWAPI::Unit u) : StarcraftUnit(u)
{
}


Assimilator::~Assimilator()
{
}
void Assimilator::Update()
{
	if (m_unit->isBeingConstructed())
		return;


	if (!m_unit->exists())
	{
		Assimilator::Assimilators.erase(std::find(Assimilator::Assimilators.begin(), Assimilator::Assimilators.end(), this));
	}
}
