#include "SDeskItemStopPlay.h"
#include "SDesk.h"

SDeskItemStopPlay::SDeskItemStopPlay(
    const char* name)
    : SDeskItem(name, 0, false)
{
    Log(7, "SDeskItemStopPlay(%s)\n", name);

    // Track this items type.
    mType = I_STOP_PLAY;

    Log(7, "SDeskItemStopPlay() done\n");
}

SDeskItemStopPlay::~SDeskItemStopPlay()
{
    Log(7, "~SDeskItemStopPlay()\n");

    Log(7, "~SDeskItemStopPlay() done\n");
}

bool SDeskItemStopPlay::OnKeyDown(u32 nKey)
{
    char buf[1024];
    bool val;

    Log(7, "SDeskItemStopPlay::OnKeyDown(%d)\n", nKey);

    if (nKey == CK_PLAY || nKey == CK_PAUSE || nKey == CK_PLAYPAUSE ||
        nKey == CK_SELECT || nKey == CK_EAST) {
        // Build the commandline.
        Log(7, "SDeskItemStopPlay::OnKeyDown(%s)\n", buf);

        sprintf(buf, "\"%s/apps/stopplay\"",
                SDesk::getInstance().getRootDir());
        val = CascadeSpawn::Spawn(buf);
        if (!val) {
            Log(1, "Could not spawn!\n");
            SDesk::getInstance().getCurrentTheme().SetStatusMessage(
                "Unable to stop playing music...");
        } else {
            SDesk::getInstance().getCurrentTheme().SetStatusMessage(
                "Stopped music...");
        }
        Log(7, "SDeskItemStopPlay::OnKeyDown() done true\n");
        return true;
    }

    Log(7, "SDeskItemStopPlay::OnKeyDown() done\n");
    return false;
}

