#include"../../Header/Service/MapService.h"
#include<exception>
#include<list>

MapService::MapService() {
	this->_rootMap = nullptr;
	this->_currentMap = nullptr;
}

MapService::~MapService() {
	try {
		this->deleteMap(this->_rootMap);
	}catch (...) {
		//ignore 
	}
}

Map* MapService::getMapByName(std::string mapName) {
	try {
		std::list<Map*> searchQueue;
		std::map<Coordinate, Map*> *subMap;
		searchQueue.push_back(this->_rootMap);
		do {
			Map* currentMap = *searchQueue.begin();
			subMap = &currentMap->_subMap;
			for (auto itr = subMap->begin(); itr != subMap->end(); itr++) {
				searchQueue.push_back(itr->second);
			}
			searchQueue.pop_front();
			if (0 == mapName.compare(currentMap->getName())) {
				return currentMap;
			}
		} while (0 != searchQueue.size());
		return nullptr;
	}
	catch (...) {
		return nullptr;
	}
}

bool MapService::addMap(int x, int y, Map* mapToAdd) {
	try {
		bool existFlag = false;
		//first check if the parent is in the tree
		Map* currentMap = mapToAdd;
		while (nullptr != currentMap->getParentMap()) {
			currentMap = currentMap->getParentMap();
		}
		if (currentMap != this->_rootMap) {
			return false;
		}
		//add
		mapToAdd->getParentMap()->addSubMap(mapToAdd);

	}
	catch (...) {
		return false;
	}
}

bool MapService::deleteMap(Map* mapToDelete) {
	try {
		if (nullptr == mapToDelete) {
			return false;
		}
		//check if the map exist in the map
		Map* currentMap = mapToDelete;
		while (nullptr!=currentMap->getParentMap()) {
			currentMap = currentMap->getParentMap();
		}
		if (currentMap != this->_rootMap) {
			return false;
		}
		//delete from parent
		auto parentSubmapSet = mapToDelete->getParentMap()->_subMap;
		for (auto itrPa = parentSubmapSet.begin(); itrPa != parentSubmapSet.end(); itrPa++) {
			if (mapToDelete == itrPa->second) {
				mapToDelete->getParentMap()->deleteSubMap(mapToDelete);
			}
		}
		//free this map and all its submaps
		std::list<Map*> deleteQueue;
		Map* currentMap;
		deleteQueue.push_back(mapToDelete);
		do {
			currentMap = *deleteQueue.begin();
			for (auto itr = currentMap->_subMap.begin(); itr != currentMap->_subMap.end(); itr++) {
				deleteQueue.push_back(itr->second);
			}
			deleteQueue.pop_front();
			delete currentMap;
		} while (0!=deleteQueue.size());
		return true;
	}catch (...) {
		return false;
	}
}





