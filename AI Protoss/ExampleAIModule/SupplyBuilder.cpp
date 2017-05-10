#include "SupplyBuilder.h"
#include <BWAPI.h>
#include "Master.h"

using namespace BWAPI;
using namespace Filter;

std::vector<SupplyBuilder*> SupplyBuilder::SupplyBuilders;
SupplyBuilder::SupplyBuilder(BWAPI::Unit u) : StarcraftUnit(u)
{
}


SupplyBuilder::~SupplyBuilder()
{
}


void SupplyBuilder::Update()
{
	// Order the depot to construct more workers! But only when it is idle.

	if (m_unit->isIdle() && !m_unit->isTraining()) 	// if our nexus is idle
	{
		for (MasterOrder* m : Master::FindOrders(BWAPI::Orders::Enum::Enum::Train))
		{
			if ( ((TrainOrder*)m)->m_unitType == BWAPI::UnitTypes::Protoss_Probe)
			{
				//Broodwar->sendText("Building peon !");
				m_unit->train(((TrainOrder*)m)->m_unitType);
				Master::TakeOrder(m);
				break;
			}
		}
	}


	/*
	if (m_unit->isIdle() && !m_unit->train(m_unit->getType().getRace().getWorker()))
	{
		// If that fails, draw the error at the location so that you can visibly see what went wrong!
		// However, drawing the error once will only appear for a single frame
		// so create an event that keeps it on the screen for some frames
		Position pos = m_unit->getPosition();
		Error lastErr = Broodwar->getLastError();
		Broodwar->registerEvent([pos, lastErr](Game*) { Broodwar->drawTextMap(pos, "%c%s", Text::White, lastErr.c_str()); },   // action
			nullptr,    // condition
			Broodwar->getLatencyFrames());  // frames to run

											// Retrieve the supply provider type in the case that we have run out of supplies
		UnitType supplyProviderType = m_unit->getType().getRace().getSupplyProvider();
		static int lastChecked = 0;

		// If we are supply blocked and haven't tried constructing more recently
		if (lastErr == Errors::Insufficient_Supply &&
			lastChecked + 400 < Broodwar->getFrameCount() &&
			Broodwar->self()->incompleteUnitCount(supplyProviderType) == 0)
		{
			lastChecked = Broodwar->getFrameCount();

			// Retrieve a unit that is capable of constructing the supply needed
			Unit supplyBuilder = m_unit->getClosestUnit(GetType == supplyProviderType.whatBuilds().first &&
				(IsIdle || IsGatheringMinerals) &&
				IsOwned);
			// If a unit was found
			if (supplyBuilder)
			{
				if (supplyProviderType.isBuilding())
				{
					TilePosition targetBuildLocation = Broodwar->getBuildLocation(supplyProviderType, supplyBuilder->getTilePosition());
					if (targetBuildLocation)
					{
						// Register an event that draws the target build location
						Broodwar->registerEvent([targetBuildLocation, supplyProviderType](Game*)
						{
							Broodwar->drawBoxMap(Position(targetBuildLocation),
								Position(targetBuildLocation + supplyProviderType.tileSize()),
								Colors::Blue);
						},
							nullptr,  // condition
							supplyProviderType.buildTime() + 100);  // frames to run

																	// Order the builder to construct the supply structure
						supplyBuilder->build(supplyProviderType, targetBuildLocation);
					}
				}
				else
				{
					// Train the supply provider (Overlord) if the provider is not a structure
					supplyBuilder->train(supplyProviderType);
				}
			} // closure: supplyBuilder is valid
		} // closure: insufficient supply
	} // closure: failed to train idle unit

	*/
}