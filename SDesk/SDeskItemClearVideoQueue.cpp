#include "SDeskItemClearVideoQueue.h"
#include "SDesk.h"

SDeskItemClearVideoQueue::SDeskItemClearVideoQueue(
    const char* name)
    : SDeskItem(name, 0, false)
{
    Log(7, "SDeskItemClearVideoQueue(%s)\n", name);

    // Track this items type.
    mType = I_CLEAR_VIDEO_QUEUE;

    Log(7, "SDeskItemClearVideoQueue() done\n");
}

SDeskItemClearVideoQueue::~SDeskItemClearVideoQueue()
{
    Log(7, "~SDeskItemClearVideoQueue()\n");

    Log(7, "~SDeskItemClearVideoQueue() done\n");
}

bool SDeskItemClearVideoQueue::OnKeyDown(u32 nKey)
{
    char buf[1024];

    Log(7, "SDeskItemClearVideoQueue::OnKeyDown(%d)\n", nKey);

    if (nKey == CK_PLAY || nKey == CK_PAUSE || nKey == CK_PLAYPAUSE ||
        nKey == CK_SELECT || nKey == CK_EAST) {
        // Build the commandline.
        Log(7, "SDeskItemClearVideoQueue::OnKeyDown(%s)\n", buf);

        sprintf(buf, "\"%s/apps/clearvideoqueue\"",
                SDesk::getInstance().getRootDir());
        system(buf);
        SDesk::getInstance().getCurrentTheme().SetStatusMessage(
            "Cleared movie queue...");
        Log(7, "SDeskItemClearVideoQueue::OnKeyDown() done true\n");
        return true;
    }

    Log(7, "SDeskItemClearVideoQueue::OnKeyDown() done\n");
    return false;
}

