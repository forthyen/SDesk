#ifndef _sdesk_theme_h
#define _sdesk_theme_h

#include <cascade/Cascade.h>

#include "SDeskBitmap.h"
#include "SDeskFont.h"
#include "SDeskLog.h"
#include "SDeskPopup.h"

class SDeskTheme : public CascadeWindow, CascadeTimer, SDeskLog
{
public:
    SDeskTheme(const char* fileName);
    virtual ~SDeskTheme();

    virtual void OnPaint(CascadeBitmap& bitmap);    
    virtual void OnVanish();
    virtual void OnMaterialize();
    virtual void OnTimer(u32 nTimerID);

    void ShowPopup(const char *message);
    void HidePopup();
    void DrawStatusBar(CascadeBitmap& bitmap);
    void DrawSelected(CascadeBitmap& bitmap, int x, int y, 
                      bool north, bool south, bool west, bool east, bool wide);
    void DrawType(CascadeBitmap& bitmap, int type, int menuType,
                  int x, int y);
    void SetStatusMessage(const char *message);

    SDeskFont& getNormalFont() {
        return mNormalFont;
    }
    SDeskFont& getSmallFont() {
        return mSmallFont;
    }
    SDeskFont& getSelectedFont() {
        return mSelectedFont;
    }
    SDeskFont& getTitleFont() {
        return mTitleFont;
    }
    CascadeColor& getNormalColor() {
        return mNormalColor;
    }
    CascadeColor& getSelectedColor() {
        return mSelectedColor;
    }
    CascadeColor& getTitleColor() {
        return mTitleColor;
    }
    int getTextRows() {
        return mTextRows;
    }
    int getTextCellHeight() {
        return mTextCellHeight;
    }
    int getTextCellWidth() {
        return mTextCellWidth;
    }
    int getTextTopOffset() {
        return mTextTopOffset;
    }
    int getTextLeftOffset() {
        return mTextLeftOffset;
    }
    int getTextRowOffset() {
        return mTextRowOffset;
    }
    int getTitleCellHeight() {
        return mTitleCellHeight;
    }
    int getTitleCellWidth() {
        return mTitleCellWidth;
    }
    int getTitleTopOffset() {
        return mTitleTopOffset;
    }
    int getTitleLeftOffset() {
        return mTitleLeftOffset;
    }
    int getNormalFontSize() {
        return mNormalFontSize;
    }
    int getSmallFontSize() {
        return mSmallFontSize;
    }
    int getSelectedFontSize() {
        return mSelectedFontSize;
    }
    int getTitleFontSize() {
        return mTitleFontSize;
    }
    int getImageXOffset() {
        return mImageXOffset;
    }
    int getImageYOffset() {
        return mImageYOffset;
    }
    u32 getImageWidth() {
        return mImageWidth;
    }
    u32 getImageHeight() {
        return mImageHeight;
    }
    int getDetailTextXOffset() {
        return mDetailTextXOffset;
    }
    int getDetailTextYOffset() {
        return mDetailTextYOffset;
    }
    u32 getDetailTextWidth() {
        return mDetailTextWidth;
    }
    u32 getDetailTextHeight() {
        return mDetailTextHeight;
    }
    int getScreenHeight() {
        return mScreenHeight;
    }
    int getScreenWidth() {
        return mScreenWidth;
    }
    void setUserName(const char* name) {
        strcpy(mUserName, name);
    }
    char *getUserName() {
        return mUserName;
    }

private:
    void ReadThemeFile();

    // Timer handle.
    u32 mTimerID;

    CascadeRect mScaleRect;

    char mThemeFile[1024];

    char mUserName[100];
           
    char mStatusMessage[500];
                                     
    bool mDrawPopup;

    SDeskPopup mPopup;

    SDeskFont mNormalFont;
    SDeskFont mSmallFont;
    SDeskFont mSelectedFont;
    SDeskFont mTitleFont;
    SDeskFont mUserFont;

    int mNormalFontSize;
    int mSmallFontSize;
    int mSelectedFontSize;
    int mTitleFontSize;
    int mUserFontSize;
    CascadeColor mNormalColor;
    CascadeColor mSelectedColor;
    CascadeColor mTitleColor;
    CascadeColor mUserColor;

    SDeskBitmap mBitmapBackground;
    SDeskBitmap mSelectedBar;
    SDeskBitmap mSelectedBarWide;
    SDeskBitmap mLocked;
    SDeskBitmap mStatusOK;
    SDeskBitmap mStatusError;
    SDeskBitmap mStatusUnused;
    SDeskBitmap mDirE;
    SDeskBitmap mDirW;
    SDeskBitmap mDirN;
    SDeskBitmap mDirS;

    SDeskBitmap mFavorites;
    SDeskBitmap mSongs;
    SDeskBitmap mSong;
    SDeskBitmap mPictures;
    SDeskBitmap mPicture;
    SDeskBitmap mMovies;
    SDeskBitmap mMovie;
    SDeskBitmap mDVD;
    SDeskBitmap mRadios;
    SDeskBitmap mRadio;
    SDeskBitmap mApps;
    SDeskBitmap mApp;
    SDeskBitmap mMenu;
    SDeskBitmap mNews;
    SDeskBitmap mNewsFeed;
    SDeskBitmap mWarning;
    SDeskBitmap mFavoritesBig;
    SDeskBitmap mSongsBig;
    SDeskBitmap mSongBig;
    SDeskBitmap mPicturesBig;
    SDeskBitmap mPictureBig;
    SDeskBitmap mMoviesBig;
    SDeskBitmap mMovieBig;
    SDeskBitmap mDVDBig;
    SDeskBitmap mRadiosBig;
    SDeskBitmap mRadioBig;
    SDeskBitmap mAppsBig;
    SDeskBitmap mAppBig;
    SDeskBitmap mMenuBig;
    SDeskBitmap mNewsBig;
    SDeskBitmap mNewsFeedBig;
    SDeskBitmap mWarningBig;

    int mTextRows;
    int mTextCellHeight;
    int mTextCellWidth;
    int mTextTopOffset;
    int mTextLeftOffset;
    int mTextRowOffset;

    int mTitleCellHeight;
    int mTitleCellWidth;
    int mTitleTopOffset;
    int mTitleLeftOffset;

    int mUserCellHeight;
    int mUserCellWidth;
    int mUserTopOffset;
    int mUserLeftOffset;

    int mStatusMessageCellHeight;
    int mStatusMessageCellWidth;
    int mStatusMessageTopOffset;
    int mStatusMessageLeftOffset;

    // Lock icon position
    int mLockTopOffset;
    int mLockLeftOffset;

    // Status position
    int mStatusTopOffset;
    int mStatusLeftOffset;
    int mStatusItemWidth;

    int mImageXOffset;
    int mImageYOffset;
    u32 mImageWidth;
    u32 mImageHeight;

    int mDetailTextXOffset;
    int mDetailTextYOffset;
    u32 mDetailTextWidth;
    u32 mDetailTextHeight;

    int mScreenWidth;
    int mScreenHeight;

    // The status counter (countdown for the # secs the message
    // stays up.
    int mStatusCounter;

    // Color values
    int mNormR;
    int mNormG;
    int mNormB;
    int mNormA;
    int mSelR;
    int mSelG;
    int mSelB;
    int mSelA;
    int mTitleR;
    int mTitleG;
    int mTitleB;
    int mTitleA;
    int mUserR;
    int mUserG;
    int mUserB;
    int mUserA;
};

#endif
