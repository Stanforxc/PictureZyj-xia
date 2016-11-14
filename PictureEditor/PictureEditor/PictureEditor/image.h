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
	string url;
	vector<string> urlPrev;  //��¼��ǰ�ķֱ��ʰ汾
	string picName;
	string description;
};

