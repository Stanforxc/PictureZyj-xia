#pragma once

#ifndef _MAPSERVICE_H_
#define _MAPSERVICE_H_

#include "../Entity/map.h"
#include <list>


class MapService {
public:
	MapService();
	~MapService();

	Map* getMapByName(std::string mapName);
	bool addMap(int x, int y, Map* mapToAdd);
	bool deleteMap(Map* mapToDelete);
	


private:
	Map* _rootMap;
	Map* _currentMap;
};


#endif
