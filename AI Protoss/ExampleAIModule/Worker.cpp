#include "Worker.h"
#include "../ExampleAIModule/W3Node.h"
#include <BWAPI.h>
#include "Master.h"

using namespace BWAPI;
using namespace Filter;

//Mdr redéclarer ça ici.. merci le c++ !
std::vector<Worker*> Worker::Workers;

Worker::Worker(BWAPI::Unit u) : StarcraftUnit(u)
{
	//LES BEHAVIORTREE SANS MOTEUR GRAPHIQUE POUR LES DEFINIRS SONT TROP COMPLIQUE A IMPLEMENTER

	//DEFINITION
/*	auto &blackboard = behaviorTree.GetBlackboard();
	auto rootSelector = std::make_shared<bt::Selector>();

	auto mineSequence = std::make_shared<bt::Sequence>();

	auto idleCondition_mineSequence = std::make_shared<CheckBoolCondition>(blackboard,"idle");
	auto carryingGasCondition_mineSequence = std::make_shared<CheckBoolCondition>(blackboard, "carryingGas");
	
	//auto carryingGasCondition_mineSequence_inverter = std::make_shared<bt::Inverter>();
	//carryingGasCondition_mineSequence_inverter->SetChild(carryingGasCondition_mineSequence);

	auto carryingMineralCondition_mineSequence = std::make_shared<CheckBoolCondition>(blackboard, "carryingMineral");


	//CONSTRUCTION DE L'ARBRE
	behaviorTree.SetRoot(rootSelector);
	rootSelector->AddChild(mineSequence);

	mineSequence->AddChild(idleCondition_mineSequence);
	mineSequence->AddChild(carryingGasCondition_mineSequence);
	mineSequence->AddChild(carryingMineralCondition_mineSequence);*/
}

void Worker::Update()
{
	//behaviorTree.Update();

	if (!m_unit->exists())
		return;

	// Ignore the unit if it has one of the following status ailments
	if (m_unit->isLockedDown() || m_unit->isMaelstrommed() || m_unit->isStasised())
		return;

	// Ignore the unit if it is in one of the following states
	if (m_unit->isLoaded() || !m_unit->isPowered() || m_unit->isStuck())
		return;

	// Ignore the unit if it is incomplete or busy constructing
	if (!m_unit->isCompleted() || m_unit->isConstructing())
		return;


	MasterOrder* masterOrder;

	// if our worker is idle
	if (m_unit->isIdle())
	{
		// Order workers carrying a resource to return them to the center,
		// otherwise find a mineral patch to harvest.
		if (lastOrder == NULL && (m_unit->isCarryingGas() || m_unit->isCarryingMinerals()))
		{
			m_unit->returnCargo();
		}
		else if ((masterOrder = Master::FindOrder(BWAPI::Orders::Enum::Enum::AIPatrol)) != NULL)
		{
			if (IsTheNearest(masterOrder->m_position, Workers))
			{
				m_unit->move(masterOrder->m_position);
				Master::TakeOrder(masterOrder);
				lastOrder = masterOrder;
			}

			Broodwar << "Patrol needed\n";
		}
		else if ((masterOrder = Master::FindOrder(BWAPI::Orders::Enum::Enum::PlaceBuilding)) != NULL)
		{
			if (IsTheNearest(masterOrder->m_position, Workers))
			{
				m_unit->build(((BuildOrder*)masterOrder)->m_unitType, BWAPI::TilePosition(masterOrder->m_position));
				Master::TakeOrder(masterOrder);
				lastOrder = masterOrder;
			}
		}
		else if (!m_unit->getPowerUp())  // The worker cannot harvest anything if it
		{                             // is carrying a powerup such as a flag
									  // Harvest from the nearest mineral patch or gas refinery
			if (!m_unit->gather(m_unit->getClosestUnit(IsMineralField || IsRefinery)))
			{
				// If the call fails, then print the last error message
				Broodwar << Broodwar->getLastError() << std::endl;
			}

		} // closure: has no powerup
	} // closure: if idle
}

Worker::~Worker()
{
}
