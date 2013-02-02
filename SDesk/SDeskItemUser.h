#ifndef _sdesk_item_user_h
#define _sdesk_item_user_h

#include <cascade/Cascade.h>

#include "SDeskItem.h"

class SDeskItemUser : public SDeskItem
{
public:
    SDeskItemUser(const char* name, const char* filename,
                  const char* user, const char* password);
    virtual ~SDeskItemUser();

    virtual bool OnKeyDown(u32 nKey);

    // The user name
    char mUserName[100];

    // The user specific password
    char mPassword[100];
};

#endif

