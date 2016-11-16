#pragma once

#ifndef _MAP_H_
#define _MAP_H_

#include "image.h"
#include "location.h"
#include <map>


struct Coordinate {
	int x;
	int y;
};

class Map :
	public Image
{
public:
	Map();
	~Map();

	Map*           getSubMapByCoordiante(int x, int y);
	bool           deleteSubMap(int x, int y, Map*);
	void           addSubMap(int x, int y, Map* map);
	Map*           getParentMap();


private:
	std::map<Coordinate, Map*>   _subMap;
	Map*                         _parentMap;
};

#endif





