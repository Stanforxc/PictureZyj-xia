#pragma once

#ifndef _MAP_H_
#define _MAP_H_

#include "image.h"
<<<<<<< HEAD
#include "location.h"
#include <map>


struct Coordinate {
	int x;
	int y;
};

class Map :
	public Image
=======
class Map :
	public image
>>>>>>> origin/master
{
public:
	Map();
	~Map();
<<<<<<< HEAD

	Map*           getSubMapByCoordiante(int x, int y);
	bool           deleteSubMap(int x, int y, Map*);
	void           addSubMap(int x, int y, Map* map);
	Map*           getParentMap();


private:
	std::map<Coordinate, Map*>   _subMap;
	Map*                         _parentMap;
=======
>>>>>>> origin/master
};

#endif





