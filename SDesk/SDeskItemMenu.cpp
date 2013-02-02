#include <algorithm>

#include <cascade/net/CascadeHTTPClient.h> 

#include <mxml.h>

#include "SDesk.h"
#include "SDeskFont.h"
#include "SDeskItemMenu.h"
#include "SDeskItemMovie.h"
#include "SDeskItemDVD.h"
#include "SDeskItemApp.h"
#include "SDeskItemSong.h"
#include "SDeskItemPicture.h"
#include "SDeskItemRadio.h"
#include "SDeskItemUser.h"
#include "SDeskItemRSS.h"
#include "SDeskItemURL.h"
#include "SDeskItemRSSFeed.h"
#include "SDeskItemMessage.h"
#include "SDeskItemStopPlay.h"
#include "SDeskItemShowPlayList.h"
#include "SDeskItemQueueVideo.h"
#include "SDeskItemPlayVideoQueue.h"
#include "SDeskItemClearVideoQueue.h"
#include "SDeskItemProtect.h"
#include "SDeskItemRemoveFile.h"
#include "SDeskItemMovieInfo.h"
#include "SDeskItemRescan.h"
#include "SDeskItemFavoriteRemove.h"
#include "SDeskItemFavoriteList.h"
#include "SDeskItemPlay.h"
#include "SDeskFavorite.h"
#include "SDeskDirScanner.h"

#define PHOTO_DELAY "/bin/sleep 5
"

SDeskItemMenu::SDeskItemMenu(
    const char* name,
    const char* type,
    const char* filename,
    bool prot,
    SDeskItemMenu *parentMenu)
    : SDeskItem(name, filename, prot),
      mPos(0),
      mFirstDisplay(0),
      mLastDisplay(0),
      mParentMenu(parentMenu),
      mSubMenu(0),
      mHaveWarning(false),
      mCanRemoveFiles(false)
{
    Log(7, "SDeskItemMenu(%s,%s,%s,%ld)\n", name, type, filename, this);

    // Track this items type.
    mType = I_MENU;

    // Track the default content for this menu.
    strcpy(mMenuTypeName, type);
    if (strcmp(type, "songs") == 0) {
        mMenuType = M_SONGS;
    } else if (strcmp(type, "movies") == 0) {
        mMenuType = M_MOVIES;
    } else if (strcmp(type, "pictures") == 0) {
        mMenuType = M_PICTURES;
    } else if (strcmp(type, "radios") == 0) {
        mMenuType = M_RADIOS;
    } else if (strcmp(type, "favorites") == 0) {
        mMenuType = M_FAVORITES;
    } else if (strcmp(type, "submenu") == 0) {
        mMenuType = M_SUBMENU;
    } else if (strcmp(type, "apps") == 0) {
        mMenuType = M_APPS;
    } else if (strcmp(type, "rssfeed") == 0) {
        mMenuType = M_RSS_FEED;
    } else if (strcmp(type, "rss") == 0) {
        mMenuType = M_RSS;
    } else if (strcmp(type, "movieinfo") == 0) {
        mMenuType = M_MOVIE_INFO;
    } else if (strcmp(type, "message") == 0) {
        mMenuType = M_MESSAGE;
    } else if (strcmp(type, "slideshow") == 0) {
        mMenuType = M_SLIDESHOW;
    } else if (strcmp(type, "playlist") == 0) {
        mMenuType = M_MISC;
    } else {
        mMenuType = M_MISC;
    }

    Log(7, "SDeskItemMenu() done\n");
}

SDeskItemMenu::~SDeskItemMenu()
{
    Log(7, "~SDeskItemMenu()\n");

    // Delete all our sub items.
    for (unsigned int i = 0; i < mItems.size(); i++) {
        delete mItems[i];
    }
    mItems.clear();

    Log(7, "~SDeskItemMenu() done\n");
}

bool SDeskItemMenu::OnKeyDown(u32 nKey)
{
    Log(7, "SDeskItemMenu::OnKeyDown(%d)\n", nKey);

    if (nKey == CK_SOUTH) {
        // Go down one item.
        if (mItems.size() == 0) {
            Log(7, "SDeskItemMenu::OnKeyDown(north empty)\n");
            return true;
        } else if (mPos + 1 != mItems.end()) {
            Log(7, "SDeskItemMenu::OnKeyDown(south)\n");

            // We still have more items after the current one.
            if (mPos >= mLastDisplay) {
                // We are the end of our displayed item list. We will
                // scroll down one item.
            Log(7, "SDeskItemMenu::OnKeyDown(south) done 1\n");
                mFirstDisplay++;
            }
            Log(7, "SDeskItemMenu::OnKeyDown(south) done 2\n");
            // Deselect the current item.
            (*mPos)->setSelected(false);
            // Go to the next item.
            Log(7, "SDeskItemMenu::OnKeyDown(south) done 3\n");
            mPos++;
            // Select that item.
            Log(7, "SDeskItemMenu::OnKeyDown(south) done 4\n");
            (*mPos)->setSelected(true);
            Log(7, "SDeskItemMenu::OnKeyDown(south) done true\n");
            return true;
        } else {
            Log(7, "SDeskItemMenu::OnKeyDown(south wrap)\n");

            // Deselect the current item.
            (*mPos)->setSelected(false);

            // We need to wrap around.
            mPos = mItems.begin();
            mFirstDisplay = mItems.begin();
            (*mPos)->setSelected(true);
            Log(7, "SDeskItemMenu::OnKeyDown(south wrap) done true\n");
            return true;
        }
    } else if (nKey == CK_NORTH) {
        // Go up one item.
        if (mItems.size() == 0) {
            Log(7, "SDeskItemMenu::OnKeyDown(north empty)\n");
            return true;
        } else if (mPos != mItems.begin()) {
            Log(7, "SDeskItemMenu::OnKeyDown(north)\n");

            // Still not the first item.
            if (mPos <= mFirstDisplay) {
                // We are at the first displayed item. We will scroll
                // up one item.
                mFirstDisplay--;
            }
            Log(7, "SDeskItemMenu::OnKeyDown(north) 2\n");
            // Deselect the current item.
            (*mPos)->setSelected(false);
            // Go up to the next item.
            mPos--;
            // Select that item.
            (*mPos)->setSelected(true);
            Log(7, "SDeskItemMenu::OnKeyDown(north) done true\n");
            return true;
        } else {
            // We need to wrap
            Log(7, "SDeskItemMenu::OnKeyDown(north wrap)\n");

            // Deselect the current item.
            (*mPos)->setSelected(false);

            // Go to the end
            mPos = mItems.end() - 1;
            mFirstDisplay = mPos;

            int counter =
                SDesk::getInstance().getCurrentTheme().getTextRows() - 1;
            for (; counter > 0; counter--) {
                if (mFirstDisplay != mItems.begin()) {
                    mFirstDisplay--;
                }
            }
            // Select that item.
            (*mPos)->setSelected(true);

            Log(7, "SDeskItemMenu::OnKeyDown(north wrap) done\n");
            return true;
        }
    } else if (nKey == CK_ZOOM_IN) {
        // Go up multiple items.
        Log(7, "SDeskItemMenu::OnKeyDown(zoom in)\n");

        // Deselect the current item.
        (*mPos)->setSelected(false);

        int counter = SDesk::getInstance().getCurrentTheme().getTextRows() - 1;
        for (; counter > 0; counter--) {
            if (mPos != mItems.begin()) {
                mPos--;
            }
            if (mFirstDisplay != mItems.begin()) {
                mFirstDisplay--;
            }
        }

        // Select that item.
        (*mPos)->setSelected(true);
        Log(7, "SDeskItemMenu::OnKeyDown(zoom in) done true\n");
        return true;
    } else if (nKey == CK_ZOOM_OUT) {
        // Go up multiple items.
        Log(7, "SDeskItemMenu::OnKeyDown(zoom out)\n");

        // Deselect the current item.
        (*mPos)->setSelected(false);
        Log(7, "SDeskItemMenu::OnKeyDown(zoom out) done true1\n");

        int lines = SDesk::getInstance().getCurrentTheme().getTextRows() - 1;
        int counter = lines;
        Log(7, "SDeskItemMenu::OnKeyDown(zoom out) done true2\n");
        for (; counter > 0; counter--) {
            if (mPos + 1 != mItems.end()) {
                mPos++;
            }
            if (mFirstDisplay + lines != mItems.end()) {
                mFirstDisplay++;
            }
        }

        // Select that item.
        Log(7, "SDeskItemMenu::OnKeyDown(zoom out) done true3\n");
        (*mPos)->setSelected(true);
        Log(7, "SDeskItemMenu::OnKeyDown(zoom out) done true4\n");
        return true;
    } else if (nKey == CK_EXIT && mParentMenu == 0) {
        // Exit in the root menu... pass it back to the main loop.
        Log(7, "SDeskItemMenu::OnKeyDown(EXIT)\n");

        return false;
    } else if ((nKey == CK_WEST ||
                (nKey == CK_EXIT && SDesk::getInstance().getExitIsUp())) &&
                mParentMenu != 0) {
        Log(7, "SDeskItemMenu::OnKeyDown(west)\n");

        // Set our global current menu reference to the
        // parent menu of the current menu.
        SDesk::getInstance().setCurrentMenu(mParentMenu);

        // Reset the current item pos.
        mPos = 0;
        mFirstDisplay = 0;
        mLastDisplay = 0;
        
        // Delete all children of the sub menu.
        for (unsigned int i = 0; i < mItems.size(); i++) {
            delete mItems[i];
        }
        mItems.clear();
        Log(7, "SDeskItemMenu::OnKeyDown(west) done true\n");
        return true;
    } else if (mPos != 0) {
        Log(7, "SDeskItemMenu::OnKeyDown(else %d %d)\n",
            (*mPos)->getType(), mMenuType);

        if ((nKey == CK_PLAY || nKey == CK_PAUSE || nKey == CK_PLAYPAUSE) &&
            ((*mPos)->getType() == I_MENU)) {
            Log(7, "SDeskItemMenu::OnKeyDown(play menu)\n");

            if (!((SDeskItemMenu *) *mPos)->Play()) {
                // Couldn't play menu.. try changing into it.
                ((SDeskItemMenu *) *mPos)->Select();
            }
            Log(7, "SDeskItemMenu::OnKeyDown(play menu) done true\n");
            return true;
        } else if ((nKey == CK_PLAY || nKey == CK_PAUSE ||
                    nKey == CK_PLAYPAUSE ||
                    nKey == CK_SELECT || nKey == CK_EAST) &&
                   (*mPos)->getType() == I_PLAY) {
            Log(7, "SDeskItemMenu::OnKeyDown(play favorites)\n");

            if (!Play()) {
                // Couldn't play menu.. try changing into it.
                Select();
            }
            Log(7, "SDeskItemMenu::OnKeyDown(play favorites) done true\n");
            return true;
        } else if ((nKey == CK_SELECT || nKey == CK_EAST) &&
            (*mPos)->getType() == I_FAVORITE_LIST) {
            Log(7, "SDeskItemMenu::OnKeyDown(add favorite)\n");

            ((SDeskItemFavoriteList *) *mPos)->OnKeyDown(nKey);

            // Set our global current menu reference to the
            // parent menu of the current menu.
            SDesk::getInstance().setCurrentMenu(mParentMenu);
            
            // Reset the current item pos.
            mPos = 0;
            mFirstDisplay = 0;
            mLastDisplay = 0;
            
            // Delete all children of the sub menu.
            for (unsigned int i = 0; i < mItems.size(); i++) {
                delete mItems[i];
            }
            mItems.clear();
            Log(7, "SDeskItemMenu::OnKeyDown(add favorite) done true\n");
            return true;
        } else if ((nKey == CK_SELECT || nKey == CK_EAST) &&
                   (*mPos)->getType() == I_MENU) {
            Log(7, "SDeskItemMenu::OnKeyDown(select menu)\n");

            ((SDeskItemMenu *) *mPos)->Select();
            Log(7, "SDeskItemMenu::OnKeyDown(select menu) done true\n");
            return true;
        } else if ((nKey == CK_PLAY || nKey == CK_PAUSE ||
                    nKey == CK_PLAYPAUSE || nKey == CK_SELECT ||
                    nKey == CK_EAST) &&
                   ((*mPos)->getType() == I_MESSAGE ||
                    (*mPos)->getType() == I_MOVIE_INFO ||
                    (*mPos)->getType() == I_RSS_FEED ||
                    (*mPos)->getType() == I_RSS)) {
            Log(7, "SDeskItemMenu::OnKeyDown(select message)\n");

            ShowSubMenu();
            Log(7, "SDeskItemMenu::OnKeyDown(select message) done true\n");
            return true;
        } else if (((nKey == CK_EAST && (*mPos)->getType() != I_APP &&
                     (*mPos)->getType() != I_STOP_PLAY &&
                     (*mPos)->getType() != I_SHOW_PLAY_LIST &&
                     (*mPos)->getType() != I_PLAY_VIDEO_QUEUE &&
                     (*mPos)->getType() != I_CLEAR_VIDEO_QUEUE &&
                     (*mPos)->getType() != I_USER &&
                     (*mPos)->getType() != I_RESCAN) ||
                    nKey == CK_MENU) && mMenuType != M_SUBMENU) {
            Log(7, "SDeskItemMenu::OnKeyDown(select sub menu)\n");

            ShowSubMenu();
            Log(7, "SDeskItemMenu::OnKeyDown(select sub menu) done true\n");
            return true;
        } else {
            // Have the item deal with the key press.
            bool ret;

            Log(7, "SDeskItemMenu::OnKeyDown(item)\n");
            ret = (*mPos)->OnKeyDown(nKey);
            
            if ((*mPos)->getType() == I_FAVORITE_REMOVE ||
                (*mPos)->getType() == I_PROTECT ||
                (*mPos)->getType() == I_REMOVE_FILE ||
                (*mPos)->getType() == I_QUEUE_VIDEO) {
                // Rebuild the parent menu
                mParentMenu->Rebuild();

                // Set our global current menu reference to the
                // parent menu of the current menu.
                SDesk::getInstance().setCurrentMenu(mParentMenu);
                
                // Reset the current item pos.
                mPos = 0;
                mFirstDisplay = 0;
                mLastDisplay = 0;
                
                // Delete all children of the sub menu.
                for (unsigned int i = 0; i < mItems.size(); i++) {
                    delete mItems[i];
                }
                mItems.clear();
            } else {
                if ((*mPos)->getType() == I_SHOW_PLAY_LIST) {
                    // Build the sub menu for the play list
                    ShowSubMenu();
                }
            }
            Log(7, "SDeskItemMenu::OnKeyDown() done ret\n");
            return ret;
        }
    }

    Log(7, "SDeskItemMenu::OnKeyDown() done false\n");
    return false;
}

int SDeskItemMenu::Play()
{
    const char *str = mFilename;
    char buf[1025];
    bool val;
    int retVal = 0;

    CascadeString command;
    CascadeString result;

    if (!str) {
        Log(7, "SDeskItemMenu::Play(NO str)\n");
        return 0;
    }

    char *ext = strrchr(str, '.');
    Log(7, "SDeskItemMenu::Play(%s)\n", str);
    if (ext && strcmp(ext, ".mnu") == 0) {
        char location[1025];
        bool prot;
        char *pos;
        FILE *fd;
        FILE *outFd;
        
        // We can not play menu files... change into the menu.
        Log(7, "SDeskItemMenu::Play() menu\n");
        if ((fd = fopen(str, "r")) == NULL) {
            Log(8, "fopen failed (%s)...\n", str);
            return 0;
        }
        if ((outFd = fopen("/tmp/slideshow", "w")) == NULL) {
            Log(8, "fopen failed (/tmp/slideshow)...\n");
            return 0;
        }
        
        while ((pos = fgets(buf, 1023, fd)) != 0) {
            Log(7, buf);
            if (strcmp(buf, "%song\n") == 0) {
                if ((pos = fgets(buf, 1023, fd)) != 0) {
                    // Skip the name
                }
                if ((pos = fgets(buf, 1023, fd)) != 0) {
                    int endPos = 0;
                    if (buf[0] != '/') {
                        endPos = strlen(buf) +
                            strlen(SDesk::getInstance().getRootDir());
                        strcpy(location, SDesk::getInstance().getRootDir());
                        strcat(location, "/");
                        strcat(location, buf);
                    } else {
                        endPos = strlen(buf) - 1;
                        strcpy(location, buf);
                    }
                    location[endPos] = 0;
                }
                prot = false;
                if ((pos = fgets(buf, 1023, fd)) != 0) {
                    if (strcmp(buf, "1\n") == 0) {
                        prot = true;
                    }
                }
                Log(7, "SDeskItemMenu::Play() song\n");
                if ((SDesk::getInstance().getProtected() && !prot) ||
                    !SDesk::getInstance().getProtected()) {
                    fprintf(outFd, "/bin/sh \"%s\" \"%s\" \"%s\"\n",
                            SDesk::getInstance().getPlaysong(),
                            SDesk::getInstance().getMusicPlayer(),
                            location);
                    retVal = 1;
                }
            }
        }
        fclose(fd);
        
        // Make sure pictures come last
        if ((fd = fopen(str, "r")) == NULL) {
            Log(8, "fopen failed (%s)...\n", str);
            return 0;
        }
        
        while ((pos = fgets(buf, 1023, fd)) != 0) {
            Log(7, buf);
            if (strcmp(buf, "%picture\n") == 0) {
                if ((pos = fgets(buf, 1023, fd)) != 0) {
                    // Skip the name
                }
                if ((pos = fgets(buf, 1023, fd)) != 0) {
                    int endPos = 0;
                    if (buf[0] != '/') {
                        endPos = strlen(buf) +
                            strlen(SDesk::getInstance().getRootDir());
                        strcpy(location, SDesk::getInstance().getRootDir());
                        strcat(location, "/");
                        strcat(location, buf);
                    } else {
                        endPos = strlen(buf) - 1;
                        strcpy(location, buf);
                    }
                    location[endPos] = 0;
                }
                prot = false;
                if ((pos = fgets(buf, 1023, fd)) != 0) {
                    if (strcmp(buf, "1\n") == 0) {
                        prot = true;
                    }
                }
                Log(7, "SDeskItemMenu::Play() picture\n");
                if ((SDesk::getInstance().getProtected() && !prot) ||
                    !SDesk::getInstance().getProtected()) {
                    fprintf(outFd, "/bin/sh \"%s\" \"%s\" \"%s\"\n",
                            SDesk::getInstance().getShowpicture(),
                            SDesk::getInstance().getPictureViewer(),
                            location);
                    fwrite(PHOTO_DELAY, sizeof(char),
                           strlen(PHOTO_DELAY), outFd);
                    retVal = 1;
                }
            }
        }
        fclose(fd);
        
        fclose(outFd);
        
        // Play the songs in the menu
        if (retVal) {
            val = CascadeSpawn::Spawn("/bin/sh \"/tmp/slideshow\"");
            if (!val) {
                Log(1, "Could not spawn!\n");
            }
        }
        Log(7, "SDeskItemMenu::Play() done true\n");
        retVal = 1;
    } else {
        Log(7, "SDeskItemMenu::Play() NOT .mnu\n");
        if (mMenuType == M_SONGS) {
        Log(7, "SDeskItemMenu::Play() NOT .mnu11\n");
            sprintf(buf, "/bin/sh \"%s\" \"%s\" \"%s\"",
                    SDesk::getInstance().getPlaymusic(),
                    SDesk::getInstance().getMusicPlayer(), str);
            Log(7, "SDeskItemMenu::Play(%s) songs\n", buf);
            
            val = CascadeSpawn::Spawn(buf);
            if (!val) {
                Log(1, "Could not spawn!\n");
            }
            Log(7, "SDeskItemMenu::Play() done true\n");
            retVal = 1;
        } else if (mMenuType == M_PICTURES) {
            sprintf(buf, "/bin/sh \"%s\" \"%s\" \"%s\"",
                    SDesk::getInstance().getSlideshow(),
                    SDesk::getInstance().getPictureViewer(), str);
            Log(7, "SDeskItemMenu::Play(%s) pictures\n", buf);
            
            val = CascadeSpawn::Spawn(buf);
            if (!val) {
                Log(1, "Could not spawn!\n");
            }
            Log(7, "SDeskItemMenu::Play() done true\n");
            retVal = 1;
        }
    }
    Log(7, "SDeskItemMenu::Play() done\n");
    return retVal;

#if 0
    if (mMenuType == M_SONGS) {
        sprintf(buf, "/bin/sh \"%s\" \"%s\" \"%s\"",
                SDesk::getInstance().getPlaymusic(),
                SDesk::getInstance().getMusicPlayer(), str);
        Log(7, "SDeskItemMenu::Play(%s) songs\n", buf);

        val = CascadeSpawn::Spawn(buf);
        if (!val) {
            Log(1, "Could not spawn!\n");
        }
        Log(7, "SDeskItemMenu::Play() done true\n");
        retVal = 1;
    } else if (mMenuType == M_PICTURES) {
        sprintf(buf, "/bin/sh \"%s\" \"%s\" \"%s\"",
                SDesk::getInstance().getSlideshow(),
                SDesk::getInstance().getPictureViewer(), str);
        Log(7, "SDeskItemMenu::Play(%s) pictures\n", buf);

        val = CascadeSpawn::Spawn(buf);
        if (!val) {
            Log(1, "Could not spawn!\n");
        }
        Log(7, "SDeskItemMenu::Play() done true\n");
        retVal = 1;
    } else {
        vector<SDeskItem *>::const_iterator it, end = mItems.end();

        // Open the script file
        if ((fd = fopen("/tmp/slideshow", "w")) == NULL) {
            Log(8, "fopen failed (%s)...\n", "/tmp/slideshow");
            return 0;
        }
        
        fwrite("#!/bin/sh\n", sizeof(char), 10, fd);

        // push the songs...
        Log(7, "SDeskItemMenu::Play() favorites\n");

        for(it = mItems.begin(); it != end; it++) {
            if ((*it)->getType() == I_SONG) {
                const char* str = (*it)->getFileName();
                sprintf(buf, "/bin/sh \"%s\" \"%s\" \"%s\"\n",
                        SDesk::getInstance().getPlaysong(),
                        SDesk::getInstance().getMusicPlayer(), str);
                Log(7, "SDeskItemMenu::Play(%s)\n", buf);

                fwrite(buf, sizeof(char), strlen(buf), fd);
                retVal = 1;
            }
        }

        // Push the pictures
        for(it = mItems.begin(); it != end; it++) {
            if ((*it)->getType() == I_PICTURE) {
                const char* str = (*it)->getFileName();
                sprintf(buf, "/bin/sh \"%s\" \"%s\" \"%s\"\n",
                        SDesk::getInstance().getShowpicture(),
                        SDesk::getInstance().getPictureViewer(), str);
                Log(7, "SDeskItemMenu::Play(%s)\n", buf);

                fwrite(buf, sizeof(char), strlen(buf), fd);
                fwrite(PHOTO_DELAY, sizeof(char), strlen(PHOTO_DELAY), fd);
                retVal = 1;
            }
        }

        fclose(fd);

        if (retVal) {
            val = CascadeSpawn::Spawn("/bin/sh \"/tmp/slideshow\"");
            if (!val) {
                Log(1, "Could not spawn!\n");
            }
        }
        Log(7, "SDeskItemMenu::Play() done true\n");
    }
    Log(7, "SDeskItemMenu::Play() done\n");
    return(retVal);
#endif
}

void SDeskItemMenu::Select()
{
    Log(7, "SDeskItemMenu::Select()\n");

    const char *path = mFilename;
    struct stat filestat;
    
    SDesk::getInstance().setCurrentMenu(this);
    
    if (mItems.size() != 0) {
        // Already scanned this...
        
        // Select the first item if we haven't done so
        if (mPos == 0) {
            mPos = mItems.begin();
            mFirstDisplay = mItems.begin();
            (*mPos)->setSelected(true);
        }
        Log(7, "SDeskItemMenu::Select() done\n");
        return;
    }
    
    // Determine the type of the menu item (file or directory).
    mPos = 0;
    mFirstDisplay = 0;
    mLastDisplay = 0;
    
    stat(path, &filestat);
    if (S_ISDIR(filestat.st_mode)) {
        if (!ScanDirectory()) {
            // error in the processing.
            Log(7, "SDeskItemMenu::Select() done\n");
            return;
        }
    } else {
        if (!ReadMenuFile()) {
            // error in the processing.
            Log(7, "SDeskItemMenu::Select() done\n");
            return;
        }
    }

    // Select the first item if we haven't done so
    if (mPos == 0 && mItems.size() > 0) {
        mPos = mItems.begin();
        mFirstDisplay = mItems.begin();
        (*mPos)->setSelected(true);
    }

    Log(7, "SDeskItemMenu::Select() done\n");
}

void SDeskItemMenu::ShowSubMenu()
{
    Log(7, "SDeskItemMenu::ShowSubMenu()\n");

    if (mPos != 0) {
        // Not a menu... show temporary sub menu
        char name[100];
        char type[100];
        char location[100];
    
        // Build the sub menu
        Log(7, "SDeskItemMenu::ShowSubMenu(%d)\n", (*mPos)->getType());
        if ((*mPos)->getType() == I_MESSAGE) {
            strcpy(name, "Warning!");
            strcpy(type, "message");
        } else if ((*mPos)->getType() == I_RSS_FEED) {
            strcpy(name, "News");
            strcpy(type, "rssfeed");
        } else if ((*mPos)->getType() == I_RSS) {
            strcpy(name, "News Message");
            strcpy(type, "rss");
        } else if ((*mPos)->getType() == I_MOVIE_INFO) {
            strcpy(name, "Movie Info");
            strcpy(type, "movieinfo");
        } else if ((*mPos)->getType() == I_SHOW_PLAY_LIST) {
            strcpy(name, "Play List");
            strcpy(type, "playlist");
        } else {
            strcpy(name, "Favorites");
            strcpy(type, "submenu");
        }
        strcpy(location, "");
        if (mSubMenu != 0) {
            delete mSubMenu;
        }
        
        mSubMenu =
            new SDeskItemMenu(name, type, location, false, this);
        if ((*mPos)->getType() == I_PICTURE) {
            // Add the play item for all choices
            mSubMenu->mItems.insert(mSubMenu->mItems.end(),
                new SDeskItemPicture("Show",
                                     (*mPos)->getFileName(), false));
            if (mCanRemoveFiles) {
                mSubMenu->mItems.insert(mSubMenu->mItems.end(),
                    new SDeskItemRemoveFile((*mPos)->getName(),
                                            (*mPos)->getFileName()));
            }
            if (!SDesk::getInstance().isNoSecurity() &&
                !SDesk::getInstance().getProtected()) {
                mSubMenu->mItems.insert(mSubMenu->mItems.end(),
                    new SDeskItemProtect((*mPos)->getName(),
                                         (*mPos)->getFileName()));
            }
            if (mMenuType == M_FAVORITES) {
                mSubMenu->mItems.insert(mSubMenu->mItems.end(),
                    new SDeskItemFavoriteRemove(mFilename,
                                                (*mPos)->getName(),
                                                (*mPos)->getFileName()));
            } else {
                mSubMenu->AppendFavorites(mMenuType,
                    ((SDeskItemPicture *) (*mPos))->getItemDefinition());
            }
        } else if ((*mPos)->getType() == I_RADIO) {
            // Add the play item for all choices
            mSubMenu->mItems.insert(mSubMenu->mItems.end(),
                 new SDeskItemRadio("Play",
                                      (*mPos)->getFileName(), false));
            if (!SDesk::getInstance().isNoSecurity() &&
                !SDesk::getInstance().getProtected()) {
                mSubMenu->mItems.insert(mSubMenu->mItems.end(),
                    new SDeskItemProtect((*mPos)->getName(),
                                         (*mPos)->getFileName()));
            }
            if (mMenuType == M_FAVORITES) {
                mSubMenu->mItems.insert(mSubMenu->mItems.end(),
                    new SDeskItemFavoriteRemove(mFilename,
                                                (*mPos)->getName(),
                                                (*mPos)->getFileName()));
            } else {
                mSubMenu->AppendFavorites(mMenuType,
                    ((SDeskItemRadio *) (*mPos))->getItemDefinition());
            }
        } else if ((*mPos)->getType() == I_SONG) {
            // Add the play item for all choices
            mSubMenu->mItems.insert(mSubMenu->mItems.end(),
                 new SDeskItemSong("Play",
                                   (*mPos)->getFileName(), false));
            if (mCanRemoveFiles) {
                mSubMenu->mItems.insert(mSubMenu->mItems.end(),
                    new SDeskItemRemoveFile((*mPos)->getName(),
                                            (*mPos)->getFileName()));
            }
            if (!SDesk::getInstance().isNoSecurity() &&
                !SDesk::getInstance().getProtected()) {
                mSubMenu->mItems.insert(mSubMenu->mItems.end(),
                    new SDeskItemProtect((*mPos)->getName(),
                                         (*mPos)->getFileName()));
            }
            if (mMenuType == M_FAVORITES) {
                mSubMenu->mItems.insert(mSubMenu->mItems.end(),
                    new SDeskItemFavoriteRemove(mFilename,
                                                (*mPos)->getName(),
                                                (*mPos)->getFileName()));
            } else {
                mSubMenu->AppendFavorites(mMenuType,
                    ((SDeskItemSong *) (*mPos))->getItemDefinition());
            }
        } else if ((*mPos)->getType() == I_MOVIE) {
            // Add the play item for all choices
            mSubMenu->mItems.insert(mSubMenu->mItems.end(),
                 new SDeskItemMovie("Play",
                                    (*mPos)->getFileName(), false));
            mSubMenu->mItems.insert(mSubMenu->mItems.end(),
                 new SDeskItemQueueVideo((*mPos)->getName(),
                                         (*mPos)->getFileName()));
            mSubMenu->mItems.insert(mSubMenu->mItems.end(),
                 new SDeskItemMovieInfo((*mPos)->getName(),
                                        (*mPos)->getFileName()));
            if (mCanRemoveFiles) {
                mSubMenu->mItems.insert(mSubMenu->mItems.end(),
                    new SDeskItemRemoveFile((*mPos)->getName(),
                                            (*mPos)->getFileName()));
            }
            if (!SDesk::getInstance().isNoSecurity() &&
                !SDesk::getInstance().getProtected()) {
                mSubMenu->mItems.insert(mSubMenu->mItems.end(),
                    new SDeskItemProtect((*mPos)->getName(),
                                         (*mPos)->getFileName()));
            }
            if (mMenuType == M_FAVORITES) {
                mSubMenu->mItems.insert(mSubMenu->mItems.end(),
                    new SDeskItemFavoriteRemove(mFilename,
                                                (*mPos)->getName(),
                                                (*mPos)->getFileName()));
            } else {
                mSubMenu->AppendFavorites(mMenuType,
                    ((SDeskItemMovie *) (*mPos))->getItemDefinition());
            }
        } else if ((*mPos)->getType() == I_DVD) {
            // Add the play item for all choices
            const char * tmpName = (*mPos)->getFileName();
            char buffer[1024];
            struct stat filestat;

            mSubMenu->mItems.insert(mSubMenu->mItems.end(),
                 new SDeskItemDVD("Play",
                                  (*mPos)->getFileName(), false));
            sprintf(buffer, "%s/VIDEO_TS", tmpName);
            if (stat(buffer, &filestat) != 0) {
                sprintf(buffer, "%s", tmpName);
            }
            mSubMenu->mItems.insert(mSubMenu->mItems.end(),
                 new SDeskItemMovieInfo((*mPos)->getName(),
                                        (*mPos)->getFileName()));
            mSubMenu->mItems.insert(
                mSubMenu->mItems.end(),
                new SDeskItemMenu("VOBs", "movies",
                                  buffer, false, this));
            if (!SDesk::getInstance().isNoSecurity() &&
                !SDesk::getInstance().getProtected()) {
                mSubMenu->mItems.insert(mSubMenu->mItems.end(),
                    new SDeskItemProtect((*mPos)->getName(),
                                         (*mPos)->getFileName()));
            }
            if (mMenuType == M_FAVORITES) {
                mSubMenu->mItems.insert(mSubMenu->mItems.end(),
                    new SDeskItemFavoriteRemove(mFilename,
                                                (*mPos)->getName(),
                                                (*mPos)->getFileName()));
            } else {
                mSubMenu->AppendFavorites(mMenuType,
                    ((SDeskItemDVD *) (*mPos))->getItemDefinition());
            }
        } else if ((*mPos)->getType() == I_MENU) {
            // Add the play item for all choices
            mSubMenu->mItems.insert(mSubMenu->mItems.end(),
                new SDeskItemMenu((*mPos)->getName(), "misc",
                                  (*mPos)->getFileName(), false, this));
            if (!SDesk::getInstance().isNoSecurity() &&
                !SDesk::getInstance().getProtected()) {
                mSubMenu->mItems.insert(mSubMenu->mItems.end(),
                    new SDeskItemProtect((*mPos)->getName(),
                                         (*mPos)->getFileName()));
            }
            if (mMenuType == M_FAVORITES) {
                mSubMenu->mItems.insert(mSubMenu->mItems.end(),
                    new SDeskItemFavoriteRemove(mFilename,
                                                (*mPos)->getName(),
                                                (*mPos)->getFileName()));
            } else {
                mSubMenu->AppendFavorites(mMenuType,
                    ((SDeskItemMenu *) (*mPos))->getItemDefinition());
            }
        } else if ((*mPos)->getType() == I_MESSAGE) {
        } else if ((*mPos)->getType() == I_MOVIE_INFO) {
        } else if ((*mPos)->getType() == I_RSS) {
        } else if ((*mPos)->getType() == I_RSS_FEED) {
            ReadNewsFeed((*mPos)->getFileName());
        } else if ((*mPos)->getType() == I_SHOW_PLAY_LIST) {
            ReadPlayList();
        }

        // Invoke the sub menu
        SDesk::getInstance().setCurrentMenu(mSubMenu);
        mSubMenu->Select();
    }
    Log(7, "SDeskItemMenu::ShowSubMenu() done\n");
}

void SDeskItemMenu::ReadNewsFeed(
    const char* url)
{
    Log(7, "SDeskScan::ReadNewsFeed(%s)\n", url);

    CascadeHTTPClient myClient; 
    mxml_node_t *tree; 

    myClient.SetURL(url); 
    myClient.GET(); 

    u32 buffsize = 8192; 

    char filename[1024];
    char buff[buffsize]; 
    u32 bytesReturned = 0; 
    u32 totalBytes = 0; 

    // Dump the URL to a file.
    strcpy(filename, SDesk::getInstance().getRootDir());
    strcat(filename, "/temp.xml");
    FILE *pOutputFile = fopen(filename, "wb"); 
    if (NULL == pOutputFile) { 
        Log(7, "SDeskScan::ReadNewsFeed(%s) write error\n", url);
        return;
    }

    while(1) { 
        bytesReturned = myClient.ReadBytes(buff,buffsize); 
        if (0 == bytesReturned) {
            break; 
        }
        if (bytesReturned != fwrite(buff, 1, bytesReturned, pOutputFile)) { 
            Log(7, "SDeskScan::ReadNewsFeed(%s) read error\n", url);
        }
        totalBytes += bytesReturned; 

        if (bytesReturned < buffsize) {
            break; 
        }
    }
    fclose(pOutputFile); 

    // Read the XML file
    FILE *fp; 
    fp = fopen(filename, "r"); 
    tree = mxmlLoadFile(NULL, fp, MXML_NO_CALLBACK); 
    fclose(fp); 

    // Find the interesting elements.
    mxml_node_t *node; 
    char title[1024];
    char description[1024];
    bool inItem = false;
    
    title[0] = 0;
    description[0] = 0;
    for (node = mxmlFindElement(tree, tree, NULL, NULL, NULL, MXML_DESCEND); 
         node != NULL; 
         node = mxmlFindElement(node, tree, NULL, NULL, NULL, MXML_DESCEND)) { 
        if (strcmp("item", node->value.element.name) == 0) { 
            inItem = true;
        }
        if (inItem && strcmp("title", node->value.element.name) == 0) { 
            mxml_node_t *child_node; 
            child_node = mxmlWalkNext(node, tree, MXML_DESCEND); 
            if (child_node == NULL ||
                child_node->value.text.string == NULL) { 
                Log(1, "SDeskScan::ReadNewsFeed(%s) no title\n", url);
            } else {
                int len = strlen(child_node->value.text.string);
                if (len > 1023) {
                    len = 1023;
                }
                strncpy(title, child_node->value.text.string, len);
                title[len] = 0;
                child_node = mxmlWalkNext(child_node, node, MXML_DESCEND); 
                while (child_node != NULL &&
                       child_node->value.text.string != NULL) { 
                    if (strlen(title) + 
                        strlen(child_node->value.text.string) + 1 < 1023) {
                        strcat(title, " ");
                        strcat(title, child_node->value.text.string); 
                        child_node = mxmlWalkNext(child_node,
                                                  node, MXML_DESCEND); 
                    } else {
                        break;
                    }
                } 
            }
        }
        if (inItem && strcmp("description", node->value.element.name) == 0) { 
            mxml_node_t *child_node; 
            child_node = mxmlWalkNext(node, tree, MXML_DESCEND); 
            if(child_node == NULL) {
                Log(1, "SDeskScan::ReadNewsFeed(%s) no description\n",
                    url);
            } else {
                    Log(1, 
                        "SDeskScan::ReadNewsFeed(%d == %d)\n",
                        child_node->type, MXML_OPAQUE);
                if (child_node->type == MXML_TEXT) {
                    if (child_node->value.text.string == NULL) { 
                        Log(1, "SDeskScan::ReadNewsFeed(%s) no description\n",
                            url);
                    } else {
                        int counter = 0;                               
                        int targetPos = 0;
                        bool inTag = false;
                        bool firstChar = true;
                        char *tmpPos = child_node->value.text.string;
                        int len = strlen(tmpPos);
                        while(targetPos < 1024 && counter < len) {
                            // Found an html tag.
                            if (counter  < len &&
                                counter + 1  < len &&
                                tmpPos[counter] == 'a' &&
                                tmpPos[counter + 1] == '>') {
                                // Found the closing a tag.
                                inTag = false;
                                counter++;
                                counter++;
                                continue;
                            }
                            if (inTag) {
                                // We are inside the a tag... skip
                                counter++;
                                continue;
                            }
                            if (tmpPos[counter] == '<') {
                                if (counter + 1 < len &&
                                    (tmpPos[counter + 1] == 'p' ||
                                     (counter + 2 < len && 
                                      tmpPos[counter + 1] == 'b' &&
                                      tmpPos[counter + 1] == 'r'))) {
                                    // Just skip to the end...
                                    while (counter < len &&
                                           tmpPos[counter] != '>') {
                                        counter++;
                                    }
                                    counter++;
                                    continue;
                                }
                                if (counter + 1 < len &&
                                    tmpPos[counter + 1] == 'a') {
                                    // Just skip to the end...
                                    inTag = true;
                                    counter++;
                                    continue;
                                }
                            }
                            description[targetPos] = tmpPos[counter];
                            targetPos++;
                            counter++;
                        }
                        description[targetPos] = 0;
                        child_node = mxmlWalkNext(child_node,
                                                  node, MXML_DESCEND); 
                        while (child_node != NULL &&
                               child_node->value.text.string != NULL) { 
                            counter = 0;
                            firstChar = true;
                            tmpPos = child_node->value.text.string;
                            len = strlen(tmpPos);
                            while(targetPos < 1024 && counter < len) {
                                // Found an html tag.
                                if (counter < len &&
                                    counter + 1 < len &&
                                    tmpPos[counter] == 'a' &&
                                    tmpPos[counter + 1] == '>') {
                                    // Found the closing a tag.
                                    inTag = false;
                                    counter++;
                                    counter++;
                                    continue;
                                }
                                if (counter < len &&
                                    counter + 1 < len &&
                                    counter + 2 < len &&
                                    tmpPos[counter] == 'b' &&
                                    tmpPos[counter + 1] == 'r' &&
                                    tmpPos[counter + 2] == '>') {
                                    // Found the closing a tag.
                                    inTag = false;
                                    counter++;
                                    counter++;
                                    counter++;
                                    continue;
                                }
                                if (inTag) {
                                    counter++;
                                    continue;
                                }
                                if (tmpPos[counter] == '<') {
                                    if (counter + 1 < len &&
                                        ((tmpPos[counter + 1] == 'p') ||
                                         (counter + 2 < len && 
                                          tmpPos[counter + 1] == 'b' &&
                                          tmpPos[counter + 2] == 'r'))) {
                                        // Just skip to the end...
                                        inTag = true;
                                        counter++;
                                        continue;
                                    }
                                    if (counter + 1 < len &&
                                        counter + 2 < len &&
                                        ((tmpPos[counter + 1] == '/' &&
                                          tmpPos[counter + 2] == 'p')||
                                         (counter + 3 < len && 
                                          tmpPos[counter + 1] == '/' &&
                                          tmpPos[counter + 2] == 'b' &&
                                          tmpPos[counter + 3] == 'r'))) {
                                        // Just skip to the end...
                                        while (counter < len &&
                                               tmpPos[counter] != '>') {
                                            counter++;
                                        }
                                        counter++;
                                        continue;
                                    }
                                    if (counter + 1 < len &&
                                        tmpPos[counter + 1] == 'a') {
                                        // Just skip to the end...
                                        inTag = true;
                                        counter++;
                                        continue;
                                    }
                                }
                                if (firstChar) {
                                    description[targetPos] = ' ';
                                    targetPos++;
                                }
                                description[targetPos] = tmpPos[counter];
                                targetPos++;
                                counter++;
                                firstChar = false;
                            }
                            child_node = mxmlWalkNext(child_node, node,
                                                      MXML_DESCEND); 
                        }
                        description[targetPos] = 0;
                    }
                } else {
                    if (child_node->type == MXML_OPAQUE) {
                        Log(1, "SDeskScan::ReadNewsFeed OPAQUE\n");
                        if (child_node->value.opaque == NULL) { 
                            Log(1, 
                                "SDeskScan::ReadNewsFeed(%s) no description\n",
                                url);
                        } else {
                            Log(1, "test %s\n", child_node->value.opaque);
                            int len = strlen(child_node->value.opaque);
                            if (len > 1023) {
                                len = 1023;
                            }
                            strncpy(description,
                                    child_node->value.opaque, len);
                            description[len] = 0;
#if 0
                            child_node = mxmlWalkNext(child_node,
                                                      node, MXML_DESCEND); 
                            while (child_node != NULL &&
                                   child_node->value.text.string != NULL) { 
                                if (strlen(description) +
                                    strlen(child_node->value.text.string) +
                                    1 < 1023) {
                                    strcat(description, " ");
                                    strcat(description,
                                           child_node->value.text.string); 
                                    child_node = mxmlWalkNext(child_node, node,
                                                              MXML_DESCEND); 
                                } else {
                                    break;
                                }
                            }
#endif
                        }
                    }
                }
            } 
        } 
        if (inItem && title[0] != 0 && description[0] != 0) {
            if (isascii(description[0])) {
                mSubMenu->mItems.insert(mSubMenu->mItems.end(),
                                        new SDeskItemRSS(title, description,
                                                         false));
            }
            title[0] = 0;
            description[0] = 0;
        }
    }

    Log(7, "SDeskScan::ReadNewsFeed()\n");
}

void SDeskItemMenu::ReadPlayList()
{
    Log(7, "SDeskScan::ReadPlayList()\n");

    FILE *fd;
    char *pos;
    char buf[1024];
    int endPos = 0;
    int lineCount = 0;

    // Read the playlist file
    if ((fd = fopen("/tmp/showplaylist", "r")) == NULL) {
        mSubMenu->mItems.insert(mSubMenu->mItems.end(),
                                new SDeskItemApp(
                                    "No files are being played.", "", false));
        return;
    }

    while ((pos = fgets(buf, 1023, fd)) != 0) {
        endPos = strlen(buf) - 1;
        if (buf[endPos] == '\n') {
            buf[endPos] = 0;
        }
        if (strlen(buf) == 0 || strncmp("sonicaApp:", buf, 10) == 0) {
            continue;
        }
        lineCount++;
        mSubMenu->mItems.insert(mSubMenu->mItems.end(),
                                new SDeskItemApp(
                                    buf, "", false));
    }

    // Give a message when there was nothing to show...
    if (lineCount == 0) {
        mSubMenu->mItems.insert(mSubMenu->mItems.end(),
                                new SDeskItemApp(
                                    "No files are being played.", "", false));
    }

    fclose(fd); 

    Log(7, "SDeskScan::ReadPlayList()\n");
}

void SDeskItemMenu::Rebuild()
{
    Log(7, "SDeskItemMenu::Rebuild()\n");
    
    for (unsigned int i = 0; i < mItems.size(); i++) {
        delete mItems[i];
    }
    mItems.clear();

    Select();

    Log(7, "SDeskItemMenu::Rebuild() done\n");
}

void SDeskItemMenu::Draw(
    CascadeBitmap& bitmap)
{
    const char* str = mName;

    Log(7, "SDeskItemMenu::Draw(%s,%d,%d)\n", str, mType, mMenuType);

    // Draw our title.
    CascadeRect r = CascadeRect(
        SDesk::getInstance().getCurrentTheme().getTitleLeftOffset(),
        SDesk::getInstance().getCurrentTheme().getTitleTopOffset(),
        SDesk::getInstance().getCurrentTheme().getTitleCellWidth(),
        SDesk::getInstance().getCurrentTheme().getTitleCellHeight());
    bitmap.DrawText(
        SDesk::getInstance().getCurrentTheme().getTitleFont(),
        r, mName,
        SDesk::getInstance().getCurrentTheme().getTitleColor(),
        TEXT_CENTER_VERTICALLY |
        TEXT_JUSTIFY_LEFT | TEXT_CLIP_WITH_DOTDOTDOT);

    // See if we have any warning messages to display
    if (mMenuType == M_MESSAGE) {
        SDesk::getInstance().DrawMessage(bitmap);
        return;
    }
    // See if this is a rss feed... show the message if yes.
    if (mMenuType == M_RSS) {
        ((SDeskItemRSS *) (* mParentMenu->mPos))->DrawMessage(bitmap);
        return;
    }
    // See if this is a movie info... show the details if yes.
    if (mMenuType == M_MOVIE_INFO) {
        ((SDeskItemMovieInfo *) (* mParentMenu->mPos))->DrawMessage(bitmap);
        return;
    }

    // Show warning messages in the toplevel menu.
    if (mParentMenu == 0 && !mHaveWarning && 
        SDesk::getInstance().HaveMessage()) {
        if (mPos) {
            (*mPos)->setSelected(false);
        }
        mItems.insert(mItems.begin(),
                      new SDeskItemMessage("Warning", "", false));

        mHaveWarning = true;

        // Reset the current item pos.
        mPos = mItems.begin();
        if (mPos) {
            (*mPos)->setSelected(true);
        }
        mFirstDisplay = mItems.begin();
        mLastDisplay = 0;
    }

    // Sanity check...
    if (mFirstDisplay == 0) {
        // Reset the current item pos.
        mPos = mItems.begin();
        if (mPos) {
            (*mPos)->setSelected(true);
        }
        mFirstDisplay = mItems.begin();
        mLastDisplay = 0;
    }

    if (mFirstDisplay == 0) {
        Log(7, "SDeskItemMenu::Draw() No first item\n");
        return;
    }

    // Draw all our sub items.
    vector<SDeskItem *>::const_iterator it, end = mItems.end();
    int x = SDesk::getInstance().getCurrentTheme().getTextLeftOffset();
    int y = SDesk::getInstance().getCurrentTheme().getTextTopOffset();
    int counter = SDesk::getInstance().getCurrentTheme().getTextRows();
    for(it = mFirstDisplay; it != end && counter > 0; it++, counter--) {
        int menuType = M_MISC;
        if ((*it)->getType() == I_MENU) {
            menuType = ((SDeskItemMenu *) (*it))->getMenuType();
        }
        if ((*it)->getSelected()) {
            bool north = true;
            bool south = true;
            bool west = true;
            bool east = true;
//            if (it == mItems.begin()) {
//                north = false;
//            }
//            if (it == end - 1) {
//                south = false;
//            }
            if (mParentMenu == 0) {
                west = false;
            }
            if ((*it)->getType() == I_RSS) {
                SDesk::getInstance().getCurrentTheme().DrawSelected(
                    bitmap, x, y, north, south, west, east, true);
            } else {
                SDesk::getInstance().getCurrentTheme().DrawSelected(
                    bitmap, x, y, north, south, west, east, false);
            }
        }
        if (SDesk::getInstance().getShowLittleIcons()) {
            SDesk::getInstance().getCurrentTheme().DrawType(
                bitmap, (*it)->getType(), menuType, x, y);
        }

        // Track the position of the last displayed item.
        mLastDisplay = it;
        // Show the current item.
        if ((*it)->getType() == I_RSS) {
            ((SDeskItemRSS *) (*it))->DrawText(bitmap, x, y, menuType);
        } else {
            Log(7, "SDeskItemMenu::Draw() gettype %d\n", (*it)->getType());
            (*it)->DrawText(bitmap, x, y, menuType);
            if ((*it)->getSelected()) {
                (*it)->DrawDetailText(bitmap);
            }
        }

        // Display some help for common items...
        if (mMenuType != M_SUBMENU) {
            if ((*it)->getType() == I_MOVIE) {
                SDesk::getInstance().getCurrentTheme().SetStatusMessage(
                    "Press <Play> to play movie...");
            } else if ((*it)->getType() == I_DVD) {
                SDesk::getInstance().getCurrentTheme().SetStatusMessage(
                    "Press <Play> to play DVD...");
            } else if ((*it)->getType() == I_SONG) {
                SDesk::getInstance().getCurrentTheme().SetStatusMessage(
                    "Press <Play> to play song...");
            } else if ((*it)->getType() == I_RADIO) {
                SDesk::getInstance().getCurrentTheme().SetStatusMessage(
                    "Press <Play> to play station...");
            } else if ((*it)->getType() == I_PICTURE) {
                SDesk::getInstance().getCurrentTheme().SetStatusMessage(
                    "Press <Play> to show picture...");
            } else if ((*it)->getType() == I_APP) {
                SDesk::getInstance().getCurrentTheme().SetStatusMessage(
                    "Press <Play> to run application...");
            } else if ((*it)->getType() == I_PLAY) {
                SDesk::getInstance().getCurrentTheme().SetStatusMessage(
                    "Press <Play> to play favorites...");
            } else if (mMenuType == M_SONGS) {
                SDesk::getInstance().getCurrentTheme().SetStatusMessage(
                    "Press <Play> to play all songs...");
            } else if (mMenuType == M_PICTURES) {
                SDesk::getInstance().getCurrentTheme().SetStatusMessage(
                    "Press <Play> to show all pictures...");
            }
        }

        // Increment the y position for the next item.
        y += SDesk::getInstance().getCurrentTheme().getTextRowOffset();
        // Add any additional offset for subsequent items.
        y += (*it)->getTopOffset();
    }

    Log(7, "SDeskItemMenu::Draw() done\n");
}

bool SDeskItemMenu::ScanDirectory()
{
    char buf[1024];
    char buf2[1024];
    char buf3[1024];
    char protName[1024];
    char removeName[1024];
    bool prot = false;
    unsigned int extPos = 0;
    const char *path = mFilename;
    struct stat filestat;

    Log(7, "SDeskItemMenu::ScanDirectory(%s)\n", path);

    // Its a directory... show the content.
    SDeskDirScanner dir(mFilename);

    for(u32 pos = 0; pos < dir.GetNumEntries(); ++pos) {
        // Go through all items in the directory.
        const char *entry = dir.GetEntry(pos);
        
        if (!entry) {
            // Not a valid entry.
            continue;
        }

        // See if this is a hidden file
        if (SDesk::getInstance().IsExcluded(entry)) {
            continue;
        }

        // Check whether there is a removal file, meaning
        // files in this directory can be removed.
        strcpy(removeName, path);
        strcat(removeName, "/sdesk.remove");
        if (stat(removeName, &filestat) == 0) {
            mCanRemoveFiles = true;
        }

        prot = false;
        sprintf(buf, "%s/%s", path, entry);
        if (stat(buf, &filestat) == 0 && S_ISDIR(filestat.st_mode)) {
            // A subdirectory... create a submenu item.

            // Check whether there is a protection file, meaning
            // this file is not to be shown.
            strcpy(protName, buf);
            strcat(protName, ".sdprot");
            if (stat(protName, &filestat) == 0) {
                prot = true;
            }

            if ((SDesk::getInstance().getProtected() && !prot) ||
                !SDesk::getInstance().getProtected()) {
                // See if this is a DVD directoy
                sprintf(buf2, "%s/%s", path, entry);
                SDeskDirScanner *dir = new SDeskDirScanner(buf2, "VTS");
                if (dir->GetNumEntries() == 0) {
                    delete dir;
                    sprintf(buf2, "%s/%s/VIDEO_TS", path, entry);
                    dir = new SDeskDirScanner(buf2, "VTS");
                }
                if (dir->GetNumEntries() > 0) {
                    mItems.insert(mItems.end(), 
                                  new SDeskItemDVD(entry, buf, prot));
                } else {
                    mItems.insert(mItems.end(),
                                  new SDeskItemMenu(entry, mMenuTypeName,
                                                    buf, prot, this));
                }
                delete dir;
            }
        } else {
            // A file... check for supported file types.
            if (SDesk::getInstance().IsVideo(entry, &extPos)) {
                // A movie file.
                sprintf(buf, "%s/%s", path, entry);
                // Chop off the extension.
                strcpy(buf2, entry);
                buf2[extPos] = 0;

                // Check whether there is a protection file, meaning
                // this file is not to be shown.
                strcpy(protName, buf);
                strcat(protName, ".sdprot");
                if (stat(protName, &filestat) == 0) {
                    prot = true;
                }
                if ((SDesk::getInstance().getProtected() && !prot) ||
                    !SDesk::getInstance().getProtected()) {
                    mItems.insert(mItems.end(), 
                                  new SDeskItemMovie(buf2, buf, prot));
                }
            } else if (SDesk::getInstance().IsMusic(entry, &extPos)) {
                // A song file.
                sprintf(buf, "%s/%s", path, entry);
                // Chop off the extension.
                strcpy(buf2, entry);
                buf2[extPos] = 0;

                // Check whether there is a protection file, meaning
                // this file is not to be shown.
                strcpy(protName, buf);
                strcat(protName, ".sdprot");
                if (stat(protName, &filestat) == 0) {
                    prot = true;
                }
                if ((SDesk::getInstance().getProtected() && !prot) ||
                    !SDesk::getInstance().getProtected()) {
                    mItems.insert(mItems.end(), 
                                  new SDeskItemSong(buf2, buf, prot));
                }
            } else if (SDesk::getInstance().IsPicture(entry, &extPos)) {
                int pos;

                // A picture file.
                sprintf(buf, "%s/%s", path, entry);
                // Chop off the extension.
                strcpy(buf2, entry);
                buf2[extPos] = 0;
                // See of an _edited file exists for this file... if
                // there is one... skip this file.
                strcpy(buf3, path);
                strcat(buf3, "/");
                strcat(buf3, buf2);
                strcat(buf3, "_edited");
                strcat(buf3, entry + strlen(entry) - 4);
                if (stat(buf3, &filestat) == 0) {
                    continue;
                }

                // See if there exists a file with the same name
                // but with _edited. This is the file we want to 
                // insert.
                pos = strlen(buf2);
                if (pos - 7 > 0) {
                    if (buf2[pos - 7] == '_' && buf2[pos - 6] == 'e' &&
                        buf2[pos - 5] == 'd' && buf2[pos - 4] == 'i' &&
                        buf2[pos - 3] == 't' && buf2[pos - 2] == 'e' &&
                        buf2[pos - 1] == 'd') {
                        // Ok... this is the file we want to use, but under
                        // the shorter name.
                        buf2[pos - 7] = 0;
                    }
                }

                // Check whether there is a protection file, meaning
                // this file is not to be shown.
                strcpy(protName, buf);
                strcat(protName, ".sdprot");
                if (stat(protName, &filestat) == 0) {
                    prot = true;
                }
                if ((SDesk::getInstance().getProtected() && !prot) ||
                    !SDesk::getInstance().getProtected()) {
                    mItems.insert(mItems.end(), 
                                  new SDeskItemPicture(buf2, buf, prot));
                }
            } else if (strncmp(entry + strlen(entry) - 5, ".roku", 5) == 0) {
                // An application file.
                sprintf(buf, "%s/%s", path, entry);
                // Chop off the extension.
                strcpy(buf2, entry);
                buf2[strlen(buf2) - 5] = 0;

                // Check whether there is a protection file, meaning
                // this file is not to be shown.
                strcpy(protName, buf);
                strcat(protName, ".sdprot");
                if (stat(protName, &filestat) == 0) {
                    prot = true;
                }
                if ((SDesk::getInstance().getProtected() && !prot) ||
                    !SDesk::getInstance().getProtected()) {
                    mItems.insert(mItems.end(), 
                                  new SDeskItemApp(buf2, buf, prot));
                }
            }
        }
    }
    
    Log(7, "SDeskItemMenu::ScanDirectory() done\n");
    return true;
}

bool SDeskItemMenu::ReadMenuFile()
{
    const char *path = mFilename;

    Log(7, "SDeskItemMenu::ReadMenuFile(%s,%d)\n", path, mType);

    // Build the item list.
    FILE *fd;
    char *pos;
    char buf[1024];
    char name[1024];
    char type[1024];
    char location[1024];
    char data1[1024];
    char data2[1024];
    bool prot;
    bool hadFavoriteMedia = false;
    
    if ((fd = fopen(path, "r")) == NULL) {
        Log(8, "fopen failed (%s)...\n", path);
        return false;
    }

    while ((pos = fgets(buf, 1023, fd)) != 0) {
        Log(7, buf);
        if (strcmp(buf, "%application\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) +
                        strlen(SDesk::getInstance().getRootDir());
                    strcpy(location, SDesk::getInstance().getRootDir());
                    strcat(location, "/");
                    strcat(location, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(location, buf);
                }
                location[endPos] = 0;
            }
            prot = false;
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                if (strcmp(buf, "1\n") == 0) {
                    prot = true;
                }
            }
            Log(7, "SDeskItemMenu::ReadMenuFile() app\n");
            if ((SDesk::getInstance().getProtected() && !prot) ||
                !SDesk::getInstance().getProtected()) {
                mItems.insert(mItems.end(),
                              new SDeskItemApp(name, location, prot));
            }
        } else if (strcmp(buf, "%url\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                endPos = strlen(buf) - 1;
                strcpy(location, buf);
                location[endPos] = 0;
            }
            prot = false;
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                if (strcmp(buf, "1\n") == 0) {
                    prot = true;
                }
            }
            Log(7, "SDeskItemMenu::ReadMenuFile() url\n");
            if ((SDesk::getInstance().getProtected() && !prot) ||
                !SDesk::getInstance().getProtected()) {
                mItems.insert(mItems.end(),
                              new SDeskItemURL(name, location, prot));
            }
        } else if (strcmp(buf, "%stopplay\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            Log(7, "SDeskItemMenu::ReadMenuFile() stopplay\n");
            mItems.insert(mItems.end(),
                          new SDeskItemStopPlay(name));
        } else if (strcmp(buf, "%showplaylist\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            Log(7, "SDeskItemMenu::ReadMenuFile() showplaylist\n");
            mItems.insert(mItems.end(),
                          new SDeskItemShowPlayList(name));
        } else if (strcmp(buf, "%playvideoqueue\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            Log(7, "SDeskItemMenu::ReadMenuFile() playvideoqueue\n");
            mItems.insert(mItems.end(),
                          new SDeskItemPlayVideoQueue(name));
        } else if (strcmp(buf, "%clearvideoqueue\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            Log(7, "SDeskItemMenu::ReadMenuFile() clearvideoqueu\n");
            mItems.insert(mItems.end(),
                          new SDeskItemClearVideoQueue(name));
        } else if (strcmp(buf, "%menu\n") == 0) {
            if ((pos = fgets(buf, 20, fd)) != 0) {
                strcpy(type, buf);
                type[strlen(buf) - 1] = 0;
            }
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) +
                        strlen(SDesk::getInstance().getRootDir());
                    strcpy(location, SDesk::getInstance().getRootDir());
                    strcat(location, "/");
                    strcat(location, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(location, buf);
                }
                location[endPos] = 0;
            }
            prot = false;
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                if (strcmp(buf, "1\n") == 0) {
                    prot = true;
                }
            }
            Log(7, "SDeskItemMenu::ReadMenuFile() menu\n");
            if ((SDesk::getInstance().getProtected() && !prot) ||
                !SDesk::getInstance().getProtected()) {
                mItems.insert(mItems.end(),
                              new SDeskItemMenu(name, type, location, 
                                                prot, this));
            }
        } else if (strcmp(buf, "%movie\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) +
                        strlen(SDesk::getInstance().getRootDir());
                    strcpy(location, SDesk::getInstance().getRootDir());
                    strcat(location, "/");
                    strcat(location, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(location, buf);
                }
                location[endPos] = 0;
            }
            prot = false;
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                if (strcmp(buf, "1\n") == 0) {
                    prot = true;
                }
            }
            Log(7, "SDeskItemMenu::ReadMenuFile() movie\n");
            if ((SDesk::getInstance().getProtected() && !prot) ||
                !SDesk::getInstance().getProtected()) {
                mItems.insert(mItems.end(),
                              new SDeskItemMovie(name, location, prot));
            }
        } else if (strcmp(buf, "%dvd\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) +
                        strlen(SDesk::getInstance().getRootDir());
                    strcpy(location, SDesk::getInstance().getRootDir());
                    strcat(location, "/");
                    strcat(location, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(location, buf);
                }
                location[endPos] = 0;
            }
            prot = false;
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                if (strcmp(buf, "1\n") == 0) {
                    prot = true;
                }
            }
            Log(7, "SDeskItemMenu::ReadMenuFile() dvd\n");
            if ((SDesk::getInstance().getProtected() && !prot) ||
                !SDesk::getInstance().getProtected()) {
                mItems.insert(mItems.end(),
                              new SDeskItemDVD(name, location, prot));
            }
        } else if (strcmp(buf, "%picture\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) +
                        strlen(SDesk::getInstance().getRootDir());
                    strcpy(location, SDesk::getInstance().getRootDir());
                    strcat(location, "/");
                    strcat(location, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(location, buf);
                }
                location[endPos] = 0;
            }
            prot = false;
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                if (strcmp(buf, "1\n") == 0) {
                    prot = true;
                }
            }
            Log(7, "SDeskItemMenu::ReadMenuFile() picture\n");
            if ((SDesk::getInstance().getProtected() && !prot) ||
                !SDesk::getInstance().getProtected()) {
                hadFavoriteMedia = true;
                mItems.insert(mItems.end(),
                              new SDeskItemPicture(name, location, prot));
            }
        } else if (strcmp(buf, "%song\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) +
                        strlen(SDesk::getInstance().getRootDir());
                    strcpy(location, SDesk::getInstance().getRootDir());
                    strcat(location, "/");
                    strcat(location, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(location, buf);
                }
                location[endPos] = 0;
            }
            prot = false;
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                if (strcmp(buf, "1\n") == 0) {
                    prot = true;
                }
            }
            Log(7, "SDeskItemMenu::ReadMenuFile() song\n");
            if ((SDesk::getInstance().getProtected() && !prot) ||
                !SDesk::getInstance().getProtected()) {
                hadFavoriteMedia = true;
                mItems.insert(mItems.end(),
                              new SDeskItemSong(name, location, prot));
            }
        } else if (strcmp(buf, "%radio\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                endPos = strlen(buf) - 1;
                strcpy(location, buf);
                location[endPos] = 0;
            }
            prot = false;
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                if (strcmp(buf, "1\n") == 0) {
                    prot = true;
                }
            }
            Log(7, "SDeskItemMenu::ReadMenuFile() radio\n");
            if ((SDesk::getInstance().getProtected() && !prot) ||
                !SDesk::getInstance().getProtected()) {
                mItems.insert(mItems.end(),
                              new SDeskItemRadio(name, location, prot));
            }
        } else if (strcmp(buf, "%rssfeed\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                endPos = strlen(buf) - 1;
                strcpy(location, buf);
                location[endPos] = 0;
            }
            prot = false;
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                if (strcmp(buf, "1\n") == 0) {
                    prot = true;
                }
            }
            Log(7, "SDeskItemMenu::ReadMenuFile() rss\n");
            if ((SDesk::getInstance().getProtected() && !prot) ||
                !SDesk::getInstance().getProtected()) {
                mItems.insert(mItems.end(),
                              new SDeskItemRSSFeed(name, location, prot));
            }
        } else if (strcmp(buf, "%user\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) +
                        strlen(SDesk::getInstance().getRootDir());
                    strcpy(location, SDesk::getInstance().getRootDir());
                    strcat(location, "/");
                    strcat(location, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(location, buf);
                }
                location[endPos] = 0;
            }
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                endPos = strlen(buf) - 1;
                strcpy(data1, buf);
                data1[endPos] = 0;
            }
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                endPos = strlen(buf) - 1;
                strcpy(data2, buf);
                data2[endPos] = 0;
            }
            Log(7, "SDeskItemMenu::ReadMenuFile() user\n");
            mItems.insert(mItems.end(),
                          new SDeskItemUser(name, location, data1, data2));
        } else if (strcmp(buf, "%rescan\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                endPos = strlen(buf) - 1;
                strcpy(data1, buf);
                data1[endPos] = 0;
            }
            prot = false;
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                if (strcmp(buf, "1\n") == 0) {
                    prot = true;
                }
            }
            Log(7, "SDeskItemMenu::ReadMenuFile() rescan\n");
            if ((SDesk::getInstance().getProtected() && !prot) ||
                !SDesk::getInstance().getProtected()) {
                mItems.insert(mItems.end(),
                              new SDeskItemRescan(name, data1, prot));
            }
        } else if (strcmp(buf, "%space\n") == 0) {
            if ((pos = fgets(buf, 10, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            prot = false;
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                if (strcmp(buf, "1\n") == 0) {
                    prot = true;
                }
            }
            if ((SDesk::getInstance().getProtected() && !prot) ||
                !SDesk::getInstance().getProtected()) {
                if (mItems.size() > 0) {
                    Log(7, "SDeskItemMenu::ReadMenuFile() space\n");
                    mItems.back()->setTopOffset(atoi(name));
                }
            }
        }
    }

    fclose(fd);
                                                    
    // If this is a favorites list... sort it...
    if (mMenuType == M_FAVORITES) {
        sort(mItems.begin(), mItems.end(), CompareItem);
        if (hadFavoriteMedia) {
            mItems.insert(mItems.begin(),
                          new SDeskItemPlay("Play", "", false));
        }
    }

    Log(7, "SDeskItemMenu::ReadMenuFile() done\n");
    return true;
}

bool CompareItem(
    const SDeskItem* a, const SDeskItem* b)
{
    const char* string1;
    const char* string2;

    if ((string1 = strrchr(a->mName, '/')) != 0) {
        string1++;
    } else {
        string1 = a->mName;
    }
    if ((string2 = strrchr(b->mName, '/')) != 0) {
        string2++;
    } else {
        string2 = b->mName;
    }
    if (string1 != 0 && string2 != 0) {
        return strcmp(string1, string2) < 0;
    }
    return false;
}

void SDeskItemMenu::AppendFavorites(
    int parentMenuType,
    CascadeString& itemDefinition)
{
    Log(7, "SDeskItemMenu::AppendFavorites()\n");

    // Build the favorites submenu.
    vector<SDeskFavorite *>::const_iterator it, end;
    it = SDesk::getInstance().getFavorites().begin();
    end = SDesk::getInstance().getFavorites().end();
    for(; it != end; it++) {
        Log(7, "SDeskItemMenu::AppendFavorites(%d, %d, %d, %d, %d, %d)\n",
            parentMenuType, (*it)->getType(), M_MISC, M_SLIDESHOW,
            M_SONGS, M_PICTURES);
        if (parentMenuType == (*it)->getType() ||
            (*it)->getType() == M_MISC ||
            ((*it)->getType() == M_SLIDESHOW && (
                 parentMenuType == M_SONGS || parentMenuType == M_PICTURES))) {
            mItems.insert(mItems.end(),
                          new SDeskItemFavoriteList((*it)->getName(),
                                                    (*it)->getFileName(),
                                                    itemDefinition));
        }
    }

    Log(7, "SDeskItemMenu::AppendFavorites() done\n");
}

CascadeString& SDeskItemMenu::getItemDefinition()
{
    char buffer[1024];

    Log(7, "SDeskItemMenu::getItemDefinition\n");

    strcpy(buffer, "%menu\nmisc\n");
    strcat(buffer, mName);
    strcat(buffer, "\n");
    strcat(buffer, mFilename);
    strcat(buffer, "\n");
    if (mProtected) {
        strcat(buffer, "1\n");
    } else {
        strcat(buffer, "0\n");
    }
    mItemDefinition = CascadeString(buffer);

    Log(7, "SDeskItemMenu::getItemDefinition done\n");
    return mItemDefinition;
}

