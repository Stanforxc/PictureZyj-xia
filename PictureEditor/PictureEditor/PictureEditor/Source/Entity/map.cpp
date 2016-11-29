#include"../../Header/Entity/map.h"

Map::Map(): Image() {
	this->_parentMap = nullptr;
}

Map::Map(string url, string picName, string des, Map* parentMap, int x, int y) :
	Image(url, picName, des) {
	this->_parentMap = parentMap;
	this->_coordinate.x = x;
	this->_coordinate.y = y;
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

bool Map::deleteSubMap(Map* map) {
	for (std::map<Coordinate, Map*>::iterator itr = this->_subMap.begin(); itr != this->_subMap.end(); itr++) {
		if (map->_coordinate.x == itr->first.x && map->_coordinate.y == itr->first.y
			&& 0 == map->getName().compare(itr->second->getName()))
			//have the same coordinate and have the same map name
		{
			this->_subMap.erase(itr);
			return true;
		}
	}
	return false;
}

bool Map::addSubMap(Map* map) {
	for (std::map<Coordinate, Map*>::iterator itr = this->_subMap.begin(); itr != this->_subMap.end(); itr++) {
		if (map->_coordinate.x == itr->first.x && map->_coordinate.y == itr->first.y) {
			return false;
		}
	}
	this->_subMap.insert(pair<Coordinate, Map*>(*new Coordinate(map->_coordinate.x, map->_coordinate.y), map));
	return true;
}

Map* Map::getParentMap() {
	return this->_parentMap;
}



void Map::setCoordinate(int x, int y) {
	this->_coordinate.x = x;
	this->_coordinate.y = y;
}

Coordinate Map::getCoordiante(){
	return this->_coordinate;
}