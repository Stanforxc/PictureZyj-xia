#pragma once

#ifndef _LOCATIONSERVICE_H_
#define _LOCATIONSERVICE_H_

#include "../Entity/location.h"
#include "../Entity/map.h"
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
	std::list<Location*>  _locationRepo;
	Location*             _currentLocation;
	static std::string _configUrl;
};


#endif



