#include"picture.h"

Picture::Picture(): Image() {
	this->_map = nullptr;
}

Picture::Picture(std::string url, std::string picName, std::string des, Map* map) : Image(url, picName, des) {
	this->_map = map;
}

Picture::~Picture() {

}

list<Location*>  Picture::getLocationPath() {
	return this->_locationPath;
}

Map*  Picture::getMap() {
	return this->_map;
}