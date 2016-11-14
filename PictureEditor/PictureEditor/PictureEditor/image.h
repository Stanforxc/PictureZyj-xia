#pragma once
#include "location.h"
#include<string>
#include<vector>

using namespace std;

class image
{
public:
	image();
	virtual bool read() = 0 ;
	virtual bool save(string url) = 0;
	virtual bool del(string url) = 0;
	virtual location* getLoc() = 0;
	virtual bool setLoc( location* loc ) = 0;
	virtual string getName() { return picName; }
	virtual bool setName(string name) { this->picName = name; }
	virtual bool setSolution();   //设置分辨率
	virtual bool undoSolution();  //撤销设置
	virtual bool redoSolution();  //重新设置
	virtual bool setDiscription();  //设置评论
	virtual string getDiscription(string url);  //获取评论
	virtual void zoomOut();   //放大
	virtual void zoomIn();    //缩小
	virtual void rotate();
	~image();
private:
	location* loc;
	string url;
	vector<string> urlPrev;  //记录先前的分辨率版本
	string picName;
	string description;
};

