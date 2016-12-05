#include"../../Header/Service/MapService.h"
#include<exception>
#include<list>

MapService::MapService()
{
	this->_rootMap = nullptr;
	this->_nextId = 0;
}

MapService::~MapService() {
	try {		
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

Map* MapService::getMapById(int id) {
	try {
		if (-1 == id)
		{
			return nullptr;
		}
		std::list<Map*> searchQueue;
		std::map<Coordinate, Map*> *subMap;
		searchQueue.push_back(this->_rootMap);
		do {
			Map* currentMap = *searchQueue.begin();
			subMap = &(currentMap->_subMap);
			for (auto itr = subMap->begin(); itr != subMap->end(); itr++) {
				searchQueue.push_back(itr->second);
			}
			searchQueue.pop_front();
			if (id == currentMap->_Id) {
				return currentMap;
			}
		} while (0 != searchQueue.size());
		return nullptr;
	}
	catch (...) {
		return nullptr;
	}
}

bool MapService::addMap(Map* mapToAdd, int x, int y) {
	try {
		bool existFlag = false;
		//first check if the parent is in the tree
		Map* currentMap = mapToAdd;
		while (nullptr != currentMap->getParentMap()) {
			currentMap = currentMap->getParentMap();
		}
		if (this->_rootMap!=nullptr&&currentMap != this->_rootMap) {
			return false;
		}
		//add
		if (mapToAdd->_Id == -100) {
			mapToAdd->_Id = this->_nextId;
			this->_nextId++;
		}

		if (0 == mapToAdd->_coordinate.x && 0 == mapToAdd->_coordinate.y) {
			mapToAdd->_coordinate.x = -mapToAdd->_Id;
			mapToAdd->_coordinate.y = -mapToAdd->_Id;
		}
		if (nullptr != this->_rootMap) {
			mapToAdd->getParentMap()->addSubMap(mapToAdd);
		}
		else {
			this->_rootMap = mapToAdd;
		}
		
		return true;

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
		if (nullptr != mapToDelete->getParentMap()) {
			auto parentSubmapSet = mapToDelete->getParentMap()->_subMap;
			for (auto itrPa = parentSubmapSet.begin(); itrPa != parentSubmapSet.end(); itrPa++) {
				if (mapToDelete == itrPa->second) {
					mapToDelete->getParentMap()->deleteSubMap(mapToDelete);
				}
			}
		}

		//free this map and all its submaps
		std::list<Map*> deleteQueue;
		currentMap = nullptr;
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





