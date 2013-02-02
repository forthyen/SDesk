#ifndef _sdesk_item_h
#define _sdesk_item_h

#include <cascade/Cascade.h>

#include "SDeskBitmap.h"
#include "SDeskLog.h"

enum tmpITEMTYPES { I_APP,               // 0
                    I_MENU,              // 1
                    I_SONG,              // 2
                    I_MOVIE,             // 3
                    I_PICTURE,           // 4
                    I_RADIO,             // 5
                    I_FAVORITE_LIST,     // 6
                    I_FAVORITE_REMOVE,   // 7
                    I_USER,              // 8
                    I_MESSAGE,           // 9
                    I_PROTECT,           // 10
                    I_RESCAN,            // 11
                    I_PLAY,              // 12
                    I_DVD,               // 13
                    I_RSS_FEED,          // 14
                    I_RSS,               // 15
                    I_STOP_PLAY,         // 16
                    I_SHOW_PLAY_LIST,    // 17
                    I_REMOVE_FILE,       // 18
                    I_MOVIE_INFO,        // 19
                    I_PLAY_VIDEO_QUEUE,  // 20
                    I_CLEAR_VIDEO_QUEUE, // 21
                    I_QUEUE_VIDEO,       // 22
                    M_MISC,              // 23 
                    M_SONGS,             // 24
                    M_MOVIES,            // 25
                    M_PICTURES,          // 26
                    M_RADIOS,            // 27
                    M_FAVORITES,         // 28
                    M_SUBMENU,           // 29
                    M_APPS,              // 30
                    M_RSS_FEED,          // 31
                    M_MESSAGE,           // 32
                    M_RSS,               // 33
                    M_MOVIE_INFO,        // 34
                    M_SLIDESHOW };       // 35

class SDeskItem : public SDeskLog
{
public:
    SDeskItem(const char* name, const char* filename, bool prot);
    virtual ~SDeskItem();

    virtual bool OnKeyDown(u32 nKey) = 0;

    void DrawText(CascadeBitmap& bitmap, int x, int y, int menuType);
    void DrawDetailText(CascadeBitmap& bitmap);

    CascadeString& getName() {
        return mName;
    }
    CascadeString& getFileName() {
        return mFilename;
    }
    virtual CascadeString& getItemDefinition() {
        return mItemDefinition;
    }
    void setSelected(bool value) {
        mSelected = value;
    }
    bool getSelected() {
        return mSelected;
    }
    int getType() {
        return mType;
    }
    u32 getTopOffset() {
        return mTopOffset;
    }
    void setTopOffset(u32 offset) {
        mTopOffset = offset;
    }

    // Internal item properties.
    CascadeString mName;
    CascadeString mFilename;

    // The favorite item definition
    CascadeString mItemDefinition;

    // The detail text retrieved for the item.
    CascadeString mDetailText;

    // Is this item currently selected.
    bool mSelected;

    // The items type.
    int mType;

    // Additional item offset
    u32 mTopOffset;

    // Is the item protected
    bool mProtected;
};

#endif

