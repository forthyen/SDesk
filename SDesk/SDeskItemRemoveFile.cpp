#include <unistd.h>

#include "SDesk.h"
#include "SDeskItemRemoveFile.h"

SDeskItemRemoveFile::SDeskItemRemoveFile(
    const char* name,
    const char* filename)
    : SDeskItem("", filename, false)
{
    Log(7, "SDeskItemRemoveFile(%s,%s)\n", name, filename);

    // Track this items type.
    mType = I_REMOVE_FILE;

    mName = "Remove File!";

    Log(7, "SDeskItemRemoveFile() done\n");
}

SDeskItemRemoveFile::~SDeskItemRemoveFile()
{
    Log(7, "~SDeskItemRemoveFile()\n");

    Log(7, "~SDeskItemRemoveFile() done\n");
}

bool SDeskItemRemoveFile::OnKeyDown(u32 nKey)
{
    Log(7, "SDeskItemRemoveFile::OnKeyDown(%d)\n", nKey);

    if (nKey == CK_PLAY || nKey == CK_PAUSE || nKey == CK_PLAYPAUSE ||
        nKey == CK_SELECT || nKey == CK_EAST) {
        char buf[3024];
        char basename[1024];
        const char *str = mFilename;
        char *pos, *slashPos;

        strcpy(basename, str);
        slashPos = strrchr(basename, '/');
        pos = strrchr(basename, '.');
        if (pos && slashPos < pos) {
            *pos = 0;
        }
        sprintf(buf, "\"%s/apps/removefile\" \"%s\" \"%s\"",
                SDesk::getInstance().getRootDir(), str, basename);

        // We do the unlikn because the roku rm has trouble with large
        // files.
        unlink(str);
        system(buf);

        SDesk::getInstance().getCurrentTheme().SetStatusMessage(
            "Removed movie...");
        return true;
    }

    Log(7, "SDeskItemRemoveFile::OnKeyDown() done\n");
    return false;
}
