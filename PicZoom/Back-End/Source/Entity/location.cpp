#include "../../Header/Entity/location.h"
#include "../../Header/Entity/map.h"
#include "../../Header/Entity/picture.h"
#include <algorithm>
#include <stdio.h>

Location::Location() {
	this->_map = nullptr;
}

Location::Location(std::string locationName, std::string description, Map* map) {
	this->_locationName = locationName;
	this->_description = description;
	this->_map = map;
}

Location::~Location() {

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
			//delete file
			remove((*itr)->getUrl().c_str());
			//delete from picture container
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
	Map* parentMap = this->_map->getParentMap();
	if (nullptr == parentMap) {
		return nullptr;
	}
	else {
		return parentMap->getLoc();
	}
}


