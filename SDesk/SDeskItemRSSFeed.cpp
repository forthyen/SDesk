#include "SDesk.h"
#include "SDeskItemRSSFeed.h"

SDeskItemRSSFeed::SDeskItemRSSFeed(
    const char* name,
    const char* filename,
    bool prot)
    : SDeskItem(name, filename, prot)
{
    Log(7, "SDeskItemRSSFeed(%s,%s)\n", name, filename);

    // Track this items type.
    mType = I_RSS_FEED;

    Log(7, "SDeskItemRSSFeed() done\n");
}

SDeskItemRSSFeed::~SDeskItemRSSFeed()
{
    Log(7, "~SDeskItemRSSFeed()\n");

    Log(7, "~SDeskItemRSSFeed() done\n");
}

bool SDeskItemRSSFeed::OnKeyDown(u32 nKey)
{
    Log(7, "SDeskItemRSSFeed::OnKeyDown(%d)\n", nKey);

    Log(7, "SDeskItemRSSFeed::OnKeyDown() done\n");
    return false;
}

