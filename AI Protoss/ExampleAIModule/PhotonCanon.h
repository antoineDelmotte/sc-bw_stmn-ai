#pragma once
#include "StarcraftUnit.h"
class PhotonCanon :
	public StarcraftUnit
{
public:
	static std::vector<PhotonCanon*> PhotonCanon::PhotonCanons;
	PhotonCanon(BWAPI::Unit u);
	~PhotonCanon();
	void Update();
};

