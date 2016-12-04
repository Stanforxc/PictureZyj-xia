#pragma once

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "LocationService.h"
#include "MapService.h"
#include "../../Header/Json/json.h"
#include "../Entity/picture.h"
#include "../../../PicZoomDlg.h"



class Application {
public:
	Application();
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

	//commonds     /*commonds:*/                                                       
	void           /*cd*/               cdLoc(std::string name);                   
	void           /*mkloc*/            mkLoc(std::string name);                   
	void           /*deloc*/            deLoc(std::string name);                   
	void           /*lsloc*/            lsLoc();                                   
	void           /*lspic*/            lsPic();                                   
	void           /*adpic*/            adPic(std::string url, std::string name, std::string des = "");  
	void           /*depic*/            dePic(std::string name);                   
	void           /*ldpic*/            ldPic(std::string name);                   
	void           /*ldmap*/            ldMap();                                   
	void           /*admap*/            adMap(std::string url, std::string name, std::string des = "");
	void           /*demap*/            deMap();                                   
	
	//output path
	void outputPath();



	//property
	static std::string        _configUrl;
	LocationService*          _locationService;
	MapService*               _mapService;
	std::list<Location*>      _locationPath;
	//CPicZoomDlg*              _PicZoom;
};

#endif
