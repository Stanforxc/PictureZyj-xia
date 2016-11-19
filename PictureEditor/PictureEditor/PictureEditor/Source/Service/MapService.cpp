#include"../../Header/Service/MapService.h"
#include<exception>

MapService::MapService() {
	this->loadConfig();
	if (0 != this->_mapRepo.size()) {
		this->_currentMap = *this->_mapRepo.begin();
	}
	else {
		this->_currentMap = nullptr;
	}
}

MapService::~MapService() {
	this->saveConfig();
	try {
		for (auto itr = this->_mapRepo.begin(); itr != _mapRepo.end(); itr++) {
			delete *itr;
		}
	}
	catch (...) {
		//ignore
	}
}

Map* MapService::getMapByCondition(std::string mapName) {

}

bool MapService::addMap(Map* mapToAdd){

}

