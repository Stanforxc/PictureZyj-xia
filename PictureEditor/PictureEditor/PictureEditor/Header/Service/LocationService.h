#pragma once

#ifndef _LOCATIONSERVICE_H_
#define _LOCATIONSERVICE_H_

#include "../Entity/location.h"
#include <list>

class LocationService {
public:
	LocationService();
	~LocationService();

	Location* getLocation(std::string locationName);
	bool      addNewLocation(Location* location);
	bool      deleteLocation(Location* locationToDelete);
	bool      loadConfig();
	bool      saveConfig();

	

private:
	std::list<Location*>  _locationRepo;
};


#endif



