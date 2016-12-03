#include"../../Header/Service/Application.h"
#include<exception>
#include<fstream>

std::string Application::_configUrl = "Config/config.json";

Application::Application() {
	this->_locationService = new LocationService();
	this->_mapService = new MapService();
	this->loadConfig();
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
				itrPic != (*itr)->getPictureContainer().end(); itrPic++) {
				picContainer.append(*itrPic);
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
				for (auto itrPic = (*itr)["pictureContainer"].begin(); itrPic != (*itr)["pictureContainer"].end(); itrPic++) {
					locationItem->_pictureContainer.push_back((*itrPic).asString());
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
				std::string url = (*itr)["url"].asString();
				std::string picName = (*itr)["pictureName"].asString();
				std::string description = (*itr)["description"].asString();
				Map*        parentMap = this->_mapService->getMapById((*itr)["parentMap"].asInt());
				int         coordianteX = (*itr)["coordiante"]["x"].asInt();
				int         coordianteY = (*itr)["coordiante"]["y"].asInt();
				//new a item
				Map* mapItem = new Map(url, picName, description, parentMap, coordianteX, coordianteY);
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