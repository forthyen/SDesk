#ifndef _sdesk_item_remove_file_h
#define _sdesk_item_remove_file_h

#include <cascade/Cascade.h>

#include "SDeskItem.h"

class SDeskItemRemoveFile : public SDeskItem
{
public:
    SDeskItemRemoveFile(const char* name, const char* filename);
    virtual ~SDeskItemRemoveFile();

    virtual bool OnKeyDown(u32 nKey);
};

#endif

