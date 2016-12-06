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

	bool operator < (Coordinate& para) const {
		return this->x < para.x;
	}

	bool operator = (Coordinate& para) const {
		return this->x == para.x && this->y == para.y;
	}
};

class Map :
	public Image
{
	friend class MapService;
	friend class Application;
public:
	Map();
	Map(string picName, string des, Map* parentMap, int x=-1, int y=-1);
	~Map();

	Map*           getSubMapByCoordiante(int x, int y);
	bool           deleteSubMap( Map* map);
	bool           addSubMap(Map* map);
	Map*           getParentMap();
	void           setCoordinate(int x, int y);
	Coordinate     getCoordiante();
	std::list<Map*> getSubMapList();
	bool           setSubMapCor(Coordinate oC, Coordinate nC );


private:
	std::map<Coordinate, Map*>   _subMap;
	Map*                         _parentMap;
	int                          _Id;
	Coordinate                   _coordinate;
};

#endif





