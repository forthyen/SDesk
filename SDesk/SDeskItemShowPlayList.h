#ifndef _sdesk_item_show_play_list_h
#define _sdesk_item_show_play_list_h

#include <cascade/Cascade.h>

#include "SDeskItem.h"

class SDeskItemShowPlayList : public SDeskItem
{
public:
    SDeskItemShowPlayList(const char* name);
    virtual ~SDeskItemShowPlayList();

    virtual bool OnKeyDown(u32 nKey);
};

#endif

