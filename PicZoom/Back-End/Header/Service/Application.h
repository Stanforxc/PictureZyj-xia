#pragma once
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "LocationService.h"
#include "MapService.h"
#include "../../Header/Json/json.h"


class Application {
public:
	Application();
	~Application();

	LocationService* getLocationService();
	MapService*      getMapService();

private:
	//methods
	bool loadConfig();
	bool savaConfig();
	//property
	static std::string      _configUrl;
	LocationService* _locationService;
	MapService*      _mapService;
};

#endif
