#include "SDesk.h"
#include "SDeskItemPlay.h"

SDeskItemPlay::SDeskItemPlay(
    const char* name,
    const char* filename,
    bool prot)
    : SDeskItem(name, filename, prot)
{
    Log(7, "SDeskItemPlay(%s,%s)\n", name, filename);

    mType = I_PLAY;

    Log(7, "SDeskItemPlay() done\n");
}

SDeskItemPlay::~SDeskItemPlay()
{
    Log(7, "~SDeskItemPlay()\n");

    Log(7, "~SDeskItemPlay() done\n");
}

bool SDeskItemPlay::OnKeyDown(u32 nKey)
{
    return false;
}
