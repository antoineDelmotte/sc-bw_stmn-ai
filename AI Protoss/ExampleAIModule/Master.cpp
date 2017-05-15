#include <BWAPI.h>
#include "Master.h"
#include "SupplyBuilder.h"
#include "Worker.h"
#include "Pylon.h"
#include "Gateway.h"
#include "Zealott.h"
#include "Assimilator.h"

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
int Master::waitAssimilatorCount = 0;

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

std::vector<MasterOrder*> Master::FindOrders(BWAPI::Orders::Enum::Enum type)
{
	std::vector<MasterOrder*> orders;

	for (int i = 0; i < Orders.size(); i++)
	{
		if (Orders[i]->m_type == type)
		{
			 orders.push_back(Orders[i]);
		}
	}

	return orders;
}

void Master::Init()
{
	fillStartingLocations();
}


void Master::TakeOrder(MasterOrder* order)
{
	Orders.erase(std::find(Orders.begin(), Orders.end() , order));
}

void Master::AddOrder(MasterOrder* order)
{
	Orders.push_back(order);
}

void Master::InformEnemyBaseLocation(BWAPI::Position position)
{
	enemyLocations.insert(position);
}


int enemyStartLocationsChecked = 2;
void Master::Update()
{
	fillStartingLocations();
	bool needScouting = enemyStartLocations.size() == 0 ? true : false;
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

	if (waitAssimilatorCount > 0 || waitGatewayCount > 0 || waitPylonCount > 0)
		return;


	if (Worker::Workers.size() < 10 && FindOrders(BWAPI::Orders::Enum::Enum::Train).size() < 5)
	{
		AddOrder(new TrainOrder(BWAPI::Orders::Enum::Enum::Train, BWAPI::Position(0,0), UnitTypes::Protoss_Probe));
		return;
	}


	if (Worker::Workers.size() > 0 && SupplyBuilder::SupplyBuilders.size() > 0)
	{
		if ((Pylon::Pylons.size() == 0 ||  Broodwar->self()->supplyTotal() - Broodwar->self()->supplyUsed() < 2) && (Master::canIBuildThisUnit(UnitTypes::Protoss_Pylon)))
		{
			Broodwar->sendText("PYLON ORDER");
			AddOrder(new BuildOrder(BWAPI::Orders::Enum::Enum::PlaceBuilding, Position(Broodwar->getBuildLocation(UnitTypes::Protoss_Pylon, SupplyBuilder::SupplyBuilders[0]->m_unit->getTilePosition())), UnitTypes::Protoss_Pylon));
			waitPylonCount++;
		}
		else if ( Gateway::Gateways.size() <= 1 &&  Pylon::Pylons.size() > 0 && Pylon::Pylons[0]->m_unit->isCompleted() && (Master::canIBuildThisUnit(UnitTypes::Protoss_Gateway)))
		{	
			Broodwar->sendText("GATEWAY ORDER");
			AddOrder(new BuildOrder(BWAPI::Orders::Enum::Enum::PlaceBuilding, Position(Broodwar->getBuildLocation(UnitTypes::Protoss_Gateway, Pylon::Pylons[0]->m_unit->getTilePosition())), UnitTypes::Protoss_Gateway));
			waitGatewayCount ++;
		}
		/*else if (Assimilator::Assimilators.size() == 0 && Master::canIBuildThisUnit(UnitTypes::Protoss_Assimilator))
		{
			Broodwar->sendText("ASSIMILATOR ORDER");
			AddOrder(new BuildOrder(BWAPI::Orders::Enum::Enum::PlaceBuilding, Position(Broodwar->getBuildLocation(UnitTypes::Protoss_Assimilator, SupplyBuilder::SupplyBuilders[0]->m_unit->getTilePosition())), UnitTypes::Protoss_Assimilator));
			waitAssimilatorCount++;
		}*/
	}


	if (Broodwar->self()->supplyTotal() > Broodwar->self()->supplyUsed() + UnitTypes::Protoss_Zealot.supplyRequired())
	{
		if (Gateway::Gateways.size() > 0 && Zealott::Zealotts.size() < 20)
		{
			bool trainZealot = false;
			for (MasterOrder* to : FindOrders(BWAPI::Orders::Enum::Enum::Train))
			{
				if (((TrainOrder*)to)->m_unitType == UnitTypes::Protoss_Zealot)
				{
					trainZealot = true;
					break;
				}
			}

			if (!trainZealot && (Master::canIBuildThisUnit(UnitTypes::Protoss_Zealot)))
			{
				Broodwar->sendText("ZEALOT ORDER");
				AddOrder(new TrainOrder(BWAPI::Orders::Enum::Enum::Train, (*Gateway::Gateways.begin())->m_unit->getPosition(), UnitTypes::Protoss_Zealot));
			}
		}
	}
	else if (Worker::Workers.size() > 0)
	{
		if (waitPylonCount == 0)
		{
			if (Master::canIBuildThisUnit(UnitTypes::Protoss_Pylon))
			{
				Broodwar->sendText("PYLON ORDER");
				AddOrder(new BuildOrder(BWAPI::Orders::Enum::Enum::PlaceBuilding, Position(Broodwar->getBuildLocation(UnitTypes::Protoss_Pylon, SupplyBuilder::SupplyBuilders[0]->m_unit->getTilePosition())), UnitTypes::Protoss_Pylon));
				waitPylonCount++;
			}
		}
	}

	//A L ATTAQUE !!! CHARGEZ !!!!!
	if (Zealott::Zealotts.size() >= 20 && enemyLocations.size() > 0)
	{
		for(int i =0; i < 20 ; i++)
			AddOrder(new AttackOrder(BWAPI::Orders::Enum::Enum::AttackMove, *enemyStartLocations.begin(), UnitTypes::Protoss_Zealot));
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

TrainOrder::TrainOrder(BWAPI::Orders::Enum::Enum type, BWAPI::Position position, UnitType unitType) : MasterOrder(type, position)
{
	m_unitType = unitType;
}

AttackOrder::AttackOrder(BWAPI::Orders::Enum::Enum type, BWAPI::Position position, BWAPI::UnitType unitType) : MasterOrder(type, position)
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


bool Master::canIBuildThisUnit(UnitType unit) {
	bool costSupply = (unit.isBuilding() && Worker::Workers.size()>0) ? true : Broodwar->self()->supplyUsed() + unit.supplyRequired() < Broodwar->self()->supplyTotal() ? true : false;
	return costSupply && Broodwar->self()->minerals() >= unit.mineralPrice() && Broodwar->self()->gas() >= unit.gasPrice();
}