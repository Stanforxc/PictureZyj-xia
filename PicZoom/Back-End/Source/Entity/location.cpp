#include "../../Header/Entity/location.h"
#include "../../Header/Entity/map.h"
#include "../../Header/Entity/picture.h"
#include <algorithm>

Location::Location() {
	this->_map = nullptr;
	this->_locationName = nullptr;
	this->_description = nullptr;
}

Location::Location(std::string locationName, std::string description, Map* map) {
	this->_locationName = locationName;
	this->_description = description;
	this->_map = map;
}

Location::~Location() {
	for (auto itr = this->_pictureContainer.begin(); itr != this->_pictureContainer.end(); itr++) {
		//delete all picture
		std::string path = "Location\\" + this->_locationName + "\\" + (*itr)->getName();
		CString fileUrl =("%s",path.c_str());
		DeleteFile(fileUrl.AllocSysString());
	}
	//delete map
	std::string mapPath = "Location\\" + this->_locationName + "\\" + this->_map->getName();
	CString mapUrl = ("%s", mapPath.c_str());
	DeleteFile(mapUrl.AllocSysString());
}


Map* Location::getMap() {
	return this->_map;
}

void Location::setMap(Map* map) {
	this->_map = map;
}

std::list<Picture*> Location::getPictureContainer() {
	return this->_pictureContainer;
}

bool Location::addToPictureContainer(Picture* pictureToAdd) {
	if (false == pictureToAdd) {
		return false;
	}
	auto itr = std::find(this->_pictureContainer.begin(), this->_pictureContainer.end(), pictureToAdd);
	if (itr == this->_pictureContainer.end()) {
		this->_pictureContainer.push_back(pictureToAdd);
		return true;
	}
	else {
		return false;
	}
}


bool Location::deletePictureFromContainer(std::string pictureToDelete) {
	for (auto itr = this->_pictureContainer.begin(); itr != this->_pictureContainer.end(); itr++) {
		if (0 == pictureToDelete.compare((*itr)->getName())) {
			this->_pictureContainer.remove(*itr);
			return true;
		}
	}
	return false;
}

std::string Location::getLocationName() {
	return this->_locationName;
}

void Location::setLocationName(std::string locationName) {
	this->_locationName = locationName;
}

std::string Location::getDescription() {
	return this->_description;
}

void Location::setDescription(std::string description) {
	this->_description = description;
}

std::list<Location*> Location::getSubLocation() {
	std::list<Location*> ret;
	std::list<Map*> subMaps = this->_map->getSubMapList();
	
	for (auto itr = subMaps.begin(); itr != subMaps.end(); itr++) {
		ret.push_back((*itr)->getLoc());
	}

	return ret;
}

Location* Location::getParentLocation() {
	return this->_map->getParentMap()->getLoc();
}


