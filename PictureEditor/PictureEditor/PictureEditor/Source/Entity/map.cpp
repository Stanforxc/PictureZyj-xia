#include"map.h"

Map::Map(): Image() {
	this->_parentMap = nullptr;
}

Map::Map(string url, string picName, string des, Map* parentMap) :
	Image(url, picName, des) {
	this->_parentMap = parentMap;
}

Map::~Map() {}

Map*  Map::getSubMapByCoordiante(int x, int y) {
	for (std::map<Coordinate, Map*>::iterator itr = this->_subMap.begin(); itr != this->_subMap.end(); itr++) {
		if (x == itr->first.x && y == itr->first.y) {
			return itr->second;
		}
	}
	return nullptr;
}

bool Map::deleteSubMap(int x, int y, Map* map) {
	for (std::map<Coordinate, Map*>::iterator itr = this->_subMap.begin(); itr != this->_subMap.end(); itr++) {
		if (x == itr->first.x && y == itr->first.y 
			&& 0 == map->getName().compare(itr->second->getName()))
			//have the same coordinate and have the same map name
		{
			this->_subMap.erase(itr);
			return true;
		}
	}
	return false;
}

bool Map::addSubMap(int x, int y, Map* map) {
	for (std::map<Coordinate, Map*>::iterator itr = this->_subMap.begin(); itr != this->_subMap.end(); itr++) {
		if (x == itr->first.x && y == itr->first.y) {
			return false;
		}
	}
	this->_subMap.insert(pair<Coordinate, Map*>(Coordinate(x,y), map));
	return true;
}

Map* Map::getParentMap() {
	return this->_parentMap;
}