#include "PhotonCanon.h"


std::vector<PhotonCanon*> PhotonCanon::PhotonCanons;
PhotonCanon::PhotonCanon(BWAPI::Unit u) : StarcraftUnit(u)
{
}


PhotonCanon::~PhotonCanon()
{
}

void PhotonCanon::Update()
{
	if (!m_unit->exists())
	{
		PhotonCanon::PhotonCanons.erase(std::find(PhotonCanon::PhotonCanons.begin(), PhotonCanon::PhotonCanons.end(), this));
		return;
	}
}
