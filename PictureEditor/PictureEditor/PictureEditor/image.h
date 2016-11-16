#pragma once
#include "location.h"
#include<string>
#include<vector>
#include<map>
#include<atlimage.h>
#include<iostream>
#include<io.h>

using namespace std;

class image
{
public:
	image();
	image(string url,string picName,string des);
	virtual bool read(string url);
	void getFiles(string path, vector<string>& files);  //获取文件名
	virtual bool readDir(string dUrl, int& sucRate);
	virtual bool save(string url);
	virtual bool del(string url);
	virtual location* getLoc() { return loc; }
	virtual bool setLoc(location* loca) { loc = loca; }
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
	string url;  //图片url
	string dirUrl;  //文件夹url
	vector<string> urlPrev;  //记录先前的分辨率版本
	map<string, vector<CImage> >  imageMap;
	string picName;
	string description;
};

