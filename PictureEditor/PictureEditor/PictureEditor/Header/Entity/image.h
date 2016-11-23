#pragma once

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "location.h"
#include<string>
#include<vector>
#include<map>
#include<atlimage.h>
#include<iostream>
#include<io.h>
#include<Afxwin.h>

using namespace std;

class Image
{
public:
	Image();
	Image(string url, string picName, string des);
	virtual bool read(string url);
	void getFiles(string path, vector<string>& files);  //获取文件名
	virtual bool readDir(string dUrl, int& sucRate);
	virtual bool save(string url);
	virtual bool del(string url);
	virtual Location* getLoc() { return loc; }
	virtual bool setLoc(Location* loca) { loc = loca; }
	virtual string getName() { return picName; }
	virtual bool setName(string name) { this->picName = name; }
	virtual bool getSolution(CImage* image, int& width, int& height);
	virtual bool setSolution(CImage *pImage, CImage *ResultImage, int StretchHeight, int StretchWidth);   //设置分辨率



	virtual bool undoSolution();  //撤销设置
	virtual bool redoSolution();  //重新设置
<<<<<<< HEAD
	virtual bool setDiscription();  //设置评论
	virtual string getDiscription(string url);  //获取评论
	virtual void zoomOut();   //放大
	virtual void zoomIn();    //缩小
	virtual void rotate();
	virtual string getPictureUrl();
=======
	virtual bool setDiscription(string comment);  //设置评论
	virtual string getDiscription();  //获取评论

	virtual HBITMAP GetRotatedBitmap(HBITMAP hBitmap, float radians, COLORREF clrBack);
	virtual void rotate(CImage* image);
>>>>>>> 90e98604eab7726b241c17a1c44cf494bc5a4a5b
	~Image();
private:
	Location* loc;
	string url;  //图片url
	string dirUrl;  //文件夹url
	vector<string> urlPrev;  //记录先前的分辨率版本
	map<string, vector<CImage> >  imageMap;
	string picName;
	string description;
};


#endif




