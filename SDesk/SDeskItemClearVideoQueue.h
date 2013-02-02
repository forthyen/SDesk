#ifndef _sdesk_item_clear_video_queue_h
#define _sdesk_item_clear_video_queue_h

#include <cascade/Cascade.h>

#include "SDeskItem.h"

class SDeskItemClearVideoQueue : public SDeskItem
{
public:
    SDeskItemClearVideoQueue(const char* name);
    virtual ~SDeskItemClearVideoQueue();

    virtual bool OnKeyDown(u32 nKey);
};

#endif

