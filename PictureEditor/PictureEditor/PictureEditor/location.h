#pragma once
#include "map.h"
#include "Iterator.h"
#include "picture.h"
#include<set>
#include<list>
#include<queue>


using namespace std;
class location
{
public:
	location();
	list<picture> getPicByOrder();  //������˳���ȡ
	priority_queue<picture> getPicBytag();    //��priority_queue˳���ȡ
	~location();
private:
	map* m;
	Iterator<picture> picIterator;
};

