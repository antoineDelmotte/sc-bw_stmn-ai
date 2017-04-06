#pragma once
#include <BWAPI.h>
#include <BWAPI/Client.h>

class StarcraftUnit
{
public:
	StarcraftUnit(BWAPI::Unit u);
	~StarcraftUnit();

	template <class T>
	bool IsTheNearest(BWAPI::Position position, std::vector<T> starcraftUnits)
	{
		double m_distance = m_unit->getDistance(position);

		for (int i = 0; i <starcraftUnits.size(); i++)
		{
			double dist = starcraftUnits[i]->m_unit->getDistance(position);

			if (dist < m_distance)
				return false;
		}

		return true;
	}

	BWAPI::Unit m_unit;
};

