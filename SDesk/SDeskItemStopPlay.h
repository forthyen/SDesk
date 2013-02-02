#ifndef _sdesk_item_stop_play_h
#define _sdesk_item_stop_play_h

#include <cascade/Cascade.h>

#include "SDeskItem.h"

class SDeskItemStopPlay : public SDeskItem
{
public:
    SDeskItemStopPlay(const char* name);
    virtual ~SDeskItemStopPlay();

    virtual bool OnKeyDown(u32 nKey);
};

#endif

