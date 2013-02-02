#include "SDesk.h"
#include "SDeskItemMovie.h"

SDeskItemMovie::SDeskItemMovie(
    const char* name,
    const char* filename,
    bool prot)
    : SDeskItem(name, filename, prot)
{
    Log(7, "SDeskItemMovie(%s,%s)\n", name, filename);

    // Track this items type.
    mType = I_MOVIE;

    Log(7, "SDeskItemMovie() done\n");
}

SDeskItemMovie::~SDeskItemMovie()
{
    Log(7, "~SDeskItemMovie()\n");

    Log(7, "~SDeskItemMovie() done\n");
}

bool SDeskItemMovie::OnKeyDown(u32 nKey)
{
    const char *str = mFilename;
    char buf[1024];
    bool val;

    Log(7, "SDeskItemMovie::OnKeyDown(%d)\n", nKey);

    if (nKey == CK_PLAY || nKey == CK_PAUSE || nKey == CK_PLAYPAUSE ||
        nKey == CK_SELECT || nKey == CK_EAST) {
        // Build the commandline.
        if (SDesk::getInstance().getMPlay() == 0) {
            Log(1, "SDeskItemMovie::OnKeyDown() no mplay configured!\n");
        }

        sprintf(buf, "/bin/sh \"%s\" \"%s\" \"%s\"",
                SDesk::getInstance().getPlaymovie(),
                SDesk::getInstance().getMPlay(), str);
        Log(7, "SDeskItemMovie::OnKeyDown(%s)\n", buf);

        // Run the commandline
        val = CascadeSpawn::Spawn(buf);
        if (!val) {
            string* message = new string("Unable to run playmovie script.");
            SDesk::getInstance().AddMessage(message);
            Log(1, "Could not spawn!\n");
            SDesk::getInstance().getCurrentTheme().SetStatusMessage(
                "Unable to start movie...");
        } else {
            SDesk::getInstance().getCurrentTheme().SetStatusMessage(
                "Started movie...");
        }
        Log(7, "SDeskItemMovie::OnKeyDown() done true\n");
        return true;
    }

    Log(7, "SDeskItemMovie::OnKeyDown() done\n");
    return false;
}

CascadeString& SDeskItemMovie::getItemDefinition()
{
    char buffer[1024];

    Log(7, "SDeskItemMovie::getItemDefinition\n");

    strcpy(buffer, "%movie\n");
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

    Log(7, "SDeskItemMovie::getItemDefinition done\n");
    return mItemDefinition;
}
