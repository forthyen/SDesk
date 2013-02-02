#ifndef _sdesk_item_movie_h
#define _sdesk_item_movie_h

#include <cascade/Cascade.h>

#include "SDeskItem.h"

class SDeskItemMovie : public SDeskItem
{
public:
    SDeskItemMovie(const char* name, const char* filename, bool prot);
    virtual ~SDeskItemMovie();

    virtual bool OnKeyDown(u32 nKey);

    virtual CascadeString& getItemDefinition();
};

#endif

