#include "Forge.h"


std::vector<Forge*> Forge::Forges;
Forge::Forge(BWAPI::Unit u) : StarcraftUnit(u)
{
}


Forge::~Forge()
{
}

void Forge::Update()
{
	if (!m_unit->exists())
	{
		Forge::Forges.erase(std::find(Forge::Forges.begin(), Forge::Forges.end(), this));
		return;
	}
}