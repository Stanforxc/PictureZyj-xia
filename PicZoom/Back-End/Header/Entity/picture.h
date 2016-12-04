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
	Picture(std::string picName, std::string des);
	~Picture();

	list<Location*>  getLocationPath();

private:
	list<Location*>  _locationPath;

};

#endif




