#ifndef _sdesk_item_play_h
#define _sdesk_item_play_h

#include <cascade/Cascade.h>

#include "SDeskItem.h"

class SDeskItemPlay : public SDeskItem
{
public:
    SDeskItemPlay(const char* name, const char* filename, bool prot);
    virtual ~SDeskItemPlay();

    virtual bool OnKeyDown(u32 nKey);
};

#endif

