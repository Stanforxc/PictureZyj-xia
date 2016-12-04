#include"../../Header/Entity/picture.h"

Picture::Picture(): Image() {
}

Picture::Picture(std::string picName, std::string des) : Image(picName, des) {
}

Picture::~Picture() {

}

std::list<Location*>  Picture::getLocationPath() {
	Location* loc = this->getLoc();
	std::list<Location*> ret;
	while (loc != nullptr) {
		ret.push_front(loc);
		loc = loc->getParentLocation();
	}
	return ret;
}
