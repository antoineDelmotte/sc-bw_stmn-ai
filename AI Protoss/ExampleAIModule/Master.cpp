#include <BWAPI.h>
#include "Master.h"
#include "SupplyBuilder.h"
#include "Worker.h"
#include "Pylon.h"

using namespace BWAPI;
using namespace Filter;

std::set<BWAPI::Position> Master::allStartLocations;	// All the starting positions
std::set<BWAPI::Position> Master::enemyStartLocations;	// Starting positions occupied by enemies

std::set<BWAPI::Position> Master::otherStartLocations;	// Starting positions that are not occupied by me (but maybe by enemies)
BWAPI::Position Master::personalStartLocation;		// My start position

std::set<BWAPI::Position> Master::enemyLocations;

std::vector<MasterOrder*> Master::Orders;

int Master::waitPylonCount = 0;
int Master::waitGatewayCount = 0;

//Find order and delete it from the list
MasterOrder* Master::FindOrder(BWAPI::Orders::Enum::Enum type)
{
	for (int i =0; i < Orders.size(); i++)
	{
		if (Orders[i]->m_type == type)
		{
			return Orders[i];
		}
	}

	return NULL;
}


void Master::Init()
{
	fillStartingLocations();
}


void Master::TakeOrder(MasterOrder* order)
{
	Orders.erase(Orders.begin() + order->index);
}

void Master::AddOrder(MasterOrder* order)
{
	Orders.push_back(order);
	order->index = Orders.size() - 1;
}

void Master::InformEnemyBaseLocation(BWAPI::Position position)
{
	enemyLocations.insert(position);
}


int enemyStartLocationsChecked = 2;
void Master::Update()
{
	fillStartingLocations();
	bool needScouting = true;
	for (Worker* w : Worker::Workers)
	{
		if (w->isScouting)
		{
			needScouting = false;
			break;
		};
	}

	if (needScouting && Master::FindOrder(BWAPI::Orders::Enum::Enum::AIPatrol) == NULL)
	{
		std::set<Position>::iterator it = otherStartLocations.begin();
		std::advance(it, enemyStartLocationsChecked);

		AddOrder(new MasterOrder(BWAPI::Orders::Enum::Enum::AIPatrol, *it));
		enemyStartLocationsChecked = (enemyStartLocationsChecked+1)% (otherStartLocations.size());
		Broodwar->sendText("SCOUT ORDER : ");
		Broodwar->sendText((std::to_string(enemyStartLocationsChecked)).c_str());
	}


	if (Worker::Workers.size() > 0 && SupplyBuilder::SupplyBuilders.size() > 0)
	{
		if (waitPylonCount == 0 && Pylon::Pylons.size() == 0)
		{
			if ((Broodwar->self()->gas() >= UnitTypes::Protoss_Pylon.gasPrice() && Broodwar->self()->minerals() >= UnitTypes::Protoss_Pylon.mineralPrice()))
			{
				Broodwar->sendText("PYLON ORDER");
				AddOrder(new BuildOrder(BWAPI::Orders::Enum::Enum::PlaceBuilding, Position(Broodwar->getBuildLocation(UnitTypes::Protoss_Pylon, SupplyBuilder::SupplyBuilders[0]->m_unit->getTilePosition())), UnitTypes::Protoss_Pylon));
				waitPylonCount++;
			}

		}
		else if (waitGatewayCount == 0 && Pylon::Pylons.size() > 0 && Pylon::Pylons[0]->m_unit->isCompleted())
		{	
			if (Broodwar->self()->gas() >= UnitTypes::Protoss_Gateway.gasPrice() && Broodwar->self()->minerals() >= UnitTypes::Protoss_Gateway.mineralPrice())
			{
				Broodwar->sendText("GATEWAY ORDER");
				AddOrder(new BuildOrder(BWAPI::Orders::Enum::Enum::PlaceBuilding, Position(Broodwar->getBuildLocation(UnitTypes::Protoss_Gateway, Pylon::Pylons[0]->m_unit->getTilePosition())), UnitTypes::Protoss_Gateway));
				waitGatewayCount ++;
			}
		}
	}
}


MasterOrder::MasterOrder(BWAPI::Orders::Enum::Enum type, BWAPI::Position position)
{
	m_type = type;
	m_position = position;
}

BuildOrder::BuildOrder(BWAPI::Orders::Enum::Enum type, BWAPI::Position position, UnitType unitType) : MasterOrder(type, position)
{
	m_unitType = unitType;
}


//Use a UnitType combined to its TilePosition to get the Position corresponding to its center
BWAPI::Position Master::ConvertTilePosition(const BWAPI::TilePosition tilePosition, const BWAPI::UnitType unitType)
{
	return
		BWAPI::Position(
			BWAPI::Position(tilePosition) +
			BWAPI::Position(
			(unitType.tileWidth() * BWAPI::TILEPOSITION_SCALE) / 2,
				(unitType.tileHeight() * BWAPI::TILEPOSITION_SCALE) / 2
			)
		);
}

/*
BWAPI::TilePosition Master::ConvertPosition(const BWAPI::Position tilePosition, const BWAPI::UnitType unitType)
{
	return
		BWAPI::TilePosition(
			BWAPI::TilePosition(tilePosition) -
			BWAPI::TilePosition(
			(unitType.tileWidth() * BWAPI::TILEPOSITION_SCALE) / 2,
				(unitType.tileHeight() * BWAPI::TILEPOSITION_SCALE) / 2
			)
		);
}*/

//Tell if the given Position is a valid one (not invalid, unknown or not set)
bool Master::IsPositionValid(const BWAPI::Position position)
{
	return
		position != BWAPI::Positions::Invalid &&
		position != BWAPI::Positions::Unknown &&
		position != BWAPI::Positions::None;
}

//Tell if the given TilePosition is a valid one (not invalid, unknown or not set)
bool Master::IsTilePositionValid(const BWAPI::TilePosition tilePosition)
{
	return
		tilePosition != BWAPI::TilePositions::Invalid &&
		tilePosition != BWAPI::TilePositions::Unknown &&
		tilePosition != BWAPI::TilePositions::None;
}

// Fill all the starting locations data sets
void Master::fillStartingLocations()
{
	// Store all starting locations
	for (const BWAPI::TilePosition tp : BWAPI::Broodwar->getStartLocations())
	{
		allStartLocations.insert(ConvertTilePosition(tp, BWAPI::UnitTypes::Special_Start_Location));

		// Store our starting location
		if (tp == BWAPI::Broodwar->self()->getStartLocation())
		{
			if (!IsPositionValid(personalStartLocation))
			{
				personalStartLocation = ConvertTilePosition(tp, BWAPI::UnitTypes::Special_Start_Location);
				Broodwar->sendText("PERSONNAL POSITION");
			}
		}
		// Store the other starting locations
		else
		{
			otherStartLocations.insert(ConvertTilePosition(tp, BWAPI::UnitTypes::Special_Start_Location));
		}
	}

	// Store enemy starting locations
	for (const BWAPI::Player& p : BWAPI::Broodwar->enemies())
	{
		const BWAPI::TilePosition enemyTilePosition = p->getStartLocation();
		if (IsTilePositionValid(enemyTilePosition))
		{
			const BWAPI::Position enemyPosition = ConvertTilePosition(enemyTilePosition, BWAPI::UnitTypes::Special_Start_Location);
			enemyStartLocations.insert(enemyPosition);
			Broodwar->sendText("ENNEMY POSITION");
		}
	}
}