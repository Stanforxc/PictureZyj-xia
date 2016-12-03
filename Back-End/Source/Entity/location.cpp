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
	try {
		delete this->_map;
	}
	catch (...) {
		//ignore, maybe delete in another place
	}
}


Map* Location::getMap() {
	return this->_map;
}

void Location::setMap(Map* map) {
	this->_map = map;
}

list<std::string> Location::getPictureContainer() {
	return this->_pictureContainer;
}

bool Location::addToPictureContainer(std::string pictureToAdd) {
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
	auto itr = std::find(this->_pictureContainer.begin(), this->_pictureContainer.end(), pictureToDelete);
	if (itr != this->_pictureContainer.end()) {
		this->_pictureContainer.remove(*itr);
		return true;
	}
	else {
		return false;
	}
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


