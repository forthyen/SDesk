#include <string.h>

#include "SDeskScan.h"
#include "SDeskScanSongByTag.h"

SDeskScanSongByTag::SDeskScanSongByTag(
    const char* name)
{
    Log(7, "SDeskScanSongByTag(%s)\n", name);

    mName = CascadeString(name);

    Log(7, "SDeskScanSongByTag() done\n");
}

SDeskScanSongByTag::~SDeskScanSongByTag()
{
    Log(7, "~SDeskScanSongByTag()\n");

    Log(7, "~SDeskScanSongByTag() done\n");
}

void SDeskScanSongByTag::addSong (
    SDeskScanSong *song)
{
    mSongList.push_back(song);
}

void SDeskScanSongByTag::DumpTagList(
    const char *tagName)
{
    char buffer[1024];
    char fileName[1024];
    char saveFileName[1024];
    const char* tag = mName;
    FILE *fd;

    Log(7, "SDeskScanSongByTag(%s, %s)::DumpTagList\n", tagName, tag);

    strcpy(buffer, tag);
    SDeskScan::getInstance().FixFileName(buffer);
    sprintf(fileName, "%s/menus/Music/By%s_%s.mnu",
            SDeskScan::getInstance().getRootDir(), tagName, buffer);
    if (SDeskScan::getInstance().DoBackup()) {
        sprintf(saveFileName, "%s/menus/Music/By%s_%s.mnu.save",
                SDeskScan::getInstance().getRootDir(), tagName, buffer);
        rename(fileName, saveFileName);
    }
    
    Log(7, "SDeskScan::WriteFiles(%s)\n", fileName);
    if ((fd = fopen(fileName, "w+")) == NULL) {
        Log(8, "fopen failed (%s)...\n", fileName);
    } else {
        vector<SDeskScanSong *>::const_iterator it, 
            end = mSongList.end();
        for(it = mSongList.begin(); it != end; it++) {
            const char *name = (*it)->getName();
            const char* location = (*it)->getLocation();
            fwrite("%song\n", sizeof(char), 6, fd);
            fwrite(name, sizeof(char), strlen(name), fd);
            fwrite("\n", sizeof(char), 1, fd);
            fwrite(location, sizeof(char), strlen(location), fd);
            if ((*it)->getProtected()) {
                fwrite("\n1\n", sizeof(char), 3, fd);
            } else {
                fwrite("\n0\n", sizeof(char), 3, fd);
            }
        }
        fclose(fd);
    }


    Log(7, "SDeskScanSongByTag()::DumpTagList done\n");
}



