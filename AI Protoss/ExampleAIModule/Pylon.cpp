#include "Pylon.h"


std::vector<Pylon*> Pylon::Pylons;
Pylon::Pylon(BWAPI::Unit u) : StarcraftUnit(u)
{
}


Pylon::~Pylon()
{
}


void Pylon::Update()
{

}