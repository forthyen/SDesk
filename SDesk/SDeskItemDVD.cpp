#include "SDesk.h"
#include "SDeskItemDVD.h"
#include "SDeskDirScanner.h"

SDeskItemDVD::SDeskItemDVD(
    const char* name,
    const char* filename,
    bool prot)
    : SDeskItem(name, filename, prot)
{
    Log(7, "SDeskItemDVD(%s,%s)\n", name, filename);

    // Track this items type.
    mType = I_DVD;

    Log(7, "SDeskItemDVD() done\n");
}

SDeskItemDVD::~SDeskItemDVD()
{
    Log(7, "~SDeskItemDVD()\n");

    Log(7, "~SDeskItemDVD() done\n");
}

bool SDeskItemDVD::OnKeyDown(u32 nKey)
{
    const char *str = mFilename;
    char buf[1024];
    char buf2[1024];
    char buf3[1024];
    char buf4[1024];
    u64 largestSize = 0;
    u64 largestFileSize = 0;
    int largestSequence = -1;
    bool val;
    struct stat filestat;

    Log(1, "SDeskItemDVD::OnKeyDown(%d)\n", nKey);

    if (nKey == CK_PLAY || nKey == CK_PAUSE || nKey == CK_PLAYPAUSE ||
        nKey == CK_SELECT || nKey == CK_EAST) {

        // Build the commandline.
        if (SDesk::getInstance().getMPlay() == 0) {
            Log(1, "SDeskItemDVD::OnKeyDown() no mplay configured!\n");
        }

        sprintf(buf2, "%s/", str);
        SDeskDirScanner *dir = new SDeskDirScanner(buf2, "VTS");
        if (dir->GetNumEntries() == 0) {
            sprintf(buf2, "%s/VIDEO_TS/", str);
        }

        for (int sequenceNum = 0; ; ++sequenceNum) {
            // Go through the sequences...
            u64 sequenceSize = 0;

            delete dir;
            sprintf(buf3, "VTS_%02d", sequenceNum);
            dir = new SDeskDirScanner(buf2, buf3);

            if (dir->GetNumEntries() == 0 && sequenceNum > 10) {
                // No more entries that match
                delete dir;
                break;
            }

            for (u32 pos2 = 0; pos2 < dir->GetNumEntries(); ++pos2) {
                // Go through all items in the directory.
                const char *entry2 = dir->GetEntry(pos2);
            
                if (!entry2) {
                    // Not a valid entry.
                    continue;
                }
                if (strncmp(entry2 + strlen(entry2) - 4, ".vob", 4) == 0 ||
                    strncmp(entry2 + strlen(entry2) - 4, ".VOB", 4) == 0) {
                    sequenceSize += dir->GetSize();
                }
                if (largestFileSize < dir->GetSize()) {
                    strcpy(buf4, entry2);
                    largestFileSize = dir->GetSize();
                }
            }
            if (largestSize < sequenceSize) {
                // This sequence is larger than the previous one.
                largestSize = sequenceSize;
                largestSequence = sequenceNum;
            }
        }
        if (largestSequence != -1) {
            sprintf(buf3, "%s/VTS_%02d_1.VOB", buf2, largestSequence);
            if (stat(buf3, &filestat) != 0) {
                sprintf(buf3, "%s/VTS_%02d_2.VOB",
                        buf2, largestSequence);
                if (stat(buf3, &filestat) != 0) {
                    sprintf(buf3, "%s/%s", buf2, buf4);
                    if (stat(buf3, &filestat) != 0) {
                        Log(1, "SDeskItemDVD::OnKeyDown() no vob %s\n",
                            buf3);
                    }
                }
            }
            sprintf(buf,        
                    "/bin/sh \"%s\" \"%s\" \"%s\"",
                    SDesk::getInstance().getPlaymovie(),
                    SDesk::getInstance().getMPlay(), buf3);
            Log(1, "SDeskItemDVD::OnKeyDown(%s)\n", buf);
            // Run the commandline
            val = CascadeSpawn::Spawn(buf);
            if (!val) {
                string* message = new string(
                    "Unable to run playmovie script.");
                SDesk::getInstance().AddMessage(message);
                Log(1, "Could not spawn!\n");
                SDesk::getInstance().getCurrentTheme().SetStatusMessage(
                    "Unable to play movie...");
            } else {
                SDesk::getInstance().getCurrentTheme().SetStatusMessage(
                    "Started to play movie...");
            }
        } else {
            Log(1, "SDeskItemDVD::OnKeyDown() no largets sequence\n");
        }

        Log(7, "SDeskItemDVD::OnKeyDown() done true\n");
        return true;
    }

    Log(7, "SDeskItemDVD::OnKeyDown() done\n");
    return false;
}

CascadeString& SDeskItemDVD::getItemDefinition()
{
    char buffer[1024];

    Log(7, "SDeskItemDVD::getItemDefinition\n");

    strcpy(buffer, "%dvd\n");
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

    Log(7, "SDeskItemDVD::getItemDefinition done\n");
    return mItemDefinition;
}
