#include "SDesk.h"
#include "SDeskItemApp.h"

SDeskItemApp::SDeskItemApp(
    const char* name,
    const char* filename,
    bool prot)
    : SDeskItem(name, filename, prot)
{
    Log(7, "SDeskItemApp(%s,%s)\n", name, filename);

    // Track this items type.
    mType = I_APP;

    Log(7, "SDeskItemApp() done\n");
}

SDeskItemApp::~SDeskItemApp()
{
    Log(7, "~SDeskItemApp()\n");

    Log(7, "~SDeskItemApp() done\n");
}

bool SDeskItemApp::OnKeyDown(u32 nKey)
{
    const char *str = mFilename;
    char buf[1024];
    bool val;

    Log(7, "SDeskItemApp::OnKeyDown(%d)\n", nKey);

    if (nKey == CK_PLAY || nKey == CK_PAUSE || nKey == CK_PLAYPAUSE ||
        nKey == CK_SELECT || nKey == CK_EAST) {
        // Build the commandline.
        if (strlen(str) > 0) {
            sprintf(buf, "\"%s\"", str);
            // Run the command.
            Log(7, "SDeskItemApp::OnKeyDown(%s)\n", buf);

            val = CascadeSpawn::Spawn(mFilename);
            if (!val) {
                Log(1, "Could not spawn!\n");
                SDesk::getInstance().getCurrentTheme().SetStatusMessage(
                    "Unable to start application...");
            } else {
                SDesk::getInstance().getCurrentTheme().SetStatusMessage(
                    "Started application...");
            }
            Log(7, "SDeskItemApp::OnKeyDown() done true\n");
        }
        return true;
    }

    Log(7, "SDeskItemApp::OnKeyDown() done\n");
    return false;
}

