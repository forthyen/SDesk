
#include <string>
#include <vector>

using namespace std;

#include <cascade/Cascade.h>
#include <cascade/av/CascadeHTTPStream.h> 
#include <cascade/av/CascadeAudioPlayer.h> 

#include "SDeskTheme.h"
#include "SDeskItemMenu.h"
#include "SDeskItemMessage.h"
#include "SDeskFavorite.h"
#include "SDeskSetup.h"
#include "SDeskLog.h"
#include "SDeskWorker.h"

class SDesk : public CascadeApp, SDeskLog
{
public:
    SDesk();
    ~SDesk();

    static SDesk& getInstance();

    // Various setter/getter functions.
    void setConfigFile(const char *fileName) {
        strcpy(mConfigFile, fileName);
    }
    void setMainMenu(SDeskItemMenu* menu) {
        mMainMenu = menu;
    }
    SDeskItemMenu& getMainMenu() {
        return *mMainMenu;
    }
    void setCurrentMenu(SDeskItemMenu* menu) {
        mCurrentMenu = menu;
    }
    SDeskItemMenu& getCurrentMenu() {
        return *mCurrentMenu;
    }
    void setCurrentTheme(SDeskTheme* theme) {
        mCurrentTheme = theme;
    }
    SDeskTheme& getCurrentTheme() {
        return *mCurrentTheme;
    }
    char *getRootDir() {
        return mRootDir;
    }
    char *getMPlay() {
        return mMPlay;
    }
    char *getRadioPlayer() {
        return mRadioPlayer;
    }
    char *getMusicPlayer() {
        return mMusicPlayer;
    }
    char *getPictureViewer() {
        return mPictureViewer;
    }
    char *getPlaymusic() {
        return mPlaymusic;
    }
    char *getPlaysong() {
        return mPlaysong;
    }
    char *getPlayradio() {
        return mPlayradio;
    }
    char *getPlaymovie() {
        return mPlaymovie;
    }
    char *getShowpicture() {
        return mShowpicture;
    }
    char *getSlideshow() {
        return mSlideshow;
    }
    char *getImagesDir() {
        return mImagesDir;
    }
    char *getTmpDir() {
        return mTmpDir;
    }
    vector<SDeskFavorite *>& getFavorites() {
        return mFavorites;
    }
    void setProtected(bool flag) {
        mProtected = flag;
    }
    bool getShowLittleIcons() {
        return mShowLittleIcons;
    }
    bool getJustSDTHM() {
        return mJustSDTHM;
    }
    bool getProtected() {
        if (mNoSecurity) {
            return false;
        }
        return mProtected;
    }
    void setExitIsUp(bool flag) {
        mExitIsUp = flag;
    }
    bool getExitIsUp() {
        return mExitIsUp;
    }
    bool getShowThumb() {
        return mShowThumb;
    }
    void setNoExit(bool flag) {
        mNoExit = flag;
    }
    bool getNoExit() {
        return mNoExit;
    }
    bool isNoSecurity() {
        return mNoSecurity;
    }
    void setAskPassword(const char* password, const char* user,
        const char* configFile) {
        mAskPassword = true;
        strcpy(mNeedPassword, password);
        strcpy(mNewUserName, user);
        strcpy(mNewConfigFile, configFile);

        for (int i = 0; i < 100; i++) {
            mUserPassword[i] = '\0';
        }
    }

    void setSetupApp(bool value) {
        mSetupApp = value;
    }
    bool getSetupApp() {
        return mSetupApp;
    }
    void setScanType(const char *value) {
        mScanType[0] = '\0';
        strcpy(mScanType, value);
    }
    const char* getScanType() {
        return mScanType;
    }

    void setStatus1(int value) {
        mStatus1 = value;
    }
    int getStatus1() {
        return mStatus1;
    }
    int getStatus1Int() {
        return mStatus1Int;
    }
    void setStatus2(int value) {
        mStatus2 = value;
    }
    int getStatus2() {
        return mStatus2;
    }
    int getStatus2Int() {
        return mStatus3Int;
    }
    void setStatus3(int value) {
        mStatus3 = value;
    }
    int getStatus3() {
        return mStatus3;
    }
    int getStatus3Int() {
        return mStatus3Int;
    }
    void setStatus4(int value) {
        mStatus4 = value;
    }
    int getStatus4() {
        return mStatus4;
    }
    int getStatus4Int() {
        return mStatus4Int;
    }
    bool getExitApp() {
        return mExitApp;
    }

    void RescanApp();
    void SetupApp();
    void RunStatus();

    bool IsExcluded(const char* name);
    bool IsPicture(const char* name, unsigned int *extPos);
    bool IsMusic(const char* name, unsigned int *extPos);
    bool IsVideo(const char* name, unsigned int *extPos);

    bool HaveMessage();
    void CleanMessages();
    void AddMessage(string* value);
    void DrawMessage(CascadeBitmap& bitmap);

protected:
    // Methods implelemting this applications featues.
    virtual void OnAppInit();
    virtual void OnAppExit();
    virtual void OnKeyDown(u32 nKey);
    virtual void OnMountMessage(const CascadeMountMessage& mountMessage);
    virtual void OnActivate();

    void ReadConfigFile();

    // The start directory for the app.
    char mRootDir[512];

    // The start menu file for the app.
    char mRootMenu[1024];

    // The main config file.
    char mConfigFile[1024];

    // The directory with additional directories.
    char mImagesDir[1024];

    // The start menu file for the app.
    char mThemeFile[1024];

    // The path to the mplay executable.
    char mMPlay[1024];

    // The path to the radio player executable.
    char mRadioPlayer[1024];

    // The path to the music player executable.
    char mMusicPlayer[1024];

    // The path to the picture viewer executable.
    char mPictureViewer[1024];

    // The path to the slideshow executable.
    char mSlideshow[1024];

    // The path to the playmusic executable.
    char mPlaymusic[1024];

    // The path to the playsong executable.
    char mPlaysong[1024];

    // The path to the showpicture executable.
    char mShowpicture[1024];

    // The path to the playradio executable.
    char mPlayradio[1024];

    // The path to the playradio executable.
    char mPlaymovie[1024];

    // The path to the custom info script
    char mCustomInfo[1024];

    // The path to the custom rotate script
    char mCustomRotate[1024];

    // The path to the custom fast forward script
    char mCustomFF[1024];

    // The path to the custom fast backward script
    char mCustomFB[1024];

    // The tmp directory
    char mTmpDir[1024];

    // The entered password
    char mUserPassword[100];

    // The required password
    char mNeedPassword[100];

    // The new user name
    char mNewUserName[100];

    // The new configuration file
    char mNewConfigFile[1024];

    // Just try to read SDTHM files.
    bool mJustSDTHM;

    // Determines the protection status for the system.
    bool mProtected;

    // Determines the way the exit button is handled (up or exit).
    bool mExitIsUp;

    // Determines whether exit can exit the app.
    bool mNoExit;

    // Flag to indicat that we want to completely restat the setup
    bool mSetupApp;

    // Flag to indicat that we are asking for a password
    bool mAskPassword;

    // Flag indicating that we need to do a setup scan
    char mScanType[100];

    // The main menu object, as well we the current menu object.
    SDeskItemMenu *mMainMenu;
    SDeskItemMenu *mCurrentMenu;

    // The current theme object.
    SDeskTheme *mCurrentTheme;

    // The rescan object.
    SDeskSetup *mSetup;

    // Our list of favorite lists.
    vector<SDeskFavorite *> mFavorites;

    // We currently support fourbal status flags.
    int mStatus1;
    int mStatus2;
    int mStatus3;
    int mStatus4;

    // The status command to invoke
    char mStatus1Cmd[1024];
    char mStatus2Cmd[1024];
    char mStatus3Cmd[1024];
    char mStatus4Cmd[1024];

    // Run the status commands at specified intervals
    int mStatus1Int;
    int mStatus1Count;
    int mStatus2Int;
    int mStatus2Count;
    int mStatus3Int;
    int mStatus3Count;
    int mStatus4Int;
    int mStatus4Count;

    // The daily run script
    int mStatusDailyInt;
    int mStatusDailyCount;
    int mStatusDailyHour;
    char mLastRunDay[6];

    // Display the little type icons.    
    bool mShowLittleIcons;

    // Turn of security stuff
    bool mNoSecurity;

    // Show thumnails for pictures
    bool mShowThumb;

    // The object we use to store messages
    SDeskItemMessage *mMessage;

    // The list of excluded file names
    vector<string *> mExcludedList;

    // The list of supported picture types.
    vector<string *> mPictureExtension;

    // The list of supported music types.
    vector<string *> mMusicExtension;

    // The list of supported video types.
    vector<string *> mVideoExtension;

    // Streaming radio...
    CascadeHTTPStream mHttpStream; 
    CascadeAudioPlayer mPlayer; 

    // Flag indicating that we want to exit.
    bool mExitApp;

    // The worker thread...
    SDeskWorker mThread;
};
