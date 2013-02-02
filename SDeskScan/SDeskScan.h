#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#include <cascade/Cascade.h>

#include "SDeskLog.h"
#include "SDeskScanApp.h"
#include "SDeskScanNews.h"
#include "SDeskScanRadio.h"
#include "SDeskScanGenre.h"
#include "SDeskScanSongByTag.h"

enum tmpSCANTYPE { S_NONE, S_ALL, S_ALL_FLAT, S_RADIO, S_TAGS, S_VIDEOS };

bool CompareItem(const string* a, const string* b);
bool CompareTagItem(SDeskScanSongByTag* a, SDeskScanSongByTag* b);

class SDeskScan : public CascadeApp, SDeskLog
{
public:
    SDeskScan();
    ~SDeskScan();

    static SDeskScan& getInstance();

    const char* getRootDir() {
        return mRootDir;
    }

    void ScanDirectories(char* path);
    void ProcessInfoFile(char *name);
    void WriteFiles();
    bool IsExcluded(const char* name);
    bool IsPicture(const char* name, unsigned int *extPos);
    bool IsMusic(const char* name, unsigned int *extPos);
    bool IsVideo(const char* name, unsigned int *extPos);
    bool DoBackup() {
        return mDoBackup;
    }
    void ReadRadio(const char* url);
    void AddRadioStation(const char* name, const char* genre,
        const char* url);
    void ReadConfigFile();
    void FindMusicExtensions();
    void FixFileName(char* name);
    void AddSongByTag(const char* name, const char* fileName);

protected:
    // Methods implelemting this applications featues.
    virtual void OnAppInit();
    virtual void OnAppExit();
    virtual void OnKeyDown(u32 nKey);
    virtual void OnMountMessage(const CascadeMountMessage& mountMessage);
    virtual void OnActivate();

    int mScanType;

    int mGenreType;

    // The status command to invoke
    char mStatus1Cmd[1024];
    char mStatus2Cmd[1024];
    char mStatus3Cmd[1024];
    char mStatus4Cmd[1024];

    // Run the status commands at specified intervals
    int mStatus1Int;
    int mStatus2Int;
    int mStatus3Int;
    int mStatus4Int;

    char mRootDir[1024];

    char mMPlay[1024];

    char mRadioPlayer[1024];

    char mMusicPlayer[1024];

    char mPictureViewer[1024];

    char mWeatherSix[1024];

    char mCustomInfo[1024];
    char mCustomRotate[1024];
    char mCustomFF[1024];
    char mCustomFB[1024];

    bool mNoSecurity;

    bool mDefaultFavorites;

    bool mDoBackup;

    // Our list of music directories.
    vector<string *> mMusicDirsTmp;
    vector<string *> mMusicDirs;

    // Our list of picture directories.
    vector<string *> mPictureDirsTmp;
    vector<string *> mPictureDirs;

    // Our list of video directories.
    vector<string *> mVideoDirsTmp;
    vector<string *> mVideoDirs;

    // Our list of favorite all list.
    vector<string *> mFavoriteAll;

    // Our list of favorite songs list.
    vector<string *> mFavoriteSongs;

    // Our list of favorite pictures list.
    vector<string *> mFavoritePictures;

    // Our list of favorite video list.
    vector<string *> mFavoriteVideos;

    // Our list of favorite radio list.
    vector<string *> mFavoriteRadios;

    // Our list of slideshows.
    vector<string *> mFavoriteSlideShows;
    vector<string *> mSlideShowDirs;
    vector<SDeskScanApp *> mSlideShowList;

    // The list of applications
    vector<SDeskScanApp *> mAppsList;

    // The list of news feeds
    vector<SDeskScanNews *> mNewsList;

    // The list of genres
    vector<SDeskScanGenre *> mGenreList;

    // The list of excluded file names
    vector<string *> mExcludedList;

    // The various video genre lists.
    vector<string *> mVideoGenreAction;
    vector<string *> mVideoGenreAdventure;
    vector<string *> mVideoGenreAnimation;
    vector<string *> mVideoGenreBiography;
    vector<string *> mVideoGenreChildren;
    vector<string *> mVideoGenreComedy;
    vector<string *> mVideoGenreCrime;
    vector<string *> mVideoGenreDocumentary;
    vector<string *> mVideoGenreDrama;
    vector<string *> mVideoGenreFamily;
    vector<string *> mVideoGenreFantasy;
    vector<string *> mVideoGenreFilmNoir;
    vector<string *> mVideoGenreGameShow;
    vector<string *> mVideoGenreHDTV;
    vector<string *> mVideoGenreHistory;
    vector<string *> mVideoGenreHomeVideo;
    vector<string *> mVideoGenreHorror;
    vector<string *> mVideoGenreMovie;
    vector<string *> mVideoGenreMusic;
    vector<string *> mVideoGenreMusical;
    vector<string *> mVideoGenreMystery;
    vector<string *> mVideoGenreNews;
    vector<string *> mVideoGenreRealityTV;
    vector<string *> mVideoGenreRomance;
    vector<string *> mVideoGenreSciFi;
    vector<string *> mVideoGenreShort;
    vector<string *> mVideoGenreSitcom;
    vector<string *> mVideoGenreSoap;
    vector<string *> mVideoGenreSport;
    vector<string *> mVideoGenreSuspense;
    vector<string *> mVideoGenreTalkShow;
    vector<string *> mVideoGenreThriller;
    vector<string *> mVideoGenreWar;
    vector<string *> mVideoGenreWestern;
    vector<string *> mVideoGenreOther;

    // Flags to pick what we scan
    bool mScanPictures;

    bool mScanMusic;
    
    bool mScanVideos;

    // Force use of vlc or sonica.
    bool mUseVLCForRadio;

    bool mUseSonicaForRadio;

    // By default we overwrite video favorites.
    bool mOverwriteVideoFavorites;

    bool mHomeManagement;

    bool mCompactRootMenu;

    // The list of supported picture types.
    vector<string *> mPictureExtension;

    // The list of supported music types.
    vector<string *> mMusicExtension;

    // The list of supported video types.
    vector<string *> mVideoExtension;

    // Songs by genre.
    vector <SDeskScanSongByTag *> mSongGenreTagList;

    // Songs by album.
    vector <SDeskScanSongByTag *> mSongAlbumTagList;

    // Songs by artist.
    vector <SDeskScanSongByTag *> mSongArtistTagList;

    // Songs by ayear.
    vector <SDeskScanSongByTag *> mSongYearTagList;
};
