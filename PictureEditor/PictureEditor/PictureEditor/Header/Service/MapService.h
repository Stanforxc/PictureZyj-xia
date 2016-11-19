#pragma once

#ifndef _MAPSERVICE_H_
#define _MAPSERVICE_H_

#include "../Entity/map.h"
#include <list>


class MapService {
public:
	MapService();
	~MapService();

	Map* getMapByCondition(std::string mapName);
	Map* getMapByLocation(Location* location);
	bool addMap(Map* mapToAdd);
	bool deleteMap(Map* mapToDelete);
	


private:
	bool loadConfig();
	bool saveConfig();
	Map* _rootMap;
	Map* _currentMap;
};


#endif
