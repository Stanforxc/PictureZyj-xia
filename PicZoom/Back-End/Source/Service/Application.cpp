#include"../../Header/Service/Application.h"
#include<exception>
#include<fstream>
#include<algorithm>

std::string Application::_configUrl = "Config/config.json";

Application::Application() {
	this->_locationService = new LocationService();
	this->_mapService = new MapService();
	this->loadConfig();
	//this->_PicZoom = picZoom;
}

Application::~Application() {
	this->savaConfig();
	delete this->_locationService;
	delete this->_mapService;
}

LocationService* Application::getLocationService() {
	return this->_locationService;
}

MapService* Application::getMapService() {
	return this->_mapService;
}

bool Application::savaConfig() {
	try {
		Json::Value ConfigValue;
		Json::Value LocationConfig;
		Json::Value MapConfig;
		std::ofstream outfile;
		outfile.open(Application::_configUrl, std::ios::out);

		/********************************************************/
		//location config
		auto locRepo = this->_locationService->_locationRepo;
		for (auto itr = locRepo.begin();itr != locRepo.begin(); itr++) {
			Json::Value location;
			location["locationName"] = (*itr)->getLocationName();
			location["description"] = (*itr)->getDescription();
			Json::Value picContainer;
			for (auto itrPic=(*itr)->getPictureContainer().begin();
				itrPic != (*itr)->getPictureContainer().end(); itrPic++) {//each Picture*
				Json::Value picItem;
				picItem["pictureName"] = (*itrPic)->getName();
				picItem["description"] = (*itrPic)->getDiscription();
				picContainer.append(picItem);
			}
			location["pictureContainer"] = picContainer;
			LocationConfig.append(location);
		}
		ConfigValue["LocationConfig"] = LocationConfig;


		/******************************************************/
		//map config --- traverse the map tree with BFS
		MapConfig["nextId"] = this->_mapService->_nextId;
		Json::Value mapContainer;
		Map* currentMap = nullptr;
		std::list<Map*> mapQueue;
		mapQueue.push_back(this->_mapService->_rootMap);
		do {
			currentMap = mapQueue.front();
			for (auto itr = currentMap->_subMap.begin(); itr != currentMap->_subMap.end(); itr++) {
				mapQueue.push_back(itr->second);
			}
			mapQueue.pop_front();

			//make map json value
			Json::Value map;
			//map::id
			map["id"] = currentMap->_Id;
			//map::coordinate
			Json::Value coordinate;
			coordinate["x"] = currentMap->_coordinate.x;
			coordinate["y"] = currentMap->_coordinate.y;
			map["coordinate"] = coordinate;
			//map::parentMap
			if (nullptr != currentMap->_parentMap) {
				map["parentMap"] = currentMap->_parentMap->_Id;
			}
			else {
				map["parentMap"] = -1;
			}
			//map::location
			map["location"] = currentMap->getLoc()->getLocationName();
			//map::url
			map["url"] = currentMap->getUrl();
			//map::pictureName
			map["pictureName"] = currentMap->getName();
			//map::description
			map["description"] = currentMap->getDiscription();

			//append
			mapContainer.append(map);

		} while (0!=mapQueue.size());
		MapConfig["mapContainer"] = mapContainer;
		ConfigValue["MapConfig"] = MapConfig;
		
		//output file
		outfile << ConfigValue.toStyledString();
		outfile.close();
		return true;
	}
	catch (std::exception e) {
		return false;
	}
}

bool Application::loadConfig() {
	try {
		std::ifstream in(Application::_configUrl, std::ios::binary);
		std::filebuf* pbuf=in.rdbuf();
		//get size of file
		long long filesize = pbuf->pubseekoff(0, std::ios::end, std::ios::in);
		pbuf->pubseekpos(0,std::ios::in);
		//alloc memory & read file
		char* configChars = new char[filesize];
		pbuf->sgetn(configChars, filesize);
		//Parse json config
		Json::Reader configReader;
		Json::Value configValue;
		if (configReader.parse(*(new string(configChars)), configValue)) {

			/************************************/
			//load location config
			Json::Value locationConfig = configValue["LocationConfig"];
			//for each location
			for (auto itr=locationConfig.begin(); itr != locationConfig.end(); itr++) {
				Location* locationItem = new Location();
				locationItem->_locationName = (*itr)["locationName"].asString();
				locationItem->_description = (*itr)["description"].asString();
				//each picture
				for (auto itrPic = (*itr)["pictureContainer"].begin();
					itrPic != (*itr)["pictureContainer"].end(); itrPic++) {
					Picture* picItem = new Picture();
					picItem->setName((*itrPic)["pictureName"].asString());
					picItem->setDiscription((*itrPic)["description"].asString());
					//add picture to location
					locationItem->addToPictureContainer(picItem);
				}
				this->_locationService->addNewLocation(locationItem);
			}

			/************************************/
			//load map config
		    Json::Value mapConfig = configValue["MapConfig"];
			this->_mapService->_nextId = mapConfig["nextId"].asInt();
			//for each map
			for (auto itr = mapConfig["mapContainer"].begin(); itr != mapConfig["mapContainer"].end(); itr++) {
				//get all parameters to prepare for newing an item
				std::string picName = (*itr)["pictureName"].asString();
				std::string description = (*itr)["description"].asString();
				Map*        parentMap = this->_mapService->getMapById((*itr)["parentMap"].asInt());
				int         coordianteX = (*itr)["coordiante"]["x"].asInt();
				int         coordianteY = (*itr)["coordiante"]["y"].asInt();
				//new a item
				Map* mapItem = new Map(picName, description, parentMap, coordianteX, coordianteY);
				//set Id
				mapItem->_Id = (*itr)["id"].asInt();
				//insert to parent map
				parentMap->_subMap.insert(
					pair<Coordinate,Map*>(
					*(new Coordinate(coordianteX,coordianteY)),
					mapItem
					)
				);
				//set location
				Location* location = this->_locationService->getLocation((*itr)["location"].asString());
				mapItem->setLoc(location);
				location->_map = mapItem;
			}
			return true;
		}//parse json successful
		else {
			return false;
		}//fail to parse json

	}catch (...) {
		return false;
	}
}

std::list<string> Application::parseCommond(std::string commond) {
	std::list<string> ret;
	int firstFlag = 0, secondFlag = 0;
	int length = commond.size();
	while (firstFlag < length) {
		while (commond[secondFlag] != ' ' && secondFlag != length) {
			secondFlag++;
		}
		ret.push_back( commond.substr(firstFlag, secondFlag-firstFlag) );
		//ignore all the space
		firstFlag = secondFlag;
		while (firstFlag < length && commond[firstFlag] == ' ') {
			firstFlag++;
		}
		secondFlag = firstFlag;
	}
	return ret;
}

bool Application::dispatchCommond(std::list<string> comList) {
	if (3 < comList.size()) {
		return false;
	}

	std::string commond = comList.front();

	//cd
	if(0 == commond.compare("cd")) {
		this->cdLoc(comList.back());
	}

	//mkloc
	if(0 == commond.compare("mkloc")) {
		this->mkLoc(comList.back());
	}

	//deloc
	if (0 == commond.compare("deloc")) {
		this->deLoc(comList.back());
	}

	//lsloc
	if (0 == commond.compare("lsloc")) {
		this->lsLoc();
	}

	//lspic
	if (0 == commond.compare("lspic")) {
		this->lsPic();
	}

	//adpic
	if (0 == commond.compare("adpic")) {
		this->adPic(*(comList.begin()++), comList.back());
	}

	//depic
	if (0 == commond.compare("depic")) {
		this->dePic(comList.back());
	}

	//ldpic
	if (0 == commond.compare("ldpic")) {
		this->ldPic(comList.back());
	}

	//ldmap
	if (0 == commond.compare("ldmap")) {
		this->ldMap();
	}

	//admap
	if (0 == commond.compare("admap")) {
		this->adMap(*(comList.begin()++), comList.back());
	}

	//demap
	if (0 == commond.compare("demap")) {
		this->deMap();
	}


}

/***************************commond********************************************************/

//cdloc
void Application::cdLoc(std::string name) {
	std::list<Location*> allLoc = this->_locationService->_currentLocation->getSubLocation();
	for (auto itr = allLoc.begin(); itr != allLoc.end(); itr++) {
		if (0 == name.compare((*itr)->_locationName)) {
			this->_locationService->_currentLocation = *itr;
			return;
		}
	}
	std::cout << "No such location!" << std::endl;
	return;
}

//mkloc
void Application::mkLoc(std::string name) {
	Location* newloc = new Location();
	Map* newmap = new Map();

	//set map info
	newmap->setLoc(newloc);
	newmap->_parentMap = this->_locationService->_currentLocation->_map;
	//set loc info
	newloc->setMap(newmap);
	newloc->_locationName = name;
	//add
	this->_locationService->addNewLocation(newloc);
	this->_mapService->addMap(newmap);
	
	//create folder
	std::string cmd = "md Location\\" + name;
	system(cmd.c_str());
}

//deloc
void Application::deLoc(std::string name) {
	std::list<Location*> allLoc = this->_locationService->_currentLocation->getSubLocation();
	for (auto itr = allLoc.begin(); itr != allLoc.end(); itr++) {
		if (0 == name.compare((*itr)->_locationName)) {
			this->_locationService->deleteLocation(*itr);
			this->_mapService->deleteMap((*itr)->_map);
			return;
		}
	}
	std::cout << "No such location!" << std::endl;
	return;
}

//lsloc
void Application::lsLoc() {
	int size = 0;
	std::list<Location*> allLoc = this->_locationService->_currentLocation->getSubLocation();
	for (auto itr = allLoc.begin(); itr != allLoc.end(); itr++, size++) {
		cout << (*itr)->_locationName;
		std::cout << (0 == size % 3 ? "\n" : "  ");
	}
}

//lsPic
void Application::lsPic() {
	int size = 0;
	auto picContainer = this->_locationService->_currentLocation->getPictureContainer();
	for (auto itr = picContainer.begin(); itr != picContainer.end(); itr++) {
		std::cout << (*itr)->getName();
		std::cout << (0 == size % 3 ? "\n" : "  ");
	}
}

//adpic
void Application::adPic(std::string url, std::string name, std::string des) {
	//copy file
	CString existingFile(url.c_str());
	std::string strTargetFileUrl = "Location\\" + this->_locationService->_currentLocation->getLocationName() + "\\" + name;
	CString targetFile(strTargetFileUrl.c_str());
	CopyFile(existingFile.AllocSysString(), targetFile.AllocSysString(), false);

	//new a Picture and add to location
	Picture* newPicItem = new Picture(name,des);
	newPicItem->setLoc(this->_locationService->_currentLocation);
	this->_locationService->_currentLocation->addToPictureContainer(newPicItem);
}

//depic
void Application::dePic(std::string name) {
	Location* curLoc = this->_locationService->_currentLocation;
	
	if(false == curLoc->deletePictureFromContainer(name)){
		std::cout << "No such picture" << endl;
	}
	
}

//ldpic
void Application::ldPic(std::string name) {
	std::list<Picture*> picCon = this->_locationService->_currentLocation->getPictureContainer();
	//search for Picture
	for (auto itr = picCon.begin(); itr != picCon.end(); itr++) {
		if (0 == name.compare((*itr)->getName())) {
			const char* picUrl = (*itr)->getUrl().c_str();
			//char to tchar
			int iLength = MultiByteToWideChar(CP_ACP, 0, picUrl, strlen(picUrl) + 1, NULL, 0);
			TCHAR* tPicUrl = new TCHAR[iLength];
			MultiByteToWideChar(CP_ACP, 0, picUrl, strlen(picUrl) + 1, tPicUrl, iLength);
			//new a dialog
			CPicZoomDlg dlg(tPicUrl);
			dlg.DoModal();
			return;
		}
	}
	std::cout << "Error!No such picture!" << std::endl;
}

//ldmap
void Application::ldMap() {
	Location* curLoc = this->_locationService->_currentLocation;
	const char* mapUrl = curLoc->_map->getUrl().c_str();
	//char to tchar
	int iLength = MultiByteToWideChar(CP_ACP, 0, mapUrl, strlen(mapUrl) + 1, NULL, 0);
	TCHAR* tMapUrl = new TCHAR[iLength];
	MultiByteToWideChar(CP_ACP, 0, mapUrl, strlen(mapUrl) + 1, tMapUrl, iLength);
	//new a dialog
	CPicZoomDlg dlg(tMapUrl);
	dlg.DoModal();
}

//admap
void Application::adMap(std::string url, std::string name, std::string des) {
	//copy file
	CString existingFile(url.c_str());
	std::string strTargetFileUrl = "Location\\" + this->_locationService->_currentLocation->getLocationName() + "\\" + name;
	CString targetFile(strTargetFileUrl.c_str());
	CopyFile(existingFile.AllocSysString(), targetFile.AllocSysString(), false);
	//add map
	Location* curLoc = this->_locationService->_currentLocation;
	curLoc->_map->setName(name);
	curLoc->_map->setDiscription(des);
}

//demap
void Application::deMap() {
	Location* curLoc = this->_locationService->_currentLocation;
	if (0 == curLoc->_map->getName().size()) {
		std::cout << "No map existing!" << endl;
	}
	//delete map file
	std::string path = "Location\\" + curLoc->getLocationName() +"\\" + curLoc->_map->getName();
	CString fileUrl = ("%s", path.c_str());
	DeleteFile(fileUrl.AllocSysString());
}


/************************************run**********************************************/



void Application::run() {

	this->outputPath();
	while (1) {
		std::string cmd;
		getline(std::cin,cmd);
		std::list<std::string> cmdList = this->parseCommond(cmd);
		this->dispatchCommond(cmdList);
		this->outputPath();
	}
}

void Application::outputPath() {
	std::cout << std::endl << "root";
	Location* loc = this->_locationService->_currentLocation;
	if (NULL == loc) {
		std::cout << ">";
		return;
	}
	std::list < std::string> path;
	while (nullptr != loc) {
		path.push_front(loc->getLocationName());
		loc = loc->getParentLocation();
	}

	for (auto itr = path.begin(); itr != path.end(); itr++) {
		std::cout << *itr;
		std::cout << (itr == --path.end() ? '/' : '>');
	}
}
