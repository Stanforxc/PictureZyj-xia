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
	bool addMap( Map* mapToAdd, int x=-1, int y=-1);
	bool deleteMap(Map* mapToDelete);
private:
	Map* _rootMap;
	int  _nextId;
};


#endif
