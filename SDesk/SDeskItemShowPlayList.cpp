#include "SDeskItemShowPlayList.h"
#include "SDesk.h"

SDeskItemShowPlayList::SDeskItemShowPlayList(
    const char* name)
    : SDeskItem(name, 0, false)
{
    Log(7, "SDeskItemShowPlayList(%s)\n", name);

    // Track this items type.
    mType = I_SHOW_PLAY_LIST;

    Log(7, "SDeskItemShowPlayList() done\n");
}

SDeskItemShowPlayList::~SDeskItemShowPlayList()
{
    Log(7, "~SDeskItemShowPlayList()\n");

    Log(7, "~SDeskItemShowPlayList() done\n");
}

bool SDeskItemShowPlayList::OnKeyDown(u32 nKey)
{
    char buf[1024];

    Log(7, "SDeskItemShowPlayList::OnKeyDown(%d)\n", nKey);

    if (nKey == CK_PLAY || nKey == CK_PAUSE || nKey == CK_PLAYPAUSE ||
        nKey == CK_SELECT || nKey == CK_EAST) {
        // Build the commandline.
        Log(7, "SDeskItemShowPlayList::OnKeyDown(%s)\n", buf);

        sprintf(buf, "\"%s/apps/showplaylist\" \"%s\"",
                SDesk::getInstance().getRootDir(),
                SDesk::getInstance().getMusicPlayer());
        system(buf);
        Log(7, "SDeskItemShowPlayList::OnKeyDown() done true\n");
        return true;
    }

    Log(7, "SDeskItemShowPlayList::OnKeyDown() done\n");
    return false;
}

