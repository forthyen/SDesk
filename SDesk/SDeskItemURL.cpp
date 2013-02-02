#include "SDesk.h"
#include "SDeskItemURL.h"

SDeskItemURL::SDeskItemURL(
    const char* name,
    const char* filename,
    bool prot)
    : SDeskItem(name, filename, prot)
{
    Log(7, "SDeskItemURL(%s,%s)\n", name, filename);

    // Track this items type.
    mType = I_APP;
    mURL = CascadeString(filename);

    Log(7, "SDeskItemURL() done\n");
}

SDeskItemURL::~SDeskItemURL()
{
    Log(7, "~SDeskItemURL()\n");

    Log(7, "~SDeskItemURL() done\n");
}

bool SDeskItemURL::OnKeyDown(u32 nKey)
{
    const char *url = mURL;
    CascadeHTTPClient myClient; 

    Log(7, "SDeskItemURL::OnKeyDown(%d)\n", nKey);

    if (nKey == CK_PLAY || nKey == CK_PAUSE || nKey == CK_PLAYPAUSE ||
        nKey == CK_SELECT || nKey == CK_EAST) {
        // Build the commandline.
        if (strlen(url) > 0) {
            // Run the command.
            Log(7, "SDeskItemURL::OnKeyDown(%s)\n", url);

            myClient.SetURL(url); 
            myClient.GET(); 

            SDesk::getInstance().getCurrentTheme().SetStatusMessage(
                "Sent url request...");

            u32 buffsize = 8192; 

            char buff[buffsize]; 
            u32 bytesReturned = 0; 
            u32 totalBytes = 0; 

            while(1) { 
                bytesReturned = myClient.ReadBytes(buff,buffsize); 
                if (0 == bytesReturned) {
                    break; 
                }
                totalBytes += bytesReturned; 
                
                if (bytesReturned < buffsize) {
                    break; 
                }
            }

            Log(7, "SDeskItemURL::OnKeyDown() done true\n");
        }
        return true;
    }

    Log(7, "SDeskItemURL::OnKeyDown() done\n");
    return false;
}

