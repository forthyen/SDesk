#include <stdio.h>

#include "SDesk.h"
#include "SDeskTheme.h"

SDeskTheme::SDeskTheme(
    const char* fileName)
    : mTimerID(0),
      mDrawPopup(false),
      mNormalFontSize(32),
      mSmallFontSize(22),
      mSelectedFontSize(32),
      mTitleFontSize(36),
      mUserFontSize(32),
      mTextRows(8),
      mTextCellHeight(40),
      mTextCellWidth(0),
      mTextTopOffset(100),
      mTextLeftOffset(120),
      mTextRowOffset(50),
      mTitleCellHeight(50),
      mTitleCellWidth(0),
      mTitleTopOffset(20),
      mTitleLeftOffset(20),
      mUserCellHeight(20),
      mUserCellWidth(140),
      mUserTopOffset(20),
      mUserLeftOffset(20),
      mStatusMessageCellHeight(20),
      mStatusMessageCellWidth(140),
      mStatusMessageTopOffset(20),
      mStatusMessageLeftOffset(20),
      mLockTopOffset(20),
      mLockLeftOffset(20),
      mStatusTopOffset(20),
      mStatusLeftOffset(20),
      mStatusItemWidth(22),
      mImageXOffset(20),
      mImageYOffset(95),
      mImageWidth(300),
      mImageHeight(300),
      mDetailTextXOffset(600),
      mDetailTextYOffset(410),
      mDetailTextWidth(400),
      mDetailTextHeight(100),
      mScreenWidth(0),
      mScreenHeight(0),
      mNormR(255),
      mNormG(255),
      mNormB(255),
      mNormA(255),
      mSelR(255),
      mSelG(255),
      mSelB(255),
      mSelA(255),
      mTitleR(255),
      mTitleG(255),
      mTitleB(255),
      mTitleA(255),
      mUserR(255),
      mUserG(255),
      mUserB(255),
      mUserA(255)
{
    char buffer[1024];

    Log(7, "SDeskTheme()\n");

    CascadeScreen screen;
    SetRectAbsolute(screen.GetRect());

    // Determine our drawable area size.
    u32 nIndex = 0;

    // figure out what output resolution [index] the screen is set to.
    screen.GetCurrentOutputResolution(nIndex);
 
	//sets scaleRect with the output resolution scaling rectangle for
    // the output resolution identified by nIndex. For example, if the
    // output resolution is set to 720p (1280x720), the output
    // resolution scale rect by default will be CascadeRect(0, 0, 1280, 720)
    screen.GetOutputResolutionScaleRect(nIndex, mScaleRect);

    // Set initial sizes.
    mScreenHeight = screen.GetRect().h;
    mScreenWidth = screen.GetRect().w;
    mTextCellWidth = screen.GetRect().w - mImageWidth - 230;
    mTitleCellWidth = screen.GetRect().w - mUserCellWidth -
        (mStatusItemWidth * 4) - 40;
    
    // Setup the user name location
    mUserLeftOffset = mScreenWidth - mUserCellWidth - 28;

    // Setup the lock position
    mLockLeftOffset = mScreenWidth - mUserCellWidth - 55;

    // Setup the statusposition
    mStatusLeftOffset = mScreenWidth - mUserCellWidth - 155;
  
    // Setup the status message position
    mStatusMessageCellWidth = mScreenWidth - 40;
    mStatusMessageCellHeight = 20;
    mStatusMessageLeftOffset = 20;
    mStatusMessageTopOffset = mScreenHeight - mStatusMessageCellHeight - 5;
    strcpy(mStatusMessage, "");

    // Assign the theme file name
    strcpy(mThemeFile, fileName);

    // By default we are the default user
    strcpy(mUserName, "Default");

    // If we don't show type icons, move the text left.
    if (!SDesk::getInstance().getShowLittleIcons()) {
        mTextLeftOffset -= 40;
    }

    // Get the theme file.
    ReadThemeFile();

    // Get a shared memory zone to save the decoded image data in.
    SDeskBitmap::InitSharedMem(mImageWidth, mImageHeight);

    // Initialize the fonts.
    mNormalFont.Set(mNormalFontSize);
    mSmallFont.Set(mSmallFontSize);
    mSelectedFont.Set(mSelectedFontSize);
    mTitleFont.Set(mTitleFontSize);
    mUserFont.Set(mUserFontSize);

    // Initialize the colors.
    mNormalColor.SetRGBA(mNormR, mNormG, mNormB, mNormA);
    mSelectedColor.SetRGBA(mSelR, mSelG, mSelB, mSelA);
    mTitleColor.SetRGBA(mTitleR, mTitleG, mTitleB, mTitleA);
    mUserColor.SetRGBA(mUserR, mUserG, mUserB, mUserA);

    // Initialize the popup image.
    mPopup.LoadBitmap(
        "/usr/local/roku/graphics/photo/wait-progress.png");

    // Load the images
    // If we don't show type icons, move the text left.
    strcpy(buffer, mThemeFile);
    strcat(buffer, "_bg.png");
    Log(8, "SDeskTheme(%s) load\n", buffer);
    mBitmapBackground.Load(buffer);
    
    strcpy(buffer, mThemeFile);
    strcat(buffer, "_selected.png");
    Log(8, "SDeskTheme(%s) load\n", buffer);
    mSelectedBar.Load(buffer);
    
    strcpy(buffer, mThemeFile);
    strcat(buffer, "_selected_wide.png");
    Log(8, "SDeskTheme(%s) load\n", buffer);
    mSelectedBarWide.Load(buffer);

    strcpy(buffer, mThemeFile);
    strcat(buffer, "_protected.png");
    Log(8, "SDeskTheme(%s) load\n", buffer);
    mLocked.Load(buffer);

    strcpy(buffer, mThemeFile);
    strcat(buffer, "_statusok.png");
    Log(8, "SDeskTheme(%s) load\n", buffer);
    mStatusOK.Load(buffer);

    strcpy(buffer, mThemeFile);
    strcat(buffer, "_statuserror.png");
    Log(8, "SDeskTheme(%s) load\n", buffer);
    mStatusError.Load(buffer);

    strcpy(buffer, mThemeFile);
    strcat(buffer, "_warning.png");
    Log(8, "SDeskTheme(%s) load\n", buffer);
    mWarning.Load(buffer);

    strcpy(buffer, mThemeFile);
    strcat(buffer, "_statusunused.png");
    Log(8, "SDeskTheme(%s) load\n", buffer);
    mStatusUnused.Load(buffer);

    strcpy(buffer, mThemeFile);
    strcat(buffer, "_n.png");
    Log(8, "SDeskTheme(%s) load\n", buffer);
    mDirN.Load(buffer);
    
    strcpy(buffer, mThemeFile);
    strcat(buffer, "_w.png");
    Log(8, "SDeskTheme(%s) load\n", buffer);
    mDirW.Load(buffer);
    
    strcpy(buffer, mThemeFile);
    strcat(buffer, "_s.png");
    Log(8, "SDeskTheme(%s) load\n", buffer);
    mDirS.Load(buffer);
    
    strcpy(buffer, mThemeFile);
    strcat(buffer, "_e.png");
    Log(8, "SDeskTheme(%s) load\n", buffer);
    mDirE.Load(buffer);

    strcpy(buffer, mThemeFile);
    strcat(buffer, "_menu_big.png");
    Log(8, "SDeskTheme(%s) load\n", buffer);
    mMenuBig.Load(buffer);
    
    strcpy(buffer, mThemeFile);
    strcat(buffer, "_songs_big.png");
    Log(8, "SDeskTheme(%s) load\n", buffer);
    mSongsBig.Load(buffer);

    strcpy(buffer, mThemeFile);
    strcat(buffer, "_song_big.png");
    Log(8, "SDeskTheme(%s) load\n", buffer);
    mSongBig.Load(buffer);
    
    strcpy(buffer, mThemeFile);
    strcat(buffer, "_pictures_big.png");
    Log(8, "SDeskTheme(%s) load\n", buffer);
    mPicturesBig.Load(buffer);
    
    strcpy(buffer, mThemeFile);
    strcat(buffer, "_picture_big.png");
    Log(8, "SDeskTheme(%s) load\n", buffer);
    mPictureBig.Load(buffer);
    
    strcpy(buffer, mThemeFile);
    strcat(buffer, "_movies_big.png");
    Log(8, "SDeskTheme(%s) load\n", buffer);
    mMoviesBig.Load(buffer);
    
    strcpy(buffer, mThemeFile);
    strcat(buffer, "_movie_big.png");
    Log(8, "SDeskTheme(%s) load\n", buffer);
    mMovieBig.Load(buffer);
    
    strcpy(buffer, mThemeFile);
    strcat(buffer, "_dvd_big.png");
    Log(8, "SDeskTheme(%s) load\n", buffer);
    mDVDBig.Load(buffer);
    
    strcpy(buffer, mThemeFile);
    strcat(buffer, "_radios_big.png");
    Log(8, "SDeskTheme(%s) load\n", buffer);
    mRadiosBig.Load(buffer);
    
    strcpy(buffer, mThemeFile);
    strcat(buffer, "_radio_big.png");
    Log(8, "SDeskTheme(%s) load\n", buffer);
    mRadioBig.Load(buffer);
    
    strcpy(buffer, mThemeFile);
    strcat(buffer, "_apps_big.png");
    Log(8, "SDeskTheme(%s) load\n", buffer);
    mAppsBig.Load(buffer);
    
    strcpy(buffer, mThemeFile);
    strcat(buffer, "_app_big.png");
    Log(8, "SDeskTheme(%s) load\n", buffer);
    mAppBig.Load(buffer);
    
    strcpy(buffer, mThemeFile);
    strcat(buffer, "_favorites_big.png");
    Log(8, "SDeskTheme(%s) load\n", buffer);
    mFavoritesBig.Load(buffer);
    
    strcpy(buffer, mThemeFile);
    strcat(buffer, "_news_feed_big.png");
    Log(8, "SDeskTheme(%s) load\n", buffer);
    mNewsFeedBig.Load(buffer);
    
    strcpy(buffer, mThemeFile);
    strcat(buffer, "_news_big.png");
    Log(8, "SDeskTheme(%s) load\n", buffer);
    mNewsBig.Load(buffer);
        
    if (SDesk::getInstance().getShowLittleIcons()) {
        strcpy(buffer, mThemeFile);
        strcat(buffer, "_menu.png");
        Log(8, "SDeskTheme(%s) load\n", buffer);
        mMenu.Load(buffer);
        
        strcpy(buffer, mThemeFile);
        strcat(buffer, "_songs.png");
        Log(8, "SDeskTheme(%s) load\n", buffer);
        mSongs.Load(buffer);
        
        strcpy(buffer, mThemeFile);
        strcat(buffer, "_song.png");
        Log(8, "SDeskTheme(%s) load\n", buffer);
        mSong.Load(buffer);
        
        strcpy(buffer, mThemeFile);
        strcat(buffer, "_pictures.png");
        Log(8, "SDeskTheme(%s) load\n", buffer);
        mPictures.Load(buffer);
        
        strcpy(buffer, mThemeFile);
        strcat(buffer, "_picture.png");
        Log(8, "SDeskTheme(%s) load\n", buffer);
        mPicture.Load(buffer);
        
        strcpy(buffer, mThemeFile);
        strcat(buffer, "_movies.png");
        Log(8, "SDeskTheme(%s) load\n", buffer);
        mMovies.Load(buffer);
        
        strcpy(buffer, mThemeFile);
        strcat(buffer, "_movie.png");
        Log(8, "SDeskTheme(%s) load\n", buffer);
        mMovie.Load(buffer);
        
        strcpy(buffer, mThemeFile);
        strcat(buffer, "_dvd.png");
        Log(8, "SDeskTheme(%s) load\n", buffer);
        mDVD.Load(buffer);
        
        strcpy(buffer, mThemeFile);
        strcat(buffer, "_radios.png");
        Log(8, "SDeskTheme(%s) load\n", buffer);
        mRadios.Load(buffer);
        
        strcpy(buffer, mThemeFile);
        strcat(buffer, "_radio.png");
        Log(8, "SDeskTheme(%s) load\n", buffer);
        mRadio.Load(buffer);
        
        strcpy(buffer, mThemeFile);
        strcat(buffer, "_apps.png");
        Log(8, "SDeskTheme(%s) load\n", buffer);
        mApps.Load(buffer);
        
        strcpy(buffer, mThemeFile);
        strcat(buffer, "_app.png");
        Log(8, "SDeskTheme(%s) load\n", buffer);
        mApp.Load(buffer);
        
        strcpy(buffer, mThemeFile);
        strcat(buffer, "_favorites.png");
        Log(8, "SDeskTheme(%s) load\n", buffer);
        mFavorites.Load(buffer);
        
        strcpy(buffer, mThemeFile);
        strcat(buffer, "_news_feed.png");
        Log(8, "SDeskTheme(%s) load\n", buffer);
        mNewsFeed.Load(buffer);
        
        strcpy(buffer, mThemeFile);
        strcat(buffer, "_news.png");
        Log(8, "SDeskTheme(%s) load\n", buffer);
        mNews.Load(buffer);
    }
        
    Materialize();

    Log(7, "SDeskTheme() done\n");
}

SDeskTheme::~SDeskTheme()
{
    Log(7, "~SDeskTheme()\n");

    Log(7, "~SDeskTheme() done\n");
}

void SDeskTheme::OnPaint(
    CascadeBitmap& bitmap)
{
    Log(7, "SDeskTheme::OnPaint()\n");

    // Draw the background
    mBitmapBackground.Draw(bitmap);

    if (mDrawPopup) {
        // Just show the popup
        mPopup.Draw(bitmap);
    } else {
        // Show the user name, but only if we are not the default user.
        if (strcmp(mUserName, "Default") != 0) {
            CascadeRect r1 = CascadeRect(mUserLeftOffset, mUserTopOffset,
                                         mUserCellWidth, mUserCellHeight);
            bitmap.DrawText(mUserFont, r1, mUserName, mUserColor,
                            TEXT_CENTER_VERTICALLY | TEXT_JUSTIFY_RIGHT);
        }

        // Show the status icons
        if (SDesk::getInstance().getStatus1Int() == 0) {
            mStatusUnused.Draw(bitmap, mStatusLeftOffset,
                               mStatusTopOffset);
        } else {
            if (SDesk::getInstance().getStatus1() == 0) {
                mStatusOK.Draw(bitmap, mStatusLeftOffset,
                               mStatusTopOffset);
            } else {
                mStatusError.Draw(bitmap, mStatusLeftOffset,
                                  mStatusTopOffset);
            }
        }
        if (SDesk::getInstance().getStatus2Int() == 0) {
            mStatusUnused.Draw(bitmap, mStatusLeftOffset + 
                               mStatusItemWidth, mStatusTopOffset,
                               mStatusTopOffset);
        } else {
            if (SDesk::getInstance().getStatus2() == 0) {
                mStatusOK.Draw(bitmap, mStatusLeftOffset + 
                               mStatusItemWidth, mStatusTopOffset);
            } else {
                mStatusError.Draw(bitmap, mStatusLeftOffset +
                                  mStatusItemWidth, mStatusTopOffset);
            }
        }
        if (SDesk::getInstance().getStatus3Int() == 0) {
            mStatusUnused.Draw(bitmap, mStatusLeftOffset +
                               (mStatusItemWidth * 2),
                               mStatusTopOffset);
        } else {
            if (SDesk::getInstance().getStatus3() == 0) {
                mStatusOK.Draw(bitmap, mStatusLeftOffset +
                               (mStatusItemWidth * 2),
                               mStatusTopOffset);
            } else {
                mStatusError.Draw(bitmap, mStatusLeftOffset +
                                  (mStatusItemWidth * 2),
                                  mStatusTopOffset);
            }
        }
        if (SDesk::getInstance().getStatus4Int() == 0) {
            mStatusUnused.Draw(bitmap, mStatusLeftOffset +
                               (mStatusItemWidth * 3),
                               mStatusTopOffset);
        } else {
            if (SDesk::getInstance().getStatus4() == 0) {
                mStatusOK.Draw(bitmap, mStatusLeftOffset +
                               (mStatusItemWidth * 3),
                               mStatusTopOffset);
            } else {
                mStatusError.Draw(bitmap, mStatusLeftOffset +
                                  (mStatusItemWidth * 3),
                                  mStatusTopOffset);
            }
        }
        
        // Show when we are in protected mode
        if (SDesk::getInstance().getProtected()) {
            mLocked.Draw(bitmap, mLockLeftOffset, mLockTopOffset);
        }

        // Show menu content
        SDesk::getInstance().getCurrentMenu().Draw(bitmap);

        // Draw the status bar
        DrawStatusBar(bitmap);
    }

    Log(7, "SDeskTheme::OnPaint() done\n");
}

void SDeskTheme::OnVanish()
{
    Log(7, "SDeskTheme::OnVanish()\n");

    mStatusCounter = 0;
	KillTimer(mTimerID);
    CascadeApp::GetApp()->Terminate(0);

    Log(7, "SDeskTheme::OnVanish() done\n");
}

void SDeskTheme::OnMaterialize()
{
    Log(7, "SDeskTheme::OnMaterialize()\n");

    // Create the timer handle.
    mTimerID = SetTimer(10000);

    Log(7, "SDeskTheme::OnMaterialize() done\n");
}

void SDeskTheme::OnTimer(
    u32 nTimerID)
{
//    Log(7, "SDeskTheme::OnTimer()\n");

    if (SDesk::getInstance().getScanType()[0] != '\0') {
        Log(7, "SDeskTheme::OnTimer() rescan\n");
        SDesk::getInstance().RescanApp();

        Log(7, "SDeskTheme::OnTimer() setup\n");
        SDesk::getInstance().SetupApp();
    }

    // Redraw the status line
    if (mStatusCounter > 0) {
        Redraw();
    }

//    Log(7, "SDeskTheme::OnTimer() done\n");
}

void SDeskTheme::ShowPopup(
    const char *message)
{
    Log(7, "SDeskTheme::ShowPopup()\n");

    mPopup.SetString(message);
    mDrawPopup = true;
    Redraw();
    Materialize();

    Log(7, "SDeskTheme::ShowPopup() done\n");
}

void SDeskTheme::HidePopup()
{
    Log(7, "SDeskTheme::HidePopup()\n");

    mDrawPopup = false;
    Redraw();
    Log(7, "SDeskTheme::HidePopup() done\n");
}

void SDeskTheme::DrawStatusBar(
    CascadeBitmap& bitmap)
{
    Log(7, "SDeskTheme::DrawStatusBar(%d)\n", mStatusCounter);

    if (mStatusCounter <= 0) {
        return;
    }

    CascadeRect r1 = CascadeRect(mStatusMessageLeftOffset, 
                                 mStatusMessageTopOffset,
                                 mStatusMessageCellWidth, 
                                 mStatusMessageCellHeight);
    bitmap.DrawText(mUserFont, r1, mStatusMessage, mUserColor,
                    TEXT_CENTER_VERTICALLY | TEXT_CENTER_HORIZONTALLY |
                    TEXT_CLIP_WITH_DOTDOTDOT);
    mStatusCounter--;

    Log(7, "SDeskTheme::DrawStatusBar() done\n");
}

void SDeskTheme::DrawSelected(
    CascadeBitmap& bitmap,
    int x,
    int y,
    bool north,
    bool south,
    bool west,
    bool east,
    bool wide)
{
    Log(3, "SDeskTheme::DrawSelected()\n");

    if (!SDesk::getInstance().getShowLittleIcons()) {
        if (wide) {
            mSelectedBarWide.Draw(bitmap, x - 50, y - 6);
        } else {
            mSelectedBar.Draw(bitmap, x - 50, y - 6);
        }

        if (north) {
            mDirN.Draw(bitmap, x - 40, y - 20);
        }
        if (south) {
            mDirS.Draw(bitmap, x - 40, y + 45);
        }
        if (west) {
            mDirW.Draw(bitmap, x - 68, y + 13);
        }
        if (east) {
            mDirE.Draw(bitmap, x - 20, y + 13);
        }
    } else {
        if (wide) {
            mSelectedBarWide.Draw(bitmap, x - 90, y - 6);
        } else {
            mSelectedBar.Draw(bitmap, x - 90, y - 6);
        }

        if (north) {
            mDirN.Draw(bitmap, x - 80, y - 20);
        }
        if (south) {
            mDirS.Draw(bitmap, x - 80, y + 45);
        }
        if (west) {
            mDirW.Draw(bitmap, x - 108, y + 13);
        }
        if (east) {
            mDirE.Draw(bitmap, x - 60, y + 13);
        }
    }

    Log(3, "SDeskTheme::DrawSelected() done\n");
}

void SDeskTheme::DrawType(
    CascadeBitmap& bitmap,
    int type,
    int menuType,
    int x,
    int y)
{
    Log(7, "SDeskTheme::DrawType(%d, %d)\n", type, menuType);

    if (x == 0 && y == 0) {
        // Aha... we are drawing the "big" type.
        int x0, y0;

        x0 = SDesk::getInstance().getCurrentTheme().getScreenWidth()
            - mSongsBig.GetWidth()
            - SDesk::getInstance().getCurrentTheme().getImageXOffset();
        y0 = SDesk::getInstance().getCurrentTheme().getImageYOffset();
            
        Log(7, "SDeskTheme::DrawType(%ld, %ld) default type image\n",
            x0, y0);

        if (type == I_MENU) {
            if (menuType == M_PICTURES) {
                mPicturesBig.Draw(bitmap, x0, y0);
            } else if (menuType == M_SONGS) {
                mSongsBig.Draw(bitmap, x0, y0);
            } else if (menuType == M_MOVIES) {
                mMoviesBig.Draw(bitmap, x0, y0);
            } else if (menuType == M_RADIOS) {
                mRadiosBig.Draw(bitmap, x0, y0);
            } else if (menuType == M_APPS) {
                mAppsBig.Draw(bitmap, x0, y0);
            } else if (menuType == M_FAVORITES) {
                mFavoritesBig.Draw(bitmap, x0, y0);
            } else if (menuType == M_MESSAGE) {
                mWarningBig.Draw(bitmap, x0, y0);
            } else if (menuType == M_RSS_FEED) {
                mNewsFeedBig.Draw(bitmap, x0, y0);
            } else {
                mMenuBig.Draw(bitmap, x0, y0);
            }
        } else if (type == I_PICTURE) {
            mPictureBig.Draw(bitmap, x0, y0);
        } else if (type == I_MESSAGE) {
            mWarningBig.Draw(bitmap, x0, y0);
        } else if (type == I_SONG) {
            mSongBig.Draw(bitmap, x0, y0);
        } else if (type == I_RADIO) {
            mRadioBig.Draw(bitmap, x0, y0);
        } else if (type == I_MOVIE) {
            mMovieBig.Draw(bitmap, x0, y0);
        } else if (type == I_DVD) {
            mDVDBig.Draw(bitmap, x0, y0);
        } else if (type == I_RSS) {
            mNewsBig.Draw(bitmap, x0, y0);
        } else if (type == I_RSS_FEED) {
            mNewsFeedBig.Draw(bitmap, x0, y0);
        } else {
            mAppBig.Draw(bitmap, x0, y0);
        }
    } else {
        Log(7, "SDeskTheme::DrawType() small default type image\n");
        if (type == I_MENU) {
            if (menuType == M_PICTURES) {
                mPictures.Draw(bitmap, x - 45, y + 2);
            } else if (menuType == M_SONGS) {
                mSongs.Draw(bitmap, x - 45, y + 2);
            } else if (menuType == M_MOVIES) {
                mMovies.Draw(bitmap, x - 45, y + 2);
            } else if (menuType == M_RADIOS) {
                mRadios.Draw(bitmap, x - 45, y + 2);
            } else if (menuType == M_APPS) {
                mApps.Draw(bitmap, x - 45, y + 2);
            } else if (menuType == M_FAVORITES) {
                mFavorites.Draw(bitmap, x - 45, y + 2);
            } else if (menuType == M_MESSAGE) {
                mWarning.Draw(bitmap, x - 45, y + 2);
            } else if (menuType == M_RSS_FEED) {
                mNewsFeed.Draw(bitmap, x - 45, y + 2);
            } else {
                mMenu.Draw(bitmap, x - 45, y + 2);
            }
        } else if (type == I_PICTURE) {
            mPicture.Draw(bitmap, x - 45, y + 2);
        } else if (type == I_MESSAGE) {
            mWarning.Draw(bitmap, x - 45, y + 2);
        } else if (type == I_SONG) {
            mSong.Draw(bitmap, x - 45, y + 2);
        } else if (type == I_RADIO) {
            mRadio.Draw(bitmap, x - 45, y + 2);
        } else if (type == I_MOVIE) {
            mMovie.Draw(bitmap, x - 45, y + 2);
        } else if (type == I_DVD) {
            mDVD.Draw(bitmap, x - 45, y + 2);
        } else if (type == I_RSS) {
            mNews.Draw(bitmap, x - 45, y + 2);
        } else if (type == I_RSS_FEED) {
            mNewsFeed.Draw(bitmap, x - 45, y + 2);
        } else {
            mApp.Draw(bitmap, x - 45, y + 2);
        }
    }

    Log(3, "SDeskTheme::DrawType() done\n");
}

void SDeskTheme::SetStatusMessage(
    const char *message)
{
    strcpy(mStatusMessage, message);
    mStatusCounter = 1;
}

void SDeskTheme::ReadThemeFile()
{
    FILE *fd;
    char *pos;
    char buf[1024];
        
    if ((fd = fopen(mThemeFile, "r")) == NULL) {
        Log(8, "fopen failed (%s)...\n", mThemeFile);
        return;
    }
    
    while ((pos = fgets(buf, 1023, fd)) != 0) {
        if (strcmp(buf, "textRows\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mTextRows = atol(buf);
            }
        } else if (strcmp(buf, "textCellHeight\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mTextCellHeight = atol(buf);
            }
        } else if (strcmp(buf, "textCellWidth\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mTextCellWidth = atol(buf);
            }
        } else if (strcmp(buf, "textTopOffset\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mTextTopOffset = atol(buf);
            }
        } else if (strcmp(buf, "textLeftOffset\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mTextLeftOffset = atol(buf);
            }
        } else if (strcmp(buf, "textRowOffset\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mTextRowOffset = atol(buf);
            }
        } else if (strcmp(buf, "titleCellHeight\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mTitleCellHeight = atol(buf);
            }
        } else if (strcmp(buf, "titleCellWidth\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mTitleCellWidth = atol(buf);
            }
        } else if (strcmp(buf, "titleTopOffset\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mTitleTopOffset = atol(buf);
            }
        } else if (strcmp(buf, "titleLeftOffset\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mTitleLeftOffset = atol(buf);
            }
        } else if (strcmp(buf, "userCellHeight\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mUserCellHeight = atol(buf);
            }
        } else if (strcmp(buf, "userCellWidth\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mUserCellWidth = atol(buf);
            }
        } else if (strcmp(buf, "userTopOffset\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mUserTopOffset = atol(buf);
            }
        } else if (strcmp(buf, "userLeftOffset\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mUserLeftOffset = atol(buf);
            }
        } else if (strcmp(buf, "statusMessageCellHeight\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mStatusMessageCellHeight = atol(buf);
            }
        } else if (strcmp(buf, "statusMessageCellWidth\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mStatusMessageCellWidth = atol(buf);
            }
        } else if (strcmp(buf, "statusMessageTopOffset\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mStatusMessageTopOffset = atol(buf);
            }
        } else if (strcmp(buf, "statusMessageLeftOffset\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mStatusMessageLeftOffset = atol(buf);
            }
        } else if (strcmp(buf, "lockTopOffset\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mLockTopOffset = atol(buf);
            }
        } else if (strcmp(buf, "lockLeftOffset\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mLockLeftOffset = atol(buf);
            }
        } else if (strcmp(buf, "statusTopOffset\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mStatusTopOffset = atol(buf);
            }
        } else if (strcmp(buf, "statusLeftOffset\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mStatusLeftOffset = atol(buf);
            }
        } else if (strcmp(buf, "statusItemWidth\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mStatusItemWidth = atol(buf);
            }
        } else if (strcmp(buf, "normalFontSize\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mNormalFontSize = atol(buf);
            }
        } else if (strcmp(buf, "smallFontSize\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mSmallFontSize = atol(buf);
            }
        } else if (strcmp(buf, "selectedFontSize\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mSelectedFontSize = atol(buf);
            }
        } else if (strcmp(buf, "titleFontSize\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mTitleFontSize = atol(buf);
            }
        } else if (strcmp(buf, "userFontSize\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mUserFontSize = atol(buf);
            }
        } else if (strcmp(buf, "screenWidth\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mScreenWidth = atol(buf);
            }
        } else if (strcmp(buf, "screenHeight\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mScreenHeight = atol(buf);
            }
        } else if (strcmp(buf, "imageXOffset\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mImageXOffset = atol(buf);
            }
        } else if (strcmp(buf, "imageYOffset\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mImageYOffset = atol(buf);
            }
        } else if (strcmp(buf, "imageWidth\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mImageWidth = atol(buf);
            }
        } else if (strcmp(buf, "imageHeight\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mImageHeight = atol(buf);
            }
        } else if (strcmp(buf, "detailTextXOffset\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mDetailTextXOffset = atol(buf);
            }
        } else if (strcmp(buf, "detailTextYOffset\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mDetailTextYOffset = atol(buf);
            }
        } else if (strcmp(buf, "detailTextWidth\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mDetailTextWidth = atol(buf);
            }
        } else if (strcmp(buf, "detailTextHeight\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mDetailTextHeight = atol(buf);
            }
        } else if (strcmp(buf, "normR\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mNormR = atol(buf);
            }
        } else if (strcmp(buf, "normG\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mNormG = atol(buf);
            }
        } else if (strcmp(buf, "normB\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mNormB = atol(buf);
            }
        } else if (strcmp(buf, "normA\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mNormA = atol(buf);
            }
        } else if (strcmp(buf, "selR\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mSelR = atol(buf);
            }
        } else if (strcmp(buf, "selG\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mSelG = atol(buf);
            }
        } else if (strcmp(buf, "selB\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mSelB = atol(buf);
            }
        } else if (strcmp(buf, "selA\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mSelA = atol(buf);
            }
        } else if (strcmp(buf, "titleR\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mTitleR = atol(buf);
            }
        } else if (strcmp(buf, "titleG\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mTitleG = atol(buf);
            }
        } else if (strcmp(buf, "titleB\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mTitleB = atol(buf);
            }
        } else if (strcmp(buf, "titleA\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mTitleA = atol(buf);
            }
        } else if (strcmp(buf, "userR\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mUserR = atol(buf);
            }
        } else if (strcmp(buf, "userG\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mUserG = atol(buf);
            }
        } else if (strcmp(buf, "userB\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mUserB = atol(buf);
            }
        } else if (strcmp(buf, "userA\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mUserA = atol(buf);
            }
        }
    }
    fclose(fd);
}

