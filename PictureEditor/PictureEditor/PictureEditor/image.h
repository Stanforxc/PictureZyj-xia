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
	void getFiles(string path, vector<string>& files);  //��ȡ�ļ���
	virtual bool readDir(string dUrl, int& sucRate);
	virtual bool save(string url);
	virtual bool del(string url);
	virtual location* getLoc() { return loc; }
	virtual bool setLoc(location* loca) { loc = loca; }
	virtual string getName() { return picName; }
	virtual bool setName(string name) { this->picName = name; }
	virtual bool setSolution();   //���÷ֱ���
	virtual bool undoSolution();  //��������
	virtual bool redoSolution();  //��������
	virtual bool setDiscription();  //��������
	virtual string getDiscription(string url);  //��ȡ����
	virtual void zoomOut();   //�Ŵ�
	virtual void zoomIn();    //��С
	virtual void rotate();
	~image();
private:
	location* loc;
	string url;  //ͼƬurl
	string dirUrl;  //�ļ���url
	vector<string> urlPrev;  //��¼��ǰ�ķֱ��ʰ汾
	map<string, vector<CImage> >  imageMap;
	string picName;
	string description;
};

