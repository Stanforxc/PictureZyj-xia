#pragma once

#ifndef _PROCESSMANAGER_H_
#define _PROCESSMANAGER_H_

#include "Back-End/Header/Service/Application.h"

class CPicZoomApp;

class RUN {
public:
	RUN();
	~RUN();
private:
	Application* m_pApp;
	CPicZoomApp* m_pPicZoom;
};

#endif
