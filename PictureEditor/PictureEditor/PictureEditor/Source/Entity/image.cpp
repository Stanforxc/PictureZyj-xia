#include "../../Header/Entity/image.h"




Image::Image()
{
	loc = new Location();
	url = "test.jpg";    //默认为当前路径
	dirUrl = "";  //默认文件夹路径
	picName = "";
	description = "";
}

Image::Image(string url, string picName, string des) {
	this->url = url;
	this->picName = picName;
	this->description = des;
}


Image::~Image()
{
	delete loc;
}


//加载单张图片
bool Image::read(string u) {
	CImage* Image = new CImage();
	Image->Load(u.c_str);

	try {
		if (Image->IsNull) {
			throw 1;
			return false;
		}

		map<string, vector<CImage> >::iterator it = imageMap.find(u);
		if (it != imageMap.end()) {
			it->second.push_back(*Image);
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
	
	delete Image;
}

//加载文件加下所有文件
bool Image::readDir(string dUrl, int& sucRate) {
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

void Image::getFiles(string path, vector<string>& files)
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

bool Image::save(string dUrl) {
	CImage* Image = new CImage();
	string name = "test.jpg";    //文件名 默认为"test.jpg"

	string comb = dUrl + name;
	Image->Save(comb.c_str);

	//查看是否保存成功
	Image->Load(comb.c_str);

	if (Image->IsNull) {
		return false;
	}
	return true;

	delete Image;
}

bool Image::del(string url) {
	remove(url.c_str);

	//查看是否删除成功

	CImage* Image = new CImage();

	if (Image->IsNull) {
		return true;
	}
	return false;

	delete Image;
}

bool Image::getSolution(CImage* Image,int& width, int& height) {
	width = Image->GetWidth;
	height = Image->GetHeight;
	return true;
}

bool Image::setSolution(CImage *pImage, CImage *ResultImage, int StretchHeight, int StretchWidth) {
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


bool Image::undoSolution() {
	urlPrev.erase(urlPrev.end() - 1);
}

bool Image::redoSolution() {
	urlPrev.push_back(url);
}

bool Image::setDiscription(string comment) {
	description = comment;
}

string Image::getDiscription() {
	return description;
}

void Image::rotate(CImage* image) {
	//hahaha
}

HBITMAP Image::GetRotatedBitmap(HBITMAP hBitmap, float radians, COLORREF clrBack) {
	CDC sourceDC, destDC;
	sourceDC.CreateCompatibleDC(NULL);
	destDC.CreateCompatibleDC(NULL);

	BITMAP bm;
	::GetObject(hBitmap, sizeof(bm), &bm);

	float cosine = (float)cos(radians);
	float sine = (float)sin(radians);

	int x1 = (int)(bm.bmHeight * sine);
	int y1 = (int)(bm.bmHeight * cosine);
	int x2 = (int)(bm.bmWidth * cosine + bm.bmHeight * sine);
	int y2 = (int)(bm.bmHeight * cosine - bm.bmWidth * sine);
	int x3 = (int)(bm.bmWidth * cosine);
	int y3 = (int)(-bm.bmWidth * sine);

	int minx = min(0, min(x1, min(x2, x3)));
	int miny = min(0, min(y1, min(y2, y3)));
	int maxx = max(0, max(x1, max(x2, x3)));
	int maxy = max(0, max(y1, max(y2, y3)));

	int w = maxx - minx;
	int h = maxy - miny;

	HBITMAP hbmResult = ::CreateCompatibleBitmap(CClientDC(NULL), w, h);

	HBITMAP hbmOldSource = (HBITMAP)::SelectObject(sourceDC.m_hDC, hBitmap);
	HBITMAP hbmOldDest = (HBITMAP)::SelectObject(destDC.m_hDC, hbmResult);


	HBRUSH hbrBack = CreateSolidBrush(clrBack);
	HBRUSH hbrOld = (HBRUSH)::SelectObject(destDC.m_hDC, hbrBack);
	destDC.PatBlt(0, 0, w, h, PATCOPY);
	::DeleteObject(::SelectObject(destDC.m_hDC, hbrOld));


	SetGraphicsMode(destDC.m_hDC, GM_ADVANCED);
	XFORM xform;
	xform.eM11 = cosine;
	xform.eM12 = -sine;
	xform.eM21 = sine;
	xform.eM22 = cosine;
	xform.eDx = (float)-minx;
	xform.eDy = (float)-miny;

	SetWorldTransform(destDC.m_hDC, &xform);

	destDC.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &sourceDC, 0, 0, SRCCOPY);


	::SelectObject(sourceDC.m_hDC, hbmOldSource);
	::SelectObject(destDC.m_hDC, hbmOldDest);

	return hbmResult;

}

std::string Image::getUrl() {
	return this->url;
}
