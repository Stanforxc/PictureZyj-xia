#pragma once

#ifndef _MAP_H_
#define _MAP_H_

#include "image.h"
#include "location.h"
#include <map>
#include <list>
#include "../Service/MapService.h"


struct Coordinate {
	int x;
	int y;

	Coordinate() {
		this->x = 0;
		this->y = 0;
	}

	Coordinate(int xx, int yy) {
		this->x = xx;
		this->y = yy;
	}
};

class Map :
	public Image
{
	friend class MapService;
public:
	Map();
	Map(string url, string picName, string des, Map* parentMap, int x, int y);
	~Map();

	Map*           getSubMapByCoordiante(int x, int y);
	bool           deleteSubMap( Map* map);
	bool           addSubMap(Map* map);
	Map*           getParentMap();
	int            getId();
	void           setId(int Id);
	void           setCoordinate(int x, int y);
	Coordinate     getCoordiante();


private:
	std::map<Coordinate, Map*>   _subMap;
	Map*                         _parentMap;
	int                          _Id;
	Coordinate                   _coordinate;
};

#endif





