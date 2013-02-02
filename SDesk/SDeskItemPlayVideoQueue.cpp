#include "SDeskItemPlayVideoQueue.h"
#include "SDesk.h"

SDeskItemPlayVideoQueue::SDeskItemPlayVideoQueue(
    const char* name)
    : SDeskItem(name, 0, false)
{
    Log(7, "SDeskItemPlayVideoQueue(%s)\n", name);

    // Track this items type.
    mType = I_PLAY_VIDEO_QUEUE;

    Log(7, "SDeskItemPlayVideoQueue() done\n");
}

SDeskItemPlayVideoQueue::~SDeskItemPlayVideoQueue()
{
    Log(7, "~SDeskItemPlayVideoQueue()\n");

    Log(7, "~SDeskItemPlayVideoQueue() done\n");
}

bool SDeskItemPlayVideoQueue::OnKeyDown(u32 nKey)
{
    char buf[1024];

    Log(7, "SDeskItemPlayVideoQueue::OnKeyDown(%d)\n", nKey);

    if (nKey == CK_PLAY || nKey == CK_PAUSE || nKey == CK_PLAYPAUSE ||
        nKey == CK_SELECT || nKey == CK_EAST) {
        // Build the commandline.
        Log(7, "SDeskItemPlayVideoQueue::OnKeyDown(%s)\n", buf);

        sprintf(buf, "\"%s/apps/playvideoqueue\" \"%s\"",
                SDesk::getInstance().getRootDir(),
                SDesk::getInstance().getMPlay());
        system(buf);
        Log(7, "SDeskItemPlayVideoQueue::OnKeyDown() done true\n");
        return true;
    }

    Log(7, "SDeskItemPlayVideoQueue::OnKeyDown() done\n");
    return false;
}

