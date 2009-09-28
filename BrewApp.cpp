#include "BrewApp.h"
#include "BrewApp.bid"
#include "BrewApp_res.h"

extern "C" int AEEClsCreateInstance(AEECLSID ClsId, IShell* piShell, IModule* piModule, void** ppApplet) {
	*ppApplet = NULL;
	AEEApplet* pMe = NULL;

    if (ClsId == AEECLSID_BREWAPP) {
	    if (AEEApplet_New(sizeof(BrewApp),
							ClsId,
							piShell, 
							piModule, 
							(IApplet**) ppApplet,
							(AEEHANDLER) BrewApp::HandleEvent,
							(PFNFREEAPPDATA) BrewApp::Destroy) ) {

			pMe = (AEEApplet*) (*ppApplet);
			if (pMe && BrewApp::Init((BrewApp*) pMe)) {
				return AEE_SUCCESS;
			}
			else if (pMe) {
                IApplet_Release((IApplet*)*ppApplet);
			}
		}
	}

    return(AEE_EFAILED);
}


boolean BrewApp::Init(BrewApp* pApp) {
	pApp->deviceInfo.wStructSize = sizeof(pApp->deviceInfo);
    ISHELL_GetDeviceInfo(pApp->m_pIShell,&pApp->deviceInfo);
	return TRUE;
}

void BrewApp::Destroy(BrewApp* pApp) {
}

boolean BrewApp::HandleEvent(BrewApp* pApp, AEEEvent eventCode, uint16 wParam, uint32 dwParam) {

    switch (eventCode) {
        // Event to inform app to start, so start-up code is here:
        case EVT_APP_START:
            pApp->DrawScreen(); // Draw text on display screen.
            return(TRUE);         

        // Event to inform app to exit, so shut-down code is here:
        case EVT_APP_STOP:
      	    return(TRUE);

        // Event to inform app to suspend, so suspend code is here:
        case EVT_APP_SUSPEND:
      	    return(TRUE);

        // Event to inform app to resume, so resume code is here:
        case EVT_APP_RESUME:
            pApp->DrawScreen(); // Redraw text on display screen.
      	    return(TRUE);

        // An SMS message has arrived for this app. 
        // The Message is in the dwParam above as (char *).
        // sender simply uses this format "//BREW:ClassId:Message", 
        // example //BREW:0x00000001:Hello World
        case EVT_APP_MESSAGE:
      	    return(TRUE);

        // A key was pressed:
        case EVT_KEY:
      	    return(FALSE);

        // Clamshell has opened/closed
        // wParam = TRUE if open, FALSE if closed
        case EVT_FLIP:
            return(TRUE);
      
	    // Clamtype device is closed and reexposed when opened, and LCD 
        // is blocked, or keys are locked by software. 
        // wParam = TRUE if keygaurd is on
        case EVT_KEYGUARD:
            return (TRUE);

        // If event wasn't handled here, then break out:
        default:
            break;
    }
    return (FALSE); // Event wasn't handled.
}


void BrewApp::DrawScreen() {

    AECHAR szBuf[64] = {0};
    int nStrLen = 0;
    RGBVAL oldTextColor;

    IDISPLAY_ClearScreen(this->m_pIDisplay); // Erase whole screen.

    // Load the string resource into a buffer:
    nStrLen = ISHELL_LoadResString(this->m_pIShell, 
                                   BREWAPP_RES_FILE,
                                   IDS_STRING_1001, szBuf, sizeof(szBuf));

    // If the text was successfully loaded from resource file into buffer:
    if (0 <  nStrLen) {
        // Set user-text color to black:
        oldTextColor = IDisplay_SetColor(this->m_pIDisplay, 
                                         CLR_USER_TEXT, 
                                         RGB_BLACK);

        IDisplay_DrawText(this->m_pIDisplay, // Display instance.
                        AEE_FONT_BOLD, // Use Bold font.
                        szBuf,         // String buffer containing text.
                        -1,     // Automatically compute string length.
                        0,      // x-coordinate ignored since IDF_ALIGN_CENTER.
                        0,      // y-coordinate ignored since IDF_ALIGN_MIDDLE.
                        NULL,   // No clipping.
                        IDF_ALIGN_CENTER | // Center-align horizontally.
                        IDF_ALIGN_MIDDLE); // Middle-align vertically.
    }

    // Restore previous color:
    IDisplay_SetColor(this->m_pIDisplay, CLR_USER_TEXT, oldTextColor);
    IDisplay_Update (this->m_pIDisplay);
}
