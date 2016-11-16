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
	~Picture();
private:
	list<Location>  _locationPath;
	Map*            _map;

};

#endif




