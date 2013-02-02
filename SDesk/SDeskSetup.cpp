#include "SDesk.h"
#include "SDeskSetup.h"

SDeskSetup::SDeskSetup()
{
    Log(7, "SDeskSetup()\n");

    // The scan command to run.
    strcpy(mCommand, SDesk::getInstance().getRootDir());
    strcat(mCommand, "/SDeskScan");

    Log(7, "SDeskSetup() done\n");
}

SDeskSetup::~SDeskSetup()
{
    Log(7, "~SDeskSetup()\n");

    Log(7, "~SDeskSetup() done\n");
}

void SDeskSetup::ReScan(
    const char* type)
{
    char buffer[1024];
    bool val;

    Log(1, "SDeskSetup(%s)::ReScan\n", type);
    
    sprintf(buffer, "\"%s\" \"%s\" \"%s\"",
            mCommand, type, SDesk::getInstance().getRootDir());
    val = system(buffer);
    if (!val) {
        Log(1, "Could not spawn!\n");
    }
    SDesk::getInstance().setScanType("");
    SDesk::getInstance().getCurrentTheme().HidePopup();
    SDesk::getInstance().getCurrentTheme().Redraw();
    Log(7, "SDeskSetup()::ReScan done\n");
}

