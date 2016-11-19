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
	Location* getCurrentLocation();
	void      setCurrentLocation(Location* locationToSet);


	

private:
	bool      loadConfig();
	bool      saveConfig();
	std::list<Location*>  _locationRepo;
	Location*             _currentLocation;
};


#endif



