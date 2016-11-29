#pragma once

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include<Afxwin.h>
#include<atlimage.h>
#include "location.h"
#include<string>
#include<vector>
#include<map>
#include<iostream>
#include<io.h>
#include "bmp.h"

using namespace std;

class Image
{
public:
	Image();
	Image(string url, string picName, string des);
	virtual bool read(string u);
	void getFiles(string path, vector<string>& files);  //获取文件名
	virtual bool readDir(string dUrl, int& sucRate);
	virtual bool save(string url);
	virtual bool del(string url);
	virtual Location* getLoc() { return loc; }
	virtual bool setLoc(Location* loca) { loc = loca; return true; }
	virtual string getName() { return picName; }
	virtual bool setName(string name) { this->picName = name; return true; }
	virtual bool getSolution(HBITMAP& hr, int& width, int& height);
	virtual HBITMAP setSolution(HBITMAP hr, int width, int height);   //设置分辨率



	virtual bool undoSolution();  //撤销设置
	virtual bool redoSolution();  //重新设置
	virtual bool setDiscription(string comment);  //设置评论
	virtual string getDiscription();  //获取评论

	virtual HBITMAP GetRotatedBitmap(HBITMAP hBitmap, float radians, COLORREF clrBack);
	virtual void rotate(CImage* image);
	virtual string getUrl();
	~Image();
private:
	Location* loc;
	string url;  //图片url
	string dirUrl;  //文件夹url
	vector<string> urlPrev;  //记录先前的分辨率版本
	map<string, vector<CImage> >  imageMap;
	string picName;
	string description;
	bmp vp;
};


#endif




