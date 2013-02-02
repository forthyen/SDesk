#include <unistd.h>

#include "SDesk.h"
#include "SDeskItemQueueVideo.h"

SDeskItemQueueVideo::SDeskItemQueueVideo(
    const char* name,
    const char* filename)
    : SDeskItem("", filename, false)
{
    Log(7, "SDeskItemQueueVideo(%s,%s)\n", name, filename);

    // Track this items type.
    mType = I_QUEUE_VIDEO;

    mName = "Queue Movie";

    Log(7, "SDeskItemQueueVideo() done\n");
}

SDeskItemQueueVideo::~SDeskItemQueueVideo()
{
    Log(7, "~SDeskItemQueueVideo()\n");

    Log(7, "~SDeskItemQueueVideo() done\n");
}

bool SDeskItemQueueVideo::OnKeyDown(u32 nKey)
{
    Log(7, "SDeskItemQueueVideo::OnKeyDown(%d)\n", nKey);

    if (nKey == CK_PLAY || nKey == CK_PAUSE || nKey == CK_PLAYPAUSE ||
        nKey == CK_SELECT || nKey == CK_EAST) {
        char buf[3024];
        const char *str = mFilename;

        sprintf(buf, "\"%s/apps/queuevideo\" \"%s\"",
                SDesk::getInstance().getRootDir(), str);
        system(buf);
        SDesk::getInstance().getCurrentTheme().SetStatusMessage(
            "Queued movie...");
        return true;
    }

    Log(7, "SDeskItemQueueVideo::OnKeyDown() done\n");
    return false;
}
