#pragma once

#ifndef _MAPSERVICE_H_
#define _MAPSERVICE_H_

#include "../Entity/map.h"
#include <list>

class Application;

class MapService {
	friend class Application;
public:
	MapService();
	~MapService();

	Map* getMapByName(std::string mapName);
	Map* getMapById(int id);
	bool addMap(int x, int y, Map* mapToAdd);
	bool deleteMap(Map* mapToDelete);
private:
	Map* _rootMap;
	Map* _currentMap;
	int  _nextId;
};


#endif
