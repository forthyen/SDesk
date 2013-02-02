#ifndef _sdesk_item_message_h
#define _sdesk_item_message_h

#include <cascade/Cascade.h>

#include "SDeskItem.h"

using namespace std;

class SDeskItemMessage : public SDeskItem
{
public:
    SDeskItemMessage(const char* name, const char* filename, bool prot);
    virtual ~SDeskItemMessage();

    virtual bool OnKeyDown(u32 nKey);

    bool HaveMessage();
    void CleanMessages();
    void AddMessage(string* value);
    void DrawMessage(CascadeBitmap& bitmap);

    // Our message list
    vector<string *> mMessages;
};

#endif

