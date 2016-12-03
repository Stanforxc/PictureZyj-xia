#pragma once

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "LocationService.h"
#include "MapService.h"
#include "../../Header/Json/json.h"
#include "../Entity/picture.h"


class CPicZoom;

class Application {
public:
	Application(CPicZoom* picZoom);
	~Application();

	LocationService* getLocationService();
	MapService*      getMapService();

private:
	//methods
	bool loadConfig();
	bool savaConfig();
	
	//commonds
	bool                       cdLoc(std::string name);
	void                       mkLoc(std::string name);
	void                       deLoc(std::string name);
	std::list<Location*>       lsLoc();
	std::list<Picture*>        lsPic();
	void                       adPic(std::string url);
	void                       dePic();
	void                       ldPic(std::string name);
	void                       ldMap();
	void                       adMap();
	void                       deMap();




	//property
	static std::string        _configUrl;
	LocationService*          _locationService;
	MapService*               _mapService;
	std::list<Location*>      _locationPath;
	CPicZoom*                 _PicZoom;
};

#endif
