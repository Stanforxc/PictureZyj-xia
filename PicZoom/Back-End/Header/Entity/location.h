#pragma once

#ifndef _LOCATION_H_
#define _LOCATION_H_


#include<set>
#include<list>
#include<queue>


extern class picture;
extern class Map;

class Location
{
	friend class LocationService;
	friend class Application;
public:
	Location();
	~Location();
	Location(std::string locationName, std::string description, Map* map);


	//getter & setter
	Map* getMap();
	void  setMap(Map* map);
	

	std::list <std::string>    getPictureContainer();
	bool                       addToPictureContainer(std::string pictureToAdd);
	bool                       deletePictureFromContainer(std::string pictureToDelete);

	std::string                getLocationName();
	void                       setLocationName(std::string locationName);

	std::string                getDescription();
	void                       setDescription(std::string decription);
	std::list<Location*>       getSubLocation();

	
private:
	Map*                       _map;
	std::list<std::string>     _pictureContainer; //only store the url/name in this level, do not save the pic in the sublevel
	std::string                _locationName;
	std::string                _description;
};

#endif



