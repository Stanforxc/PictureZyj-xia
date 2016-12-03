#include "../../Header/Service/LocationService.h"
#include<exception>
#include <fstream>



LocationService::LocationService() {
	this->_currentLocation = nullptr;
}

LocationService::~LocationService() {
	
	try {
		//do not delete the _currentLocation, because it is a member of _locationRepo
		for (auto itr = this->_locationRepo.begin(); itr != this->_locationRepo.end(); itr++) {
			delete *itr; //delete all the location
		}
	}
	catch (...) {
		//ignore, maybe be deleted in another place
	}
}

Location* LocationService::getLocation(std::string locationName) {
	for (auto itr = this->_locationRepo.begin(); itr != this->_locationRepo.end(); itr++) {
		if (locationName == (*itr)->getLocationName()) {
			return *itr;
		}
	}
	return nullptr;
}

bool LocationService::addNewLocation(Location* locationToAdd) {
	if (nullptr == locationToAdd) {
		return false;
	}
	this->_locationRepo.push_back(locationToAdd);
	return true;
}

bool LocationService::deleteLocation(Location* locationToDelete) {
	if (nullptr == locationToDelete) {
		return false;
	}
	this->_locationRepo.remove(locationToDelete);
	return true;
}

Location* LocationService::getCurrentLocation() {
	return this->_currentLocation;
}

void LocationService::setCurrentLocation(Location* locationToSet) {
	this->_currentLocation = nullptr;
}




