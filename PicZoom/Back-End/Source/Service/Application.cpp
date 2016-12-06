#include"../../Header/Service/Application.h"
#include<exception>
#include<fstream>
#include<algorithm>
#include<windows.h>


void setStyle(int style) {
	if (1 == style) {
		HANDLE outPutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(outPutHandle, BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | BACKGROUND_INTENSITY | FOREGROUND_INTENSITY);
	}
	else if (2 == style) {
		HANDLE outPutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(outPutHandle, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	}

}

std::string Application::_configUrl = "Config\\config.json";

Application::Application() {
	this->_locationService = new LocationService();
	this->_mapService = new MapService();
	this->loadConfig();
	setStyle(2);
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
		for (auto itr = locRepo.begin();itr != locRepo.end(); itr++) {
			Json::Value location;
			location["locationName"] = (*itr)->getLocationName();
			location["description"] = (*itr)->getDescription();
			Json::Value picContainer;
			std::list<Picture*> lPicCon = (*itr)->getPictureContainer();
			for (auto itrPic=lPicCon.begin();
				itrPic != lPicCon.end();
				itrPic++) {//each Picture*
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
		std::ifstream in(Application::_configUrl);
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
					//set location
					picItem->setLoc(locationItem);
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
				int parentId = (*itr)["parentMap"].asInt();
				Map*        parentMap = this->_mapService->getMapById(parentId);
				int         coordianteX = (*itr)["coordiante"]["x"].asInt();
				int         coordianteY = (*itr)["coordiante"]["y"].asInt();
				//new a item
				Map* mapItem = new Map(picName, description, parentMap, coordianteX, coordianteY);
				//set Id
				mapItem->_Id = (*itr)["id"].asInt();
				//insert to parent map
				if (nullptr != parentMap) {
					this->_mapService->addMap(mapItem);
				}
				//root map
				else {
					this->_mapService->_rootMap = mapItem;
				}

				//set location
				Location* location = this->_locationService->getLocation((*itr)["location"].asString());
				mapItem->setLoc(location);
				location->_map = mapItem;
			}
			this->_locationService->_currentLocation =
				this->_mapService->_rootMap->getLoc();
			return true;
		}//parse json successful
		else {
			return false;
		}//fail to parse json

	}catch (...) {
		return false;
	}
}

std::vector<std::string> Application::parseCommond(std::string commond) {
	std::vector<std::string> ret;
	int firstFlag = 0, secondFlag = 0;
	int length = commond.size();
	while (firstFlag < length) {
		while (commond[secondFlag] != ' ' && secondFlag != length) {
			secondFlag++;
		}
		std::string cmd = commond.substr(firstFlag, secondFlag - firstFlag);
		ret.push_back( cmd );
		//ignore all the space
		firstFlag = secondFlag;
		while (firstFlag < length && commond[firstFlag] == ' ') {
			firstFlag++;
		}
		secondFlag = firstFlag;
	}
	return ret;
}

bool Application::dispatchCommond(std::vector<std::string> comList) {
	if (3 < comList.size()) {
		return false;
	}

	
	std::string commond = comList.front();

	//exit
	if (0 == commond.compare("exit")) {
		return false;
	}

	//cd
	else if(0 == commond.compare("cd")) {
		this->cdLoc(comList[1]);
	}

	//mkloc
	else if(0 == commond.compare("mkloc")) {
		this->mkLoc(comList[1]);
	}

	//deloc
	else if (0 == commond.compare("deloc")) {
		this->deLoc(comList[1]);
	}

	//lsloc
	else if (0 == commond.compare("lsloc")) {
		this->lsLoc();
	}

	//lspic
	else if (0 == commond.compare("lspic")) {
		this->lsPic();
	}

	//adpic
	else if (0 == commond.compare("adpic")) {
		this->adPic(comList[1], comList[2]);
	}

	//depic
	else if (0 == commond.compare("depic")) {
		this->dePic(comList[1]);
	}

	//ldpic
	else if (0 == commond.compare("ldpic")) {
		this->ldPic(comList[1]);
	}

	//ldmap
	else if (0 == commond.compare("ldmap")) {
		this->ldMap();
	}

	//admap
	else if (0 == commond.compare("admap")) {
		this->adMap(comList[1], comList[2]);
	}

	//demap
	else if (0 == commond.compare("demap")) {
		this->deMap();
	}
	
	//setcor
	else if (0 == commond.compare("setcor")) {
		this->setCoordinate(atoi(comList[1].c_str()), atoi(comList[2].c_str()));
	}

	//cor
	else if (0 == commond.compare("cor")) {
		this->getCoordiante(comList[1]);
	}

	//setdes
	else if (0 == commond.compare("setdes")) {
		this->setPicDescription(comList[1], comList[2]);
	}

	//des
	else if (0 == commond.compare("des")) {
		this->getPicDescription(comList[1]);
	}
	else
	{
		std::cout << "No such commond!" << std::endl;
	}

	return true;


}

/***************************commond********************************************************/

//cdloc -- check
void Application::cdLoc(std::string name) {
	Location* curVal = this->_locationService->_currentLocation;
	if (0 == name.compare("..")) {
		Location* parentLoc = curVal->getParentLocation();
		this->_locationService->_currentLocation = (parentLoc == nullptr ? curVal : parentLoc);
		return;
	}
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

//mkloc -- check
void Application::mkLoc(std::string name) {
	Location* newloc = new Location();
	Map* newmap = new Map();

	//set map info
	newmap->setLoc(newloc);
	//newmap->_parentMap = this->_locationService->_currentLocation->_map;
	Location* curLoc = this->_locationService->_currentLocation;
	newmap->_parentMap = (nullptr == curLoc ? nullptr : curLoc->_map);
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

//deloc -- check
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

//lsloc -- check
void Application::lsLoc() {
	int size = 1;
	
	std::list<Location*> allLoc = this->_locationService->_currentLocation->getSubLocation();
	for (auto itr = allLoc.begin(); itr != allLoc.end(); itr++, size++) {
		setStyle(1);
		cout << (*itr)->_locationName;
		setStyle(2);
		std::cout << (0 == size % 10 ? "\n" : "  ");
	}
	std::cout << '\n';
}

//lsPic -- check
void Application::lsPic() {
	int size = 0;
	auto picContainer = this->_locationService->_currentLocation->getPictureContainer();
	for (auto itr = picContainer.begin(); itr != picContainer.end(); itr++) {
		setStyle(1);
		std::cout << (*itr)->getName();
		setStyle(2);
		std::cout << (0 == size % 3 ? "\n" : "  ");
	}
}

//adpic -- check
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

//depic -- check
void Application::dePic(std::string name) {
	Location* curLoc = this->_locationService->_currentLocation;
	
	if(false == curLoc->deletePictureFromContainer(name)){
		std::cout << "No such picture" << endl;
	}
	
}

//ldpic -- check
void Application::ldPic(std::string name) {
	std::list<Picture*> picCon = this->_locationService->_currentLocation->getPictureContainer();
	//search for Picture
	for (auto itr = picCon.begin(); itr != picCon.end(); itr++) {
		if (0 == name.compare((*itr)->getName())) {
			std::string picUrl = (*itr)->getUrl();
			//char to tchar
			int iLength = MultiByteToWideChar(CP_ACP, 0, picUrl.c_str(), strlen(picUrl.c_str()) + 1, NULL, 0);
			TCHAR* tPicUrl = new TCHAR[iLength];
			MultiByteToWideChar(CP_ACP, 0, picUrl.c_str(), strlen(picUrl.c_str()) + 1, tPicUrl, iLength);
			//new a dialog
			CPicZoomDlg dlg(tPicUrl);
			dlg.DoModal();
			return;
		}
	}
	std::cout << "Error!No such picture!" << std::endl;
}

//ldmap -- check
void Application::ldMap() {
	Location* curLoc = this->_locationService->_currentLocation;

	if (0 == curLoc->_map->getName().compare("")) {
		std::cout << "No map here!" << endl;
		return;
	}

	std::string mapUrl = curLoc->_map->getUrl();
	//char to tchar
	int iLength = MultiByteToWideChar(CP_ACP, 0, mapUrl.c_str(), strlen(mapUrl.c_str()) + 1, NULL, 0);
	TCHAR* tMapUrl = new TCHAR[iLength];
	MultiByteToWideChar(CP_ACP, 0, mapUrl.c_str(), strlen(mapUrl.c_str()) + 1, tMapUrl, iLength);
	//new a dialog
	CPicZoomDlg dlg(tMapUrl);
	dlg.DoModal();
}

//admap -- check
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
	//delete from loc
	curLoc->_map->setName("");

}

//setcor
void Application::setCoordinate(int x, int y) {
	Location* curLoc = this->_locationService->_currentLocation;
	Map*      curMap = curLoc->getMap();
	Map*      parMap = curMap->getParentMap();
	if (false == parMap->setSubMapCor(curMap->getCoordiante(), Coordinate(x, y))) {
		std::cout << "Error!" << std:: endl;
	}
}

//getcor
void Application::getCoordiante(std::string name){

	Location* curLoc = this->_locationService->_currentLocation;
	Map*      curMap = curLoc->_map;
	Map*      tarMap;
	//get target map
	if (0 == name.compare("self")) {
		tarMap = curMap;
	}
	else {
		auto locLis = curLoc->getSubLocation();
		for (auto itr = locLis.begin(); itr != locLis.end(); itr++) {
			if (0 == (*itr)->_locationName.compare(name)) {
				tarMap = (*itr)->_map;
			}
		}
	}
	//output
	if (0 > tarMap->_coordinate.x || 0 > tarMap->_coordinate.y) {
		std::cout << "unset!" << std::endl;
	}
	else {
		std::cout << tarMap->getCoordiante().x;
		std::cout << "	";
		std::cout << tarMap->getCoordiante().y << endl;
	}
}

//setdes
void Application::setPicDescription(std::string name, std::string des) {
	Location* curLoc = this->_locationService->_currentLocation;
	auto      picLis = curLoc->getPictureContainer();

	for (auto itr = picLis.begin(); itr != picLis.end(); itr++) {
		if (0 == (*itr)->getName().compare(name)) {
			(*itr)->setDiscription(des);
			std::cout << (*itr)->getName() << ": " << endl;
			std::cout << (*itr)->getDiscription() << std::endl;
		}
	}
}

//des
void Application::getPicDescription(std::string name) {
	Location* curLoc = this->_locationService->_currentLocation;
	auto      picLis = curLoc->getPictureContainer();

	for (auto itr = picLis.begin(); itr != picLis.end(); itr++) {
		if (0 == (*itr)->getName().compare(name)) {
			std::cout << (*itr)->getName() << ": " << endl;
			std::cout << (*itr)->getDiscription() << std::endl;
		}
	}
}

/************************************run**********************************************/


void Application::run() {

	this->outputPath();
	while (1) {
		std::string cmd;
		getline(std::cin,cmd);
		std::vector<std::string> cmdList = this->parseCommond(cmd);
		if (!this->dispatchCommond(cmdList)) {
			break;
		}
		this->outputPath();
	}
}

void Application::outputPath() {
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
		std::cout << (*itr == path.back() ? "> " : "/ ");
	}
}