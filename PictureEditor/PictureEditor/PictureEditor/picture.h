#pragma once
#ifndef _PICTURE_H_
#define _PICTURE_H_

#include "image.h"
#include "location.h"
#include <list>
#include <string>


class Picture :
	public Image
{
public:
	Picture();
	Picture(std::string url, std::string picName, std::string des, Map* map);
	~Picture();

	list<Location*> getLocationPath();
	Map*            getMap();

private:
	list<Location*>  _locationPath;
	Map*             _map;

};

#endif




