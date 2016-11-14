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
	list<picture> getPicByOrder();  //用链表顺序存取
	priority_queue<picture> getPicBytag();    //有priority_queue顺序存取
	~location();
private:
	map* m;
	Iterator<picture> picIterator;
};

