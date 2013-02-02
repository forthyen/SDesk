#include "SDesk.h"
#include "SDeskItemRadio.h"

SDeskItemRadio::SDeskItemRadio(
    const char* name,
    const char* filename,
    bool prot)
    : SDeskItem(name, filename, prot)
{
    Log(7, "SDeskItemRadio(%s,%s)\n", name, filename);

    // Track this items type.
    mType = I_RADIO;

    Log(7, "SDeskItemRadio() done\n");
}

SDeskItemRadio::~SDeskItemRadio()
{
    Log(7, "~SDeskItemRadio()\n");

    Log(7, "~SDeskItemRadio() done\n");
}

bool SDeskItemRadio::OnKeyDown(u32 nKey)
{
    const char *str = mFilename;
    char buf[1024];
    bool val;

    Log(1, "SDeskItemRadio::OnKeyDown(%d)\n", nKey);

    if (nKey == CK_PLAY || nKey == CK_PAUSE || nKey == CK_PLAYPAUSE ||
        nKey == CK_SELECT || nKey == CK_EAST) {
        // Build the commandline.
        if (SDesk::getInstance().getRadioPlayer() == 0) {
            Log(1, "SDeskItemRadio::OnKeyDown() no radio player configured!\n");
        }

        sprintf(buf, "/bin/sh \"%s\" \"%s\" \"%s\"",
                SDesk::getInstance().getPlayradio(), 
                SDesk::getInstance().getRadioPlayer(), str);
        Log(7, "SDeskItemRadio::OnKeyDown(%s)\n", buf);

        // Run the commandline
        val = CascadeSpawn::Spawn(buf);
        if (!val) {
            string* message = new string("Unable to run playradio script.");
            SDesk::getInstance().AddMessage(message);
            Log(1, "Could not spawn!\n");
            SDesk::getInstance().getCurrentTheme().SetStatusMessage(
                "Unable to start radio...");
        } else {
            SDesk::getInstance().getCurrentTheme().SetStatusMessage(
                "Started radio...");
        }
        Log(7, "SDeskItemRadio::OnKeyDown() done true\n");
        return true;
    }

    Log(7, "SDeskItemRadio::OnKeyDown() done\n");
    return false;
}

CascadeString& SDeskItemRadio::getItemDefinition()
{
    char buffer[1024];

    Log(7, "SDeskItemRadio::getItemDefinition()\n");

    strcpy(buffer, "%radio\n");
    strcat(buffer, mName);
    strcat(buffer, "\n");
    strcat(buffer, mFilename);
    strcat(buffer, "\n");
    if (mProtected) {
        strcat(buffer, "1\n");
    } else {
        strcat(buffer, "0\n");
    }
    mItemDefinition = CascadeString(buffer);

    Log(7, "SDeskItemRadio::getItemDefinition() done\n");
    return mItemDefinition;
}
