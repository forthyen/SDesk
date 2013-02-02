#ifndef _sdesk_item_menu_h
#define _sdesk_item_menu_h

#include <cascade/Cascade.h>

#include <vector>

using namespace std;

#include "SDeskItem.h"
#include "SDeskItemApp.h"

bool CompareItem(const SDeskItem* a, const SDeskItem* b);

class SDeskItemMenu : public SDeskItem
{
public:
    SDeskItemMenu(const char* name, const char* type,
                  const char* filename, bool prot, SDeskItemMenu *parentMenu);
    virtual ~SDeskItemMenu();

    virtual bool OnKeyDown(u32 nKey);

    int Play();
    void Select();
    void Rebuild();
    void Draw(CascadeBitmap& bitmap);

    bool ScanDirectory();
    bool ReadMenuFile();
    void ShowSubMenu();
    void ReadNewsFeed(const char *url);
    void ReadPlayList();
    void AppendFavorites(int parentMenuType, CascadeString& itemDefinition);

    int getMenuType() {
        return mMenuType;
    }

    virtual CascadeString& getItemDefinition();

private:
    // Menu items...
    vector<SDeskItem *> mItems;
    vector<SDeskItem *>::const_iterator mPos;
    vector<SDeskItem *>::const_iterator mFirstDisplay;
    vector<SDeskItem *>::const_iterator mLastDisplay;

    // Our own parent menu
    SDeskItemMenu *mParentMenu;

    // The temporary submenu (actions on current item).
    SDeskItemMenu *mSubMenu;

    // The menu type (what content do we have here).
    int mMenuType;
    char mMenuTypeName[20];

    // Indicates that we already have a warning message menu item
    bool mHaveWarning;

    // Can remove filers
    bool mCanRemoveFiles;
};

#endif

