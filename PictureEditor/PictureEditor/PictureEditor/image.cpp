#include "image.h"




image::image()
{
	loc = new location();
	url = "test.jpg";    //默认为当前路径
	dirUrl = "";  //默认文件夹路径
	picName = "";
	description = "";
}

image::image(string url, string picName, string des) {
	this->url = url;
	this->picName = picName;
	this->description = des;
}


image::~image()
{
	delete loc;
}


//加载单张图片
bool image::read(string u) {
	CImage* image = new CImage();
	image->Load(u.c_str);

	try {
		if (image->IsNull) {
			throw 1;
			return false;
		}

		map<string, vector<CImage> >::iterator it = imageMap.find(u);
		if (it != imageMap.end()) {
			it->second.push_back(*image);
		}
		else {
			imageMap.insert(make_pair(u, vector<CImage>()));
		}

		
	}
	catch (int i) {
		cerr << "照片不存在，请重新输入路径" << endl;
		string newUrl;
		cin >> newUrl;
		url = newUrl;
	}
	
	delete image;
}

//加载文件加下所有文件
bool image::readDir(string dUrl, int& sucRate) {
	//获取文件夹下所有文件名
	vector<string> files;
	getFiles(dUrl, files);
	int countLoad = 0;
	for (int i = 0; i < files.size(); ++i) {
		if (read(files[i])) {
			countLoad++;
		}
	}
	sucRate = countLoad;
	if (sucRate == files.size())
	{
		return true;
	}
	return false;

}

void image::getFiles(string path, vector<string>& files)
{
	//文件句柄  
	long   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

bool image::save(string dUrl) {
	CImage* image = new CImage();
	string name = "test.jpg";    //文件名 默认为"test.jpg"

	string comb = dUrl + name;
	image->Save(comb.c_str);

	//查看是否保存成功
	image->Load(comb.c_str);

	if (image->IsNull) {
		return false;
	}
	return true;

	delete image;
}

bool image::del(string url) {
	remove(url.c_str);

	//查看是否删除成功

	CImage* image = new CImage();

	if (image->IsNull) {
		return true;
	}
	return false;

	delete image;
}

bool image::getSolution(CImage* image,int& width, int& height) {
	width = image->GetWidth;
	height = image->GetHeight;
	return true;
}

bool image::setSolution(CImage *pImage, CImage *ResultImage, int StretchHeight, int StretchWidth) {
	if (pImage->IsDIBSection) {	
		// 取得 pImage 的 DC
		CDC* pImageDC1 = CDC::FromHandle(pImage->GetDC);
		CBitmap* bitmap1 = pImageDC1->GetCurrentBitmap();
		BITMAP bmpInfo;
		bitmap1->GetBitmap(&bmpInfo);

		// 建立新的 CImage
		ResultImage->Create(StretchWidth, StretchHeight, bmpInfo.bmBitsPixel);
		CDC* ResultImageDC = CDC::FromHandle(ResultImage->GetDC());

		ResultImageDC->SetStretchBltMode(HALFTONE);

		SetBrushOrgEx(ResultImageDC->m_hDC, 0, 0, NULL);

		StretchBlt(*ResultImageDC, 0, 0, StretchWidth, StretchHeight, *pImageDC1, 0, 0, pImage->GetWidth(), pImage->GetHeight(), SRCCOPY);

		pImage->ReleaseDC();
		ResultImage->ReleaseDC();
	}
}
