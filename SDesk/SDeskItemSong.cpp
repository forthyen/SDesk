#include "SDesk.h"
#include "SDeskItemSong.h"

SDeskItemSong::SDeskItemSong(
    const char* name,
    const char* filename,
    bool prot)
    : SDeskItem(name, filename, prot)
{
    Log(7, "SDeskItemSong(%s,%s)\n", name, filename);

    // Track this items type.
    mType = I_SONG;

    Log(7, "SDeskItemSong() done\n");
}

SDeskItemSong::~SDeskItemSong()
{
    Log(7, "~SDeskItemSong()\n");

    Log(7, "~SDeskItemSong() done\n");
}

bool SDeskItemSong::OnKeyDown(u32 nKey)
{
    const char *str = mFilename;
    char buf[1024];
    bool val;

    Log(7, "SDeskItemSong::OnKeyDown(%d)\n", nKey);

    if (nKey == CK_PLAY || nKey == CK_PAUSE || nKey == CK_PLAYPAUSE ||
        nKey == CK_SELECT || nKey == CK_EAST) {
        // Build the commandline.
        sprintf(buf, "/bin/sh \"%s\" \"%s\" \"%s\"",
                SDesk::getInstance().getPlaysong(),
                SDesk::getInstance().getMusicPlayer(), str);
        Log(7, "SDeskItemSong::OnKeyDown(%s)\n", buf);

        // Run the commandline
        val = CascadeSpawn::Spawn(buf);
        if (!val) {
            string* message = new string("Unable to run playsong script.");
            SDesk::getInstance().AddMessage(message);
            Log(1, "Could not spawn!\n");
            SDesk::getInstance().getCurrentTheme().SetStatusMessage(
                "Unable to play song...");
        } else {
            SDesk::getInstance().getCurrentTheme().SetStatusMessage(
                "Playing song...");
        }
        Log(7, "SDeskItemSong::OnKeyDown() done true\n");
        return true;
    }

    Log(7, "SDeskItemSong::OnKeyDown() done\n");
    return false;
}

CascadeString& SDeskItemSong::getItemDefinition()
{
    char buffer[1024];

    Log(7, "SDeskItemSong::getItemDefinition()\n");

    strcpy(buffer, "%song\n");
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

    Log(7, "SDeskItemSong::getItemDefinition() done\n");
    return mItemDefinition;
}

