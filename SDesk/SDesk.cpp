// Samples: /usr/local/roku/sdk/samples/
// Includes: /usr/local/include/roku/

#include <signal.h> 
#include <unistd.h> 

#include "SDesk.h"
#include "SDeskBitmap.h"

// This is our global handle to all our state/functionality.
static SDesk* sApp = 0;

static char configFile[1024];

void SigHandler(
    int x) 
{ 
    SDeskLog::Log(1, "segfault handler called with error %d\n", x);
    _exit(-1); 
} 

int main(
    int argc,
    const char** argv)
{
    SDeskLog::LogLevel(2);

    signal(SIGSEGV, SigHandler);

    // Get the commandline arguments
    if (argc > 1) {
        strcpy(configFile, argv[1]);
    }

    // Start the main application...
    SDesk::getInstance().Run(argc, argv);

    // Cleanup the application.
    delete sApp;
    return 0;
}

SDesk& SDesk::getInstance() {
    if (!sApp) {
        sApp = new SDesk();
    }
    return *sApp;
}

SDesk::SDesk()
    : mJustSDTHM(false),
      mProtected(true),
      mExitIsUp(true),
      mNoExit(false),
      mSetupApp(false),
      mMainMenu(0),
      mCurrentMenu(0),
      mCurrentTheme(0),
      mStatus1(-1),
      mStatus2(-1),
      mStatus3(-1),
      mStatus4(-1),
      mStatus1Int(0),
      mStatus1Count(0),
      mStatus2Int(0),
      mStatus2Count(0),
      mStatus3Int(0),
      mStatus3Count(0),
      mStatus4Int(0),
      mStatus4Count(0),
      mStatusDailyInt(3600),
      mStatusDailyCount(3600),
      mStatusDailyHour(0),
      mShowLittleIcons(false),
      mNoSecurity(true),
      mExitApp(false)
{
    mScanType[0] = '\0';
    mLastRunDay[0] = '\0';
    strcpy(mMusicPlayer, "/usr/local/bin/mp3player");
    strcpy(mPictureViewer, "/usr/local/bin/photo");
}

SDesk::~SDesk()
{
    Log(7, "~SDesk()\n");

    delete mMainMenu;
    delete mCurrentTheme;
    delete mMessage;

    // Delete all excluded filename items.
    for (unsigned int i = 0; i < mExcludedList.size(); i++) {
        delete mExcludedList[i];
    }
    mExcludedList.clear();

    // Delete all favorites items.
    for (unsigned int i = 0; i < mFavorites.size(); i++) {
        delete mFavorites[i];
    }
    mFavorites.clear();

    Log(7, "~SDesk() done\n");
}

void SDesk::OnAppInit()
{
    char *t;
    struct stat filestat;

    Log(7, "SDesk::OnAppInit()\n");

    // Get the apps start directory.
    strcpy(mRootDir, GetArgv()[0]);
    t = strrchr(mRootDir, '/');
    if (t) {
        t[1] = 0;
    } else {
        mRootDir[0] = 0;
    }
    if (mRootDir[0] == '\0') {
        getcwd(mRootDir, 1023);
    }
    
    LogFile(mRootDir, true);

    // The image directory.
    if (mImagesDir[0] == '\0') {
        strcpy(mImagesDir, mRootDir);
        strcat(mImagesDir, "/images");
    }

    // Set the config file name.
    if (configFile[0] == '\0') {
        strcpy(mConfigFile, mRootDir);
        strcat(mConfigFile, "/SDesk.cfg");

        // See if the config file exists... if not... rebuild it. We
        // only do that if we use the default config file... otherwise
        // the non existence of the file is proabably due to a typo.
        if (stat(mConfigFile, &filestat) != 0) {
            strcpy(mScanType, "all");
        }
    } else {
        if (configFile[0] != '/') {
            strcpy(mConfigFile, mRootDir);
            strcat(mConfigFile, "/");
            strcat(mConfigFile, configFile);
        } else {
            strcpy(mConfigFile, configFile);
        }
    }

    // Initialize the setup scan object.
    mMessage = new SDeskItemMessage("Warning", "", false);

    // Initialize the setup scan object.
    mSetup = new SDeskSetup();

    // Our first pass on reading the config file.
    mSetupApp = true;
    SetupApp();

    if (mScanType[0] != '\0') {
        Log(7, "SDesk::OnAppInit() rescan\n");
        SDesk::getInstance().getCurrentTheme().ShowPopup(
            "Building menus...");
    }

    // Start the mplayer thread.
    mThread.Start();

    Log(7, "SDesk::OnAppInit() done\n");
}

void SDesk::OnAppExit()
{
    Log(7, "SDesk::OnAppExit()\n");
} 

void SDesk::OnActivate()
{
    if (mCurrentTheme && mCurrentMenu) {
        Log(7, "SDesk::OnActivate()\n");

        // Make the window appear.
        mCurrentTheme->Materialize();
        
        // Display the main menus menu items.
        mCurrentMenu->Select();

        Log(7, "SDesk::OnActivate() done\n");
    }
}

void SDesk::OnKeyDown(
    u32 nKey)
{
    bool val;
    char buf[1024];

    Log(7, "SDesk::OnKeyDown() key %d\n", nKey);

    if (mAskPassword) {
        char buffer[200];

        Log(7, "SDesk::OnKeyDown() ask password\n");

        if (nKey == CK_WEST) {
            if (strlen(mUserPassword) < 100) {
                strcat(mUserPassword, "w");
            }
        } else if (nKey == CK_EAST) {
            if (strlen(mUserPassword) < 100) {
                strcat(mUserPassword, "e");
            }
        } else if (nKey == CK_NORTH) {
            if (strlen(mUserPassword) < 100) {
                strcat(mUserPassword, "n");
            }
        } else if (nKey == CK_SOUTH) {
            if (strlen(mUserPassword) < 100) {
                strcat(mUserPassword, "s");
            }
        } else if (nKey == CK_SELECT || nKey == CK_EXIT) {
            Log(7, "SDesk::OnKeyDown(%s, %s) password done\n",
                mUserPassword, mNeedPassword);

            // Did we get the correct password?
            if (strcmp(mUserPassword, mNeedPassword) == 0) {
                // Set the new configuraton file
                setConfigFile(mNewConfigFile);
                
                // Disable child protection
                mProtected = false;
                
                // Set the user name
                mCurrentTheme->setUserName(mNewUserName);

                // Note that we will have to reread the configuration.
                mSetupApp = true;

                // This is an ok place to reset the setup
                SetupApp();
            }
                
            // Go back into normal mode.
            mAskPassword = false;

            // Hide the popup
            mCurrentTheme->HidePopup();
            return;
        }

        // While we are still asking for a password... print *'s
        strcpy(buffer, "Password (SELECT=done):");
        for (unsigned int counter = 0; counter < strlen(mUserPassword); 
             counter++) {
            strcat(buffer, "*");
        }
        SDesk::getInstance().getCurrentTheme().ShowPopup(buffer);

        return;
    }

    if (mCurrentMenu->OnKeyDown(nKey)) {
        Log(7, "SDesk::OnKeyDown() return\n");
        
        mCurrentTheme->Redraw();
    } else if (nKey == CK_INFO) {
        if (strlen(mCustomInfo) > 0) {
            if (strcmp(mCustomInfo, "stopplay") == 0) {
                sprintf(buf, "\"%s/apps/stopplay\"",
                    SDesk::getInstance().getRootDir());
                val = CascadeSpawn::Spawn(buf);
                if (!val) {
                    Log(1, "Could not spawn!\n");
                }
            } else {
                sprintf(buf, "\"%s\"", mCustomInfo);
                val = CascadeSpawn::Spawn(buf);
                if (!val) {
                    Log(1, "Could not spawn!\n");
                }
            }
        }
    } else if (nKey == CK_ROTATE) {
        if (strlen(mCustomRotate) > 0) {
            if (strcmp(mCustomRotate, "stopplay") == 0) {
                sprintf(buf, "\"%s/apps/stopplay\"",
                    SDesk::getInstance().getRootDir());
                val = CascadeSpawn::Spawn(buf);
                if (!val) {
                    Log(1, "Could not spawn!\n");
                }
            } else {
                sprintf(buf, "\"%s\"", mCustomRotate);
                val = CascadeSpawn::Spawn(buf);
                if (!val) {
                    Log(1, "Could not spawn!\n");
                }
            }
        }
    } else if (nKey == CK_FASTFORWARD ||
               nKey == CK_NEXT) {
        if (strlen(mCustomFF) > 0) {
            if (strcmp(mCustomFF, "stopplay") == 0) {
                sprintf(buf, "\"%s/apps/stopplay\"",
                    SDesk::getInstance().getRootDir());
                val = CascadeSpawn::Spawn(buf);
                if (!val) {
                    Log(1, "Could not spawn!\n");
                }
            } else {
                sprintf(buf, "\"%s\"", mCustomFF);
                val = CascadeSpawn::Spawn(buf);
                if (!val) {
                    Log(1, "Could not spawn!\n");
                }
            }
        }
    } else if (nKey == CK_REWIND ||
               nKey == CK_PREVIOUS) {
        if (strlen(mCustomFB) > 0) {
            if (strcmp(mCustomFB, "stopplay") == 0) {
                sprintf(buf, "\"%s/apps/stopplay\"",
                    SDesk::getInstance().getRootDir());
                val = CascadeSpawn::Spawn(buf);
                if (!val) {
                    Log(1, "Could not spawn!\n");
                }
            } else {
                sprintf(buf, "\"%s\"", mCustomFB);
                val = CascadeSpawn::Spawn(buf);
                if (!val) {
                    Log(1, "Could not spawn!\n");
                }
            }
        }
    } else if (nKey == CK_EXIT && (mNoExit == false ||
                                   mProtected == false)) {
        Log(7, "SDesk::OnKeyDown() CX_EXIT\n");
        
        // Shut down the player thread.
        mExitApp = true;

        mThread.WaitUntilFinished();

        // Release the shared memory.
        SDeskBitmap::CloseSharedMem();

        Terminate(0);
    }
    Log(7, "SDesk::OnKeyDown() done\n");
}

void SDesk::OnMountMessage(
    const CascadeMountMessage& mountMessage)
{
    Log(7, "SDesk::OnMountMessage()\n");

    // Reset the application
    mSetupApp = true;
    SetupApp();

    Log(7, "SDesk::OnMountMessage() done\n");
}

void SDesk::RescanApp()
{
    if (mScanType[0] == '\0') {
        return;
    }

    Log(7, "SDesk::RescanApp()\n");

    // Show the popup window.
    mCurrentTheme->ShowPopup("Building menus...");
    mCurrentTheme->Redraw();

    // Rescan the menus
    mSetupApp = true;
    mSetup->ReScan(mScanType);
}

void SDesk::SetupApp()
{
    bool first = false;
    struct stat filestat;
    FILE *fd;
    char *pos;
    char buf[1024];
    int lineCount = 0;

    if (!mSetupApp) {
        return;
    }

    Log(7, "SDesk::SetupApp(%s)\n", mConfigFile);

    CleanMessages();
    mSetupApp = false;

    // Set the theme file name.
    strcpy(mThemeFile, mRootDir);
    strcat(mThemeFile, "/themes/default.theme");

    // Set the default menu file.
    strcpy(mRootMenu, mRootDir);
    strcat(mRootMenu, "/menus/SDesk.mnu");

    // Set the default mplay executable.
    strcpy(mMPlay, mRootDir);
    strcat(mMPlay, "/apps/MPlay.app");

    // Set the default radio player executable.
    strcpy(mRadioPlayer, "");

    // Set the default slideshow executable.
    strcpy(mSlideshow, mRootDir);
    strcat(mSlideshow, "/apps/slideshow");

    // Set the default playmusic executable.
    strcpy(mPlaymusic, mRootDir);
    strcat(mPlaymusic, "/apps/playmusic");

    // Set the default playradio executable.
    strcpy(mPlayradio, mRootDir);
    strcat(mPlayradio, "/apps/playradio");

    // Set the default playmovie executable.
    strcpy(mPlaymovie, mRootDir);
    strcat(mPlaymovie, "/apps/playmovie");

    // Set the default playsong executable.
    strcpy(mPlaysong, mRootDir);
    strcat(mPlaysong, "/apps/playsong");

    // Set the default showpicture executable.
    strcpy(mShowpicture, mRootDir);
    strcat(mShowpicture, "/apps/showpicture");

    // Cleanup favorites
    for (unsigned int i = 0; i < mFavorites.size(); i++) {
        delete mFavorites[i];
    }
    mFavorites.clear();

    // Now read the configuration file.
    ReadConfigFile();

    // Create our main set of objects. The themes as well as the main
    // menu.
    if (mCurrentTheme == 0) {
        first = true;
        mCurrentTheme = new SDeskTheme(mThemeFile);
    }

    if (mMainMenu != 0) {
        delete mMainMenu;
    }
    mMainMenu = new SDeskItemMenu("Main Menu", "misc", mRootMenu, false, 0);
    mCurrentMenu = mMainMenu;

    // Make the window appear.
    if (first) {
        mCurrentTheme->Materialize();
    }
        
    // Perform sanity checks...
    if (stat(mMPlay, &filestat) != 0) {
        string* message1 = new string("Unable to locate MPlay (no video playing)");
        string* message2 = new string(mMPlay);
        string* message3 = new string("  Please install MPlay and run rescan.");
        AddMessage(message1);
        AddMessage(message2);
        AddMessage(message3);
    }
    if (stat(mRadioPlayer, &filestat) != 0) {
        string* message1 = new string("Unable to locate radio player (no radio playing)");
        string* message2 = new string("  Please install sonica.");
        AddMessage(message1);
        AddMessage(message2);
    }
    if (stat(mRootMenu, &filestat) != 0) {
        string* message1 = new string("No root menu file found.");
        string* message2 = new string(mRootMenu);
        string* message3 = new string("  Are you sure that you have write permsission to the files?");
        AddMessage(message1);
        AddMessage(message2);
        AddMessage(message3);
    }
    if (stat(mPlaysong, &filestat) != 0) {
        string* message1 = new string("Unable to locate playsong script.");
        string* message2 = new string(mPlaysong);
        string* message3 = new string("  playsong should have come with this prporam.");
        AddMessage(message1);
        AddMessage(message2);
        AddMessage(message3);
    }
    if (stat(mPlaymovie, &filestat) != 0) {
        string* message1 = new string("Unable to locate playmovie script.");
        string* message2 = new string(mPlaymovie);
        string* message3 = new string("  playmovie should have come with this program.");
        AddMessage(message1);
        AddMessage(message2);
        AddMessage(message3);
    }

    // Determine whether we have write permission in the current
    // directory.
    strcpy(buf, mRootDir);
    strcat(buf, "/sdesktest");
    if ((fd = fopen(buf, "w")) == NULL) {
        string* message1 = new string("Unable to write to directory:");
        string* message2 = new string(mRootDir);
        string* message3 = new string("  Please make sure you have write permission.");
        AddMessage(message1);
        AddMessage(message2);
        AddMessage(message3);
    } else {
        fclose(fd);
        unlink(buf);
    }

    // Find out whether there is a swap file
    if ((fd = fopen("/proc/swaps", "r")) == NULL) {
        Log(8, "fopen failed (/proc/swaps)...\n");
        return;
    }

    while ((pos = fgets(buf, 1023, fd)) != 0) {
        lineCount++;
    }
    fclose(fd);

    if (lineCount < 2) {
        string* message1 = new string("There appears to be no swap file. Please create a swap file to ensure");
        string* message2 = new string("  proper functioning of SDesk.");
        AddMessage(message1);
        AddMessage(message2);
    }

    // Display the main menus menu items.
    mCurrentMenu->Select();

    Log(7, "SDesk::SetupApp() done\n");
}

void SDesk::RunStatus()
{
    char buffer[1024];

    if (mStatus1Int > 0) {
        if (mStatus1Count == 0) {
            mStatus1Count = mStatus1Int;
            sprintf(buffer, "/bin/sh \"%s\"", mStatus1Cmd);
            mStatus1 = system(buffer);
            Log(7, "SDesk::RunStatus(%s, %d)\n", mStatus1Cmd, mStatus1);
        } else {
            mStatus1Count--;
        }
    }
    if (mStatus2Int > 0) {
        if (mStatus2Count == 0) {
            mStatus2Count = mStatus2Int;
            sprintf(buffer, "/bin/sh \"%s\"", mStatus2Cmd);
            mStatus2 = system(buffer);
            Log(7, "SDesk::RunStatus(%s, %d)\n", mStatus2Cmd, mStatus2);
        } else {
            mStatus2Count--;
        }
    }
    if (mStatus3Int > 0) {
        if (mStatus3Count == 0) {
            mStatus3Count = mStatus3Int;
            sprintf(buffer, "/bin/sh \"%s\"", mStatus3Cmd);
            mStatus3 = system(buffer);
            Log(7, "SDesk::RunStatus(%s, %d)\n", mStatus3Cmd, mStatus3);
        } else {
            mStatus3Count--;
        }
    }
    if (mStatus4Int > 0) {
        if (mStatus4Count == 0) {
            mStatus4Count = mStatus4Int;
            sprintf(buffer, "/bin/sh \"%s\"", mStatus4Cmd);
            mStatus4 = system(buffer);
            Log(7, "SDesk::RunStatus(%s, %d)\n", mStatus4Cmd, mStatus4);
        } else {
            mStatus4Count--;
        }
    }
    if (mStatusDailyInt > 0) {
        if (mStatusDailyCount <= 0) {
            int status;
            struct tm resTime;
            time_t tm;
            char bufferHour[6];
            char bufferDay[6];

            tm = time(NULL);
            localtime_r(&tm, &resTime);

            strftime(bufferHour, 5, "%H", &resTime);
            strftime(bufferDay, 5, "%d", &resTime);
            mStatusDailyCount = mStatusDailyInt;
            if (mStatusDailyHour >= 0 &&
                atol(bufferHour) == mStatusDailyHour &&
                strcmp(bufferDay, mLastRunDay) != 0) {
                strcpy(mLastRunDay, bufferDay);
                sprintf(buffer, "/bin/sh \"%s/apps/sdeskdaily\"", mRootDir);
                status = system(buffer);
                Log(7, "SDesk::RunStatus(%s, %d)\n", buffer, status);
            }
        } else {
            mStatusDailyCount--;
        }
    }

//    Log(7, "SDesk::RunStatus() done\n");
}

bool SDesk::IsExcluded(
    const char* name)
{
    vector<string *>::const_iterator it, end = mExcludedList.end();
    for(it = mExcludedList.begin(); it != end; it++) {
        if (strcmp(name, (*it)->c_str()) == 0) {
            return true;
        }
    }
    return false;
}

bool SDesk::HaveMessage()
{
    return mMessage->HaveMessage();
}

void SDesk::CleanMessages()
{
    mMessage->CleanMessages();
}

void SDesk::AddMessage(
    string* value)
{
    mMessage->AddMessage(value);
}

void SDesk::DrawMessage(
    CascadeBitmap& bitmap)
{
    mMessage->DrawMessage(bitmap);
}

void SDesk::ReadConfigFile()
{
    FILE *fd;
    char *pos;
    char buf[1024];
    char name[1024];
    char type[1024];
    char location[1024];
        
    if ((fd = fopen(mConfigFile, "r")) == NULL) {
        Log(8, "fopen failed (%s)...\n", mConfigFile);
        return;
    }

    // Initialize tmpdir
    strcpy(mTmpDir, "/tmp");

    while ((pos = fgets(buf, 1023, fd)) != 0) {
        if (strcmp(buf, "rootmenu\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) +
                        strlen(SDesk::getInstance().getRootDir());
                    strcpy(mRootMenu, SDesk::getInstance().getRootDir());
                    strcat(mRootMenu, "/");
                    strcat(mRootMenu, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(mRootMenu, buf);
                }
                mRootMenu[endPos] = 0;
            }
        } else if (strcmp(buf, "mplay\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) +
                        strlen(SDesk::getInstance().getRootDir());
                    strcpy(mMPlay, SDesk::getInstance().getRootDir());
                    strcat(mMPlay, "/");
                    strcat(mMPlay, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(mMPlay, buf);
                }
                mMPlay[endPos] = 0;
            }
        } else if (strcmp(buf, "imagesdir\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) +
                        strlen(SDesk::getInstance().getRootDir());
                    strcpy(mImagesDir, SDesk::getInstance().getRootDir());
                    strcat(mImagesDir, "/");
                    strcat(mImagesDir, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(mImagesDir, buf);
                }
                mMPlay[endPos] = 0;
            }
        } else if (strcmp(buf, "radioplayer\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) +
                        strlen(SDesk::getInstance().getRootDir());
                    strcpy(mRadioPlayer, SDesk::getInstance().getRootDir());
                    strcat(mRadioPlayer, "/");
                    strcat(mRadioPlayer, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(mRadioPlayer, buf);
                }
                mRadioPlayer[endPos] = 0;
            }
        } else if (strcmp(buf, "musicplayer\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) +
                        strlen(SDesk::getInstance().getRootDir());
                    strcpy(mMusicPlayer, SDesk::getInstance().getRootDir());
                    strcat(mMusicPlayer, "/");
                    strcat(mMusicPlayer, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(mMusicPlayer, buf);
                }
                mMusicPlayer[endPos] = 0;
            }
        } else if (strcmp(buf, "pictureviewer\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) +
                        strlen(SDesk::getInstance().getRootDir());
                    strcpy(mPictureViewer, SDesk::getInstance().getRootDir());
                    strcat(mPictureViewer, "/");
                    strcat(mPictureViewer, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(mPictureViewer, buf);
                }
                mPictureViewer[endPos] = 0;
            }
        } else if (strcmp(buf, "slideshow\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) +
                        strlen(SDesk::getInstance().getRootDir());
                    strcpy(mSlideshow, SDesk::getInstance().getRootDir());
                    strcat(mSlideshow, "/");
                    strcat(mSlideshow, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(mSlideshow, buf);
                }
                mSlideshow[endPos] = 0;
            }
        } else if (strcmp(buf, "playmusic\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) +
                        strlen(SDesk::getInstance().getRootDir());
                    strcpy(mPlaymusic, SDesk::getInstance().getRootDir());
                    strcat(mPlaymusic, "/");
                    strcat(mPlaymusic, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(mPlaymusic, buf);
                }
                mPlaymusic[endPos] = 0;
            }
        } else if (strcmp(buf, "playradio\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) +
                        strlen(SDesk::getInstance().getRootDir());
                    strcpy(mPlayradio, SDesk::getInstance().getRootDir());
                    strcat(mPlayradio, "/");
                    strcat(mPlayradio, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(mPlayradio, buf);
                }
                mPlayradio[endPos] = 0;
            }
        } else if (strcmp(buf, "playmovie\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) +
                        strlen(SDesk::getInstance().getRootDir());
                    strcpy(mPlaymovie, SDesk::getInstance().getRootDir());
                    strcat(mPlaymovie, "/");
                    strcat(mPlaymovie, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(mPlaymovie, buf);
                }
                mPlaymovie[endPos] = 0;
            }
        } else if (strcmp(buf, "playsong\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) +
                        strlen(SDesk::getInstance().getRootDir());
                    strcpy(mPlaysong, SDesk::getInstance().getRootDir());
                    strcat(mPlaysong, "/");
                    strcat(mPlaysong, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(mPlaysong, buf);
                }
                mPlaysong[endPos] = 0;
            }
        } else if (strcmp(buf, "showpicture\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) +
                        strlen(SDesk::getInstance().getRootDir());
                    strcpy(mShowpicture, SDesk::getInstance().getRootDir());
                    strcat(mShowpicture, "/");
                    strcat(mShowpicture, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(mShowpicture, buf);
                }
                mShowpicture[endPos] = 0;
            }
        } else if (strcmp(buf, "tmpdir\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = strlen(buf) - 1;
                strcpy(mTmpDir, buf);
                mTmpDir[endPos] = 0;
            }
        } else if (strcmp(buf, "custominfo\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/' && strcmp(buf, "stopplay\n") != 0) {
                    endPos = strlen(buf) +
                        strlen(SDesk::getInstance().getRootDir());
                    strcpy(mCustomInfo, SDesk::getInstance().getRootDir());
                    strcat(mCustomInfo, "/");
                    strcat(mCustomInfo, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(mCustomInfo, buf);
                }
                mCustomInfo[endPos] = 0;
            }
        } else if (strcmp(buf, "customrotate\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/' && strcmp(buf, "stopplay\n") != 0) {
                    endPos = strlen(buf) +
                        strlen(SDesk::getInstance().getRootDir());
                    strcpy(mCustomRotate, SDesk::getInstance().getRootDir());
                    strcat(mCustomRotate, "/");
                    strcat(mCustomRotate, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(mCustomRotate, buf);
                }
                mCustomRotate[endPos] = 0;
            }
        } else if (strcmp(buf, "customff\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/' && strcmp(buf, "stopplay\n") != 0) {
                    endPos = strlen(buf) +
                        strlen(SDesk::getInstance().getRootDir());
                    strcpy(mCustomFF, SDesk::getInstance().getRootDir());
                    strcat(mCustomFF, "/");
                    strcat(mCustomFF, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(mCustomFF, buf);
                }
                mCustomFF[endPos] = 0;
            }
        } else if (strcmp(buf, "customfb\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/' && strcmp(buf, "stopplay\n") != 0) {
                    endPos = strlen(buf) +
                        strlen(SDesk::getInstance().getRootDir());
                    strcpy(mCustomFB, SDesk::getInstance().getRootDir());
                    strcat(mCustomFB, "/");
                    strcat(mCustomFB, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(mCustomFB, buf);
                }
                mCustomFB[endPos] = 0;
            }
        } else if (strcmp(buf, "status1\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) +
                        strlen(SDesk::getInstance().getRootDir());
                    strcpy(mStatus1Cmd, SDesk::getInstance().getRootDir());
                    strcat(mStatus1Cmd, "/");
                    strcat(mStatus1Cmd, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(mStatus1Cmd, buf);
                }
                mStatus1Cmd[endPos] = 0;
            }
        } else if (strcmp(buf, "status1interval\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mStatus1Int = atol(buf);
            }
        } else if (strcmp(buf, "status2\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) +
                        strlen(SDesk::getInstance().getRootDir());
                    strcpy(mStatus2Cmd, SDesk::getInstance().getRootDir());
                    strcat(mStatus2Cmd, "/");
                    strcat(mStatus2Cmd, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(mStatus2Cmd, buf);
                }
                mStatus2Cmd[endPos] = 0;
            }
        } else if (strcmp(buf, "status2interval\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mStatus2Int = atol(buf);
            }
        } else if (strcmp(buf, "status3\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) +
                        strlen(SDesk::getInstance().getRootDir());
                    strcpy(mStatus3Cmd, SDesk::getInstance().getRootDir());
                    strcat(mStatus3Cmd, "/");
                    strcat(mStatus3Cmd, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(mStatus3Cmd, buf);
                }
                mStatus3Cmd[endPos] = 0;
            }
        } else if (strcmp(buf, "status3interval\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mStatus3Int = atol(buf);
            }
        } else if (strcmp(buf, "status4\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) +
                        strlen(SDesk::getInstance().getRootDir());
                    strcpy(mStatus4Cmd, SDesk::getInstance().getRootDir());
                    strcat(mStatus4Cmd, "/");
                    strcat(mStatus4Cmd, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(mStatus4Cmd, buf);
                }
                mStatus4Cmd[endPos] = 0;
            }
        } else if (strcmp(buf, "status4interval\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mStatus4Int = atol(buf);
            }
        } else if (strcmp(buf, "themefile\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) +
                        strlen(SDesk::getInstance().getRootDir());
                    strcpy(mThemeFile, SDesk::getInstance().getRootDir());
                    strcat(mThemeFile, "/");
                    strcat(mThemeFile, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(mThemeFile, buf);
                }
                mThemeFile[endPos] = 0;
            }
        } else if (strcmp(buf, "statusdailyhour\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mStatusDailyHour = atol(buf);
            }
        } else if (strcmp(buf, "justsdthm\n") == 0) {
            mJustSDTHM = true;
        } else if (strcmp(buf, "showlittleicons\n") == 0) {
            mShowLittleIcons = true;
        } else if (strcmp(buf, "security\n") == 0) {
            mNoSecurity = false;
        } else if (strcmp(buf, "nosecurity\n") == 0) {
            mNoSecurity = true;
        } else if (strcmp(buf, "showthumb\n") == 0) {
            mNoSecurity = false;
        } else if (strcmp(buf, "noshowthumb\n") == 0) {
            mNoSecurity = true;
        } else if (strcmp(buf, "exitisup\n") == 0) {
            mExitIsUp = true;
        } else if (strcmp(buf, "exitisquit\n") == 0) {
            mExitIsUp = false;
        } else if (strcmp(buf, "exitallowed\n") == 0) {
            mNoExit = false;
        } else if (strcmp(buf, "exitnotallowed\n") == 0) {
            mNoExit = true;
        } else if (strcmp(buf, "protected\n") == 0) {
            mProtected = true;
        } else if (strcmp(buf, "notprotected\n") == 0) {
            mProtected = false;
        } else if (strcmp(buf, "debug\n") == 0) {
            SDeskLog::LogLevel(9);
        } else if (strcmp(buf, "excluded\n") == 0) {
            if ((pos = fgets(buf, 200, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            mExcludedList.push_back(new string(name));
        } else if (strcmp(buf, "musicextension\n") == 0) {
            if ((pos = fgets(buf, 200, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            mMusicExtension.push_back(new string(name));
        } else if (strcmp(buf, "pictureextension\n") == 0) {
            if ((pos = fgets(buf, 200, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            mPictureExtension.push_back(new string(name));
        } else if (strcmp(buf, "videoextension\n") == 0) {
            if ((pos = fgets(buf, 200, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            mVideoExtension.push_back(new string(name));
        } else if (strcmp(buf, "favorite\n") == 0) {
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
            mFavorites.push_back(new SDeskFavorite(name, location, type));
        }
    }

    fclose(fd);
}

bool SDesk::IsPicture(
    const char* name,
    unsigned int *extPos)
{
    char buffer[1024];
    const char *fromPos = name;
    char *toPos = buffer;
    vector<string *>::const_iterator it, end = mPictureExtension.end();

    while (fromPos && *fromPos) {
        *toPos = tolower(*fromPos);
        toPos++;
        fromPos++;
    }
    *toPos = 0;
    for(it = mPictureExtension.begin(); it != end; it++) {
        *extPos = strlen(buffer) - strlen((*it)->c_str());
        if (strncmp(buffer + *extPos, (*it)->c_str(),
                    strlen((*it)->c_str())) == 0) {
            return true;
        }
    }
    *extPos = 0;
    return false;
}

bool SDesk::IsMusic(
    const char* name,
    unsigned int *extPos)
{
    char buffer[1024];
    const char *fromPos = name;
    char *toPos = buffer;
    vector<string *>::const_iterator it, end = mMusicExtension.end();

    while (fromPos && *fromPos) {
        *toPos = tolower(*fromPos);
        toPos++;
        fromPos++;
    }
    *toPos = 0;
    for(it = mMusicExtension.begin(); it != end; it++) {
        *extPos = strlen(buffer) - strlen((*it)->c_str());
        if (strncmp(buffer + *extPos, (*it)->c_str(),
                    strlen((*it)->c_str())) == 0) {
            return true;
        }
    }
    *extPos = 0;
    return false;
}

bool SDesk::IsVideo(
    const char* name,
    unsigned int *extPos)
{
    char buffer[1024];
    const char *fromPos = name;
    char *toPos = buffer, *pos, *slashPos;
    vector<string *>::const_iterator it, end = mVideoExtension.end();

    // Ok... a little crude, but if the name ends with a .<number>.ext,
    // where <number> is larger than 1, we skip the file assuming it's
    // a sequence file.
    strcpy(buffer, name);
    slashPos = strrchr(buffer, '/');
    pos = strrchr(buffer, '.');
    if (pos && slashPos < pos) {
        // found a file extension
        bool isFirstFile = true;

        *pos = 0;
        pos--;
        Log(7, "SDesk::IsVideo(%s) skip?\n", buffer);
        if (pos > buffer && *pos != '1') {
            // Not a 1 at the end... we are not the first file in the
            // sequence.
            isFirstFile = false;
            Log(7, "SDesk::IsVideo(%c) not first?\n", *pos);
        } else {
            if (pos > buffer) {
                pos--;
            }
        }
        while(pos > buffer && isdigit(*pos)) {
            Log(7, "SDesk::IsVideo(%c) char\n", *pos);
            if (*pos != '0') {
                // Not a leading 0... not the first file in the sequence.
                Log(7, "SDesk::IsVideo(%c) not first?\n", *pos);
                isFirstFile = false;
            }
            pos--;
        }
        Log(7, "SDesk::IsVideo(%c) dot?\n", *pos);
        if (*pos == '.' && !isFirstFile) {
            Log(7, "SDesk::IsVideo() dont add\n");
            // We are a sequence file, and we are not the first... so jsut
            // return false.
            return false;
        }
    }

    while (fromPos && *fromPos) {
        *toPos = tolower(*fromPos);
        toPos++;
        fromPos++;
    }
    *toPos = 0;
    for(it = mVideoExtension.begin(); it != end; it++) {
        *extPos = strlen(buffer) - strlen((*it)->c_str());
        if (strncmp(buffer + *extPos, (*it)->c_str(), 
                    strlen((*it)->c_str())) == 0) {
            return true;
        }
    }
    *extPos = 0;
    return false;
}


