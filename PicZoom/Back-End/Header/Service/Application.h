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
	void             run();


private:
	//methods
	bool loadConfig();
	bool savaConfig();
	
	//parse commond
	std::list<std::string> parseCommond(std::string);
	
	//dispatcher commond
	bool dispatchCommond(std::list<string> comList);

	//commonds                                          //commonds:
	bool                       cdLoc(std::string name); //cd
	void                       mkLoc(std::string name); //mkloc
	void                       deLoc(std::string name); //deloc
	std::list<Location*>       lsLoc();                 //lsloc
	std::list<Picture*>        lsPic();                 //lspic
	void                       adPic(std::string url);  //adpic
	void                       dePic();                 //depic
	void                       ldPic(std::string name); //ldpic
	void                       ldMap();                 //ldmap
	void                       adMap();                 //admap
	void                       deMap();                 //demap
	



	//property
	static std::string        _configUrl;
	LocationService*          _locationService;
	MapService*               _mapService;
	std::list<Location*>      _locationPath;
	CPicZoom*                 _PicZoom;
};

#endif
