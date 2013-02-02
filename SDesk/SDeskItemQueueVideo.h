#ifndef _sdesk_item_queue_video_h
#define _sdesk_item_queue_video_h

#include <cascade/Cascade.h>

#include "SDeskItem.h"

class SDeskItemQueueVideo : public SDeskItem
{
public:
    SDeskItemQueueVideo(const char* name, const char* filename);
    virtual ~SDeskItemQueueVideo();

    virtual bool OnKeyDown(u32 nKey);
};

#endif

