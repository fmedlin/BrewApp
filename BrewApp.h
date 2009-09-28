#ifndef __BREWAPP_H__
#define __BREWAPP_H__

#include "AEEAppGen.h"
#include "AEEShell.h"

class BrewApp : public AEEApplet {
public:
	//interface
	static boolean Init(BrewApp* pApp);
	static boolean HandleEvent(BrewApp* pApp, AEEEvent eventCode, uint16 wParam, uint32 dwParam);
	static void Destroy(BrewApp* pApp);

	void DrawScreen();

	// implementation
	AEEDeviceInfo  deviceInfo;
};

#endif __BREWAPP_H__
