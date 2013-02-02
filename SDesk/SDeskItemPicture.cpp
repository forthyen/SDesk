#include "SDesk.h"
#include "SDeskItemPicture.h"
#include "SDeskItemMenu.h"

SDeskItemPicture::SDeskItemPicture(
    const char* name,
    const char* filename,
    bool prot)
    : SDeskItem(name, filename, prot)
{
    Log(7, "SDeskItemPicture(%s,%s)\n", name, filename);

    mType = I_PICTURE;

    Log(7, "SDeskItemPicture() done\n");
}

SDeskItemPicture::~SDeskItemPicture()
{
    Log(7, "~SDeskItemPicture()\n");

    Log(7, "~SDeskItemPicture() done\n");
}

bool SDeskItemPicture::OnKeyDown(u32 nKey)
{
    const char *str = mFilename;
    char buf[1024];
    bool val;

    Log(1, "SDeskItemPicture::OnKeyDown(%d)\n", nKey);

    if (nKey == CK_PLAY || nKey == CK_PAUSE || nKey == CK_PLAYPAUSE ||
        nKey == CK_SELECT || nKey == CK_EAST) {
        // Build the commandline.
        if (SDesk::getInstance().getShowpicture() == 0) {
            Log(1, "SDeskItemPicture::Activate() no showpicture configured!\n");
        }

        sprintf(buf, "/bin/sh \"%s\" \"%s\" \"%s\"",
                SDesk::getInstance().getShowpicture(), 
                SDesk::getInstance().getPictureViewer(), str);
        Log(1, "SDeskItemPicture::OnKeyDown(%s)\n", buf);

        // Run the commandline
        val = CascadeSpawn::Spawn(buf);
        if (!val) {
            string* message = new string("Unable to run showpicture script.");
            SDesk::getInstance().AddMessage(message);
            Log(1, "Could not spawn!\n");
            SDesk::getInstance().getCurrentTheme().SetStatusMessage(
                "Unable to show picture...");
        } else {
            SDesk::getInstance().getCurrentTheme().SetStatusMessage(
                "Showing picture...");
        }
        Log(1, "SDeskItemPicture::OnKeyDown() done true\n");
        return true;
    }

    Log(1, "SDeskItemPicture::OnKeyDown() done\n");
    return false;
}

CascadeString& SDeskItemPicture::getItemDefinition()
{
    char buffer[1024];

    Log(7, "SDeskItemPicture::getItemDefinition\n");

    strcpy(buffer, "%picture\n");
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

    Log(7, "SDeskItemPicture::getItemDefinition() done\n");
    return mItemDefinition;
}

