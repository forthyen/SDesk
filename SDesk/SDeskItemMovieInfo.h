#ifndef _sdesk_item_movie_info_h
#define _sdesk_item_movie_info_h

#include <cascade/Cascade.h>

#include "SDeskItem.h"

class SDeskItemMovieInfo : public SDeskItem
{
public:
    SDeskItemMovieInfo(const char* name, const char* filename);
    virtual ~SDeskItemMovieInfo();

    void DrawMessage(CascadeBitmap& bitmap);
    virtual bool OnKeyDown(u32 nKey);
};

#endif

