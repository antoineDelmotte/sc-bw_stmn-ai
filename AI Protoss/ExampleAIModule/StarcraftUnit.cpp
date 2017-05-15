#include "StarcraftUnit.h"


StarcraftUnit::StarcraftUnit(BWAPI::Unit u)
{
	m_unit = u;
	bool isScouting = false;
	lastOrder = NULL;
}


StarcraftUnit::~StarcraftUnit()
{
}

