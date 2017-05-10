#include "ExampleAIModule.h"
#include <iostream>
#include "../Worker.h"
#include "../Master.h"
#include "../SupplyBuilder.h"
#include "../Pylon.h"
#include "../SupplyBuilder.h"

using namespace BWAPI;
using namespace Filter;

void ExampleAIModule::onStart()
{
	Master::Init();

	for (auto unit : Broodwar->self()->getUnits())
	{
		if (unit->getType().isWorker())
		{
			Worker::Workers.push_back(new Worker(unit));
		}
		else if (unit->getType().isResourceDepot())
		{
			SupplyBuilder::SupplyBuilders.push_back(new SupplyBuilder(unit));
		}
	}


  // Hello World!
  Broodwar->sendText("Hello world!");

  // Print the map name.
  // BWAPI returns std::string when retrieving a string, don't forget to add .c_str() when printing!
  Broodwar << "The map is " << Broodwar->mapName() << "!" << std::endl;

  // Enable the UserInput flag, which allows us to control the bot and type messages.
  Broodwar->enableFlag(Flag::UserInput);

  // Uncomment the following line and the bot will know about everything through the fog of war (cheat).
  //Broodwar->enableFlag(Flag::CompleteMapInformation);

  // Set the command optimization level so that common commands can be grouped
  // and reduce the bot's APM (Actions Per Minute).
  Broodwar->setCommandOptimizationLevel(2);

  // Check if this is a replay
  if ( Broodwar->isReplay() )
  {

    // Announce the players in the replay
    Broodwar << "The following players are in this replay:" << std::endl;
    
    // Iterate all the players in the game using a std:: iterator
    Playerset players = Broodwar->getPlayers();
    for(auto p : players)
    {
      // Only print the player if they are not an observer
      if ( !p->isObserver() )
        Broodwar << p->getName() << ", playing as " << p->getRace() << std::endl;
    }

  }
  else // if this is not a replay
  {
    // Retrieve you and your enemy's races. enemy() will just return the first enemy.
    // If you wish to deal with multiple enemies then you must use enemies().
    if ( Broodwar->enemy() ) // First make sure there is an enemy
      Broodwar << "The matchup is " << Broodwar->self()->getRace() << " vs " << Broodwar->enemy()->getRace() << std::endl;
  }

}

void ExampleAIModule::onEnd(bool isWinner)
{
  // Called when the game ends
  if ( isWinner )
  {
    // Log your win here!
  }
}

void ExampleAIModule::onFrame()
{
  // Called once every game frame

  // Display the game frame rate as text in the upper left area of the screen
  Broodwar->drawTextScreen(200, 0,  "FPS: %d", Broodwar->getFPS() );
  Broodwar->drawTextScreen(200, 20, "Average FPS: %f", Broodwar->getAverageFPS() );

  // Return if the game is a replay or is paused
  if ( Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self() )
    return;

  // Prevent spamming by only running our onFrame once every number of latency frames.
  // Latency frames are the number of frames before commands are processed.
  if ( Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0 )
    return;




  Master::Update();

  for (Worker* w : Worker::Workers)
  {
	  w->Update();
  }

  for (Pylon* p : Pylon::Pylons)
  {
	  p->Update();
  }

  /*for (SupplyBuilder* s : SupplyBuilder::SupplyBuilders)
  {
	  s->Update();
  }*/



  // Iterate through all the units that we own
 /* for (auto &u : Broodwar->self()->getUnits())
  {
	  // Ignore the unit if it no longer exists
	  // Make sure to include this block when handling any Unit pointer!
	  if (!u->exists())
		  continue;

	  // Ignore the unit if it has one of the following status ailments
	  if (u->isLockedDown() || u->isMaelstrommed() || u->isStasised())
		  continue;

	  // Ignore the unit if it is in one of the following states
	  if (u->isLoaded() || !u->isPowered() || u->isStuck())
		  continue;

	  // Ignore the unit if it is incomplete or busy constructing
	  if (!u->isCompleted() || u->isConstructing())
		  continue;

  } */

}

void ExampleAIModule::onSendText(std::string text)
{

  // Send the text to the game if it is not being processed.
  Broodwar->sendText("%s", text.c_str());


  // Make sure to use %s and pass the text as a parameter,
  // otherwise you may run into problems when you use the %(percent) character!

}

void ExampleAIModule::onReceiveText(BWAPI::Player player, std::string text)
{
  // Parse the received text
  Broodwar << player->getName() << " said \"" << text << "\"" << std::endl;
}

void ExampleAIModule::onPlayerLeft(BWAPI::Player player)
{
  // Interact verbally with the other players in the game by
  // announcing that the other player has left.
  Broodwar->sendText("Goodbye %s!", player->getName().c_str());
}

void ExampleAIModule::onNukeDetect(BWAPI::Position target)
{

  // Check if the target is a valid position
  if ( target )
  {
    // if so, print the location of the nuclear strike target
    Broodwar << "Nuclear Launch Detected at " << target << std::endl;
  }
  else 
  {
    // Otherwise, ask other players where the nuke is!
    Broodwar->sendText("Where's the nuke?");
  }

  // You can also retrieve all the nuclear missile targets using Broodwar->getNukeDots()!
}

void ExampleAIModule::onUnitDiscover(BWAPI::Unit unit)
{
	if (unit->getType().isBuilding())
	{
		Master::InformEnemyBaseLocation(unit->getPosition());
	}

}

void ExampleAIModule::onUnitEvade(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitShow(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitHide(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitCreate(BWAPI::Unit unit)
{
  if ( Broodwar->isReplay() )
  {
    // if we are in a replay, then we will print out the build order of the structures
    if ( unit->getType().isBuilding() && !unit->getPlayer()->isNeutral() )
    {
      int seconds = Broodwar->getFrameCount()/24;
      int minutes = seconds/60;
      seconds %= 60;
      Broodwar->sendText("%.2d:%.2d: %s creates a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().c_str());
    }
  }
  else
  {
	  //CODE HERE PLZ

	  if (Broodwar->self()->getUnits().contains(unit))
	  {
		  if (unit->getType().isWorker())
			  Worker::Workers.push_back(new Worker(unit));
		  else if (unit->getType() == UnitTypes::Protoss_Pylon)
		  {
			  Pylon::Pylons.push_back(new Pylon(unit));
			  Master::waitPylonCount--;
		  }
		  else if (unit->getType().isResourceDepot())
		  {
			  SupplyBuilder::SupplyBuilders.push_back(new SupplyBuilder(unit));
		  }
		  else if (unit->getType() == UnitTypes::Protoss_Gateway)
		  {
			  Master::waitGatewayCount--;
		  }
	  }

  }
}

void ExampleAIModule::onUnitDestroy(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitMorph(BWAPI::Unit unit)
{
  if ( Broodwar->isReplay() )
  {
    // if we are in a replay, then we will print out the build order of the structures
    if ( unit->getType().isBuilding() && !unit->getPlayer()->isNeutral() )
    {
      int seconds = Broodwar->getFrameCount()/24;
      int minutes = seconds/60;
      seconds %= 60;
      Broodwar->sendText("%.2d:%.2d: %s morphs a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().c_str());
    }
  }
}

void ExampleAIModule::onUnitRenegade(BWAPI::Unit unit)
{
}

void ExampleAIModule::onSaveGame(std::string gameName)
{
  Broodwar << "The game was saved to \"" << gameName << "\"" << std::endl;
}

void ExampleAIModule::onUnitComplete(BWAPI::Unit unit)
{

}
