#include <string.h>

#include "SDeskScan.h"
#include "SDeskScanGenre.h"

SDeskScanGenre::SDeskScanGenre(
    const char* name,
    int genre)
{
    Log(7, "SDeskScanGenre(%s,%d)\n", name, genre);

    mName = CascadeString(name);
    mGenre = genre;

    Log(7, "SDeskScanGenre() done\n");
}

SDeskScanGenre::~SDeskScanGenre()
{
    Log(7, "~SDeskScanGenre()\n");

    Log(7, "~SDeskScanGenre() done\n");
}

bool SDeskScanGenre::isGenre(
    const char* genre)
{
    char buffer[1024];

    strcpy(buffer, genre);
    for (unsigned int counter = 0; counter < strlen(buffer); counter++) {
        buffer[counter] = tolower(buffer[counter]);
    }
    string * tmpGenre = new string(buffer);

    vector<string *>::const_iterator it, end = mPatterns.end();
    for(it = mPatterns.begin(); it != end; it++) {
        if (tmpGenre->find((*it)->c_str()) != string::npos) {
            Log(7, "SDeskScanGenre::isGenre(%s, %s)\n", tmpGenre->c_str(),
                (*it)->c_str());
            return true;
        }
    }
    return false;
}

void SDeskScanGenre::DumpGenre()
{
    char fileName[1024];
    char saveFileName[1024];
    const char* name = mName;
    FILE *fd;

    Log(7, "SDeskScanGenre::DumpGenre(%s)\n", name);

    sprintf(fileName, "%s/menus/Radio/%s.mnu",
            SDeskScan::getInstance().getRootDir(), name);
    if (SDeskScan::getInstance().DoBackup()) {
        sprintf(saveFileName, "%s.save", fileName);
        rename(fileName, saveFileName);
    }
    
    Log(7, "SDeskScan::WriteFiles(%s)\n", fileName);
    if ((fd = fopen(fileName, "w+")) == NULL) {
        Log(8, "fopen failed (%s)...\n", fileName);
    } else {
        vector<SDeskScanRadio *>::const_iterator it, 
            end = mRadioList.end();
        for(it = mRadioList.begin(); it != end; it++) {
            const char* name = (*it)->getName();
            const char* url = (*it)->getURL();
            fwrite("%radio\n", sizeof(char), 7, fd);
            fwrite(name, sizeof(char), strlen(name), fd);
            fwrite("\n", sizeof(char), 1, fd);
            fwrite(url, sizeof(char), strlen(url), fd);
            fwrite("\n0\n", sizeof(char), 3, fd);
        }
        fclose(fd);
    }

    Log(7, "SDeskScanGenre::DumpGenre() done\n");
}
