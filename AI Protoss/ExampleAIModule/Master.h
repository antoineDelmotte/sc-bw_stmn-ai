#pragma once
#include <BWAPI.h>
#include <BWAPI/Client.h>

struct MasterOrder
{
public:
	MasterOrder(BWAPI::Orders::Enum::Enum type, BWAPI::Position position);

	BWAPI::Orders::Enum::Enum m_type;
	BWAPI::Position m_position;
	int index;
};


struct BuildOrder : public MasterOrder
{
	BuildOrder(BWAPI::Orders::Enum::Enum type, BWAPI::Position position, BWAPI::UnitType unitType);

	BWAPI::UnitType m_unitType;
};


static class Master
{
public:

	static std::vector<MasterOrder*> Orders;
	static std::set<BWAPI::Position> allStartLocations;	// All the starting positions
	static std::set<BWAPI::Position> enemyStartLocations;	// Starting positions occupied by enemies
	static std::set<BWAPI::Position> otherStartLocations;	// Starting positions that are not occupied by me (but maybe by enemies)
	static std::set<BWAPI::Position> Master::enemyLocations;

	static BWAPI::Position personalStartLocation;		// My start position

	static MasterOrder* FindOrder(BWAPI::Orders::Enum::Enum order);
	static void fillStartingLocations();
	static bool IsTilePositionValid(const BWAPI::TilePosition tilePosition);
	static bool IsPositionValid(const BWAPI::Position position);
	static BWAPI::Position ConvertTilePosition(const BWAPI::TilePosition tilePosition, const BWAPI::UnitType unitType);
	//static BWAPI::TilePosition ConvertPosition(const BWAPI::Position tilePosition, const BWAPI::UnitType unitType);
	static void TakeOrder(MasterOrder* order);
	static void AddOrder(MasterOrder* order);
	static void InformEnemyBaseLocation(BWAPI::Position position);


	static int waitPylonCount;
	static int waitGatewayCount;

	static void Init();
	static void Update();
};


