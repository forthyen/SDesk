#ifndef _sdesk_item_play_video_queue_h
#define _sdesk_item_play_video_queue_h

#include <cascade/Cascade.h>

#include "SDeskItem.h"

class SDeskItemPlayVideoQueue : public SDeskItem
{
public:
    SDeskItemPlayVideoQueue(const char* name);
    virtual ~SDeskItemPlayVideoQueue();

    virtual bool OnKeyDown(u32 nKey);
};

#endif

