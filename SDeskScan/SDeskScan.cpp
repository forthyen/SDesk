#include <signal.h> 
#include <unistd.h> 

#include <cascade/net/CascadeHTTPClient.h> 
#include <cascade/util/CascadeFileStream.h> 

#include <mxml.h>

#include "read_tags.h"

#include "SDeskScan.h"
#include "SDeskDirScanner.h"

// This is our global handle to all our state/functionality.
static SDeskScan* sApp = 0;

static char scanType[100];

static char rootDir[1024];

// Define the config file content
#define MPLAY "mplay
"
#define RADIOPLAYER "
radioplayer
"
#define MUSICPLAYER "
musicplayer
"
#define PICTUREVIEWER "
pictureviewer
"
#define CUSTOM_INFO "
custominfo
"
#define NO_SECURITY "
nosecurity"
#define DO_SECURITY "
security"
#define CUSTOM_ROTATE "
customrotate
"
#define CUSTOM_FF "
customff
"
#define CUSTOM_FB "
customfb
"
#define SDESK_CFG "
exitisup
exitallowed
customrotate
stopplay
customff
apps/nextsong
customfb
apps/prevsong
excluded
System Volume Information
excluded
Recycle
excluded
folder.jpg
excluded
cover.jpg
"
#define SDESK_MNU_FAVORITES "%menu
favorites
Favorites
menus/MyFavorites.mnu
0
"
#define SDESK_MNU_VIDEOS "%menu
movies
Videos
menus/MyVideos.mnu
0
"
#define SDESK_MNU_TV "%menu
movies
TV
/mnt/smb/dune/tv
0
"
#define SDESK_MNU_MUSIC "%menu
songs
Music
menus/MyMusic.mnu
0
"
#define SDESK_MNU_RADIO "%menu
radios
Radio
menus/Radio/MyRadio.mnu
0
"
#define SDESK_MNU_PICTURES "%menu
pictures
Pictures
menus/MyPictures.mnu
0
"
#define SDESK_MNU_NEWS "%rssfeed
News
http://rss.news.yahoo.com/rss/topstories
0
"
#define SDESK_MNU_HOME_MANAGEMENT "%menu
apps
Home Management
menus/HomeManagement.mnu
0
"
#define SDESK_MNU_HOME_MANAGEMENT_MENU "%url
Morning
http://192.168.0.3/cgi-bin/morning.cgi
0
%url
Night
http://192.168.0.3/cgi-bin/night.cgi
0
"
#define SDESK_MNU_NEWS_FILE "%menu
rssfeed
News
menus/MyNews.mnu
0
"
#define SDESK_MNU_SLIDESHOWS "%menu
favorites
SlideShows
menus/MySlideShows.mnu
0
"
#define SDESK_MNU_BOTTOM "%menu
apps
Applications
menus/MyApplications.mnu
0
"
#define SDESK_APPS_SPACE "%space
15
0
"
#define SDESK_APPS_BOTTOM "%user
Change To Default User
SDesk.cfg
Default

%user
Change To Super User
SDesk.cfg
Super
wewe
%space
15
0
%menu
misc
SDesk Setup
menus/Setup.mnu
1
%application
Roku Setup
/usr/local/bin/setup
1
%application
Reboot Roku
apps/myreboot
1
"
#define SETUP "%rescan
Rescan menus
all
1
%rescan
Rescan menus flat structure
allflat
1
"
#define RESCAN_RADIO "
%rescan
Rescan all radio stations
radio
1
%rescan
Rescan Ambient stations
radio_Ambient
1
%rescan
Rescan Blues stations
radio_Blues
1
%rescan
Rescan Christian stations
radio_Christian
1
%rescan
Rescan Classic stations
radio_Classic
1
%rescan
Rescan Country stations
radio_Country
1
%rescan
Rescan Dance stations
radio_Dance
1
%rescan
Rescan Folk stations
radio_Folk
1
%rescan
Rescan Goth stations
radio_Goth
1
%rescan
Rescan Jazz stations
radio_Jazz
1
%rescan
Rescan Latin stations
radio_Latin
1
%rescan
Rescan National stations
radio_National
1
%rescan
Rescan New Age stations
radio_New_Age
1
%rescan
Rescan Oldies stations
radio_Oldies
1
%rescan
Rescan Other stations
radio_Other
1
%rescan
Rescan Pop stations
radio_Pop
1
%rescan
Rescan Rap stations
radio_Rap
1
%rescan
Rescan Reggae stations
radio_Reggae
1
%rescan
Rescan Rock stations
radio_Rock
1
%rescan
Rescan Talk stations
radio_Talk
1
%rescan
Rescan Techno stations
radio_Techno
1
"
#define MYRADIO "%menu
radios
Ambient
menus/Radio/Ambient.mnu
0
%menu
radios
Blues
menus/Radio/Blues.mnu
0
%menu
radios
Christian
menus/Radio/Christian.mnu
0
%menu
radios
Classic
menus/Radio/Classic.mnu
0
%menu
radios
Country
menus/Radio/Country.mnu
0
%menu
radios
Dance
menus/Radio/Dance.mnu
0
%menu
radios
Folk
menus/Radio/Folk.mnu
0
%menu
radios
Goth
menus/Radio/Goth.mnu
0
%menu
radios
Jazz
menus/Radio/Jazz.mnu
0
%menu
radios
Latin
menus/Radio/Latin.mnu
0
%menu
radios
National
menus/Radio/National.mnu
0
%menu
radios
New Age
menus/Radio/New Age.mnu
0
%menu
radios
Oldies
menus/Radio/Oldies.mnu
0
%menu
radios
Other
menus/Radio/Other.mnu
0
%menu
radios
Pop
menus/Radio/Pop.mnu
0
%menu
radios
Rap
menus/Radio/Rap.mnu
0
%menu
radios
Reggae
menus/Radio/Reggae.mnu
0
%menu
radios
Rock
menus/Radio/Rock.mnu
0
%menu
radios
Talk
menus/Radio/Talk.mnu
0
%menu
radios
Techno
menus/Radio/Techno.mnu
0
%space
15
0
%menu
misc
Rescan Radio Stations
menus/RescanRadio.mnu
1
"

#define MUSIC_EXTS "musicextension
"
#define PICTURE_EXTS "pictureextension
"
#define VIDEO_EXTS "videoextension
"

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
    // Set this to 9 to debug early code (before readconfig)
    SDeskLog::LogLevel(9);

    signal(SIGSEGV, SigHandler);

    // Get the commandline arguments
    if (argc > 1) {
        strcpy(scanType, argv[1]);
    }
    if (argc > 2) {
        strcpy(rootDir, argv[2]);
    }

    // Start the main application...
    SDeskScan::getInstance().Run(argc, argv);

    // Cleanup the application.
    delete sApp;
    return 0;
}

SDeskScan& SDeskScan::getInstance() {
    if (!sApp) {
        sApp = new SDeskScan();
    }
    return *sApp;
}

SDeskScan::SDeskScan()
    : mScanType(S_ALL),
      mGenreType(G_ALL),
      mStatus1Int(0),
      mStatus2Int(0),
      mStatus3Int(0),
      mStatus4Int(0),
      mNoSecurity(true),
      mDefaultFavorites(true),
      mDoBackup(false),
      mUseVLCForRadio(true),
      mUseSonicaForRadio(false),
      mOverwriteVideoFavorites(true),
      mHomeManagement(false),
      mCompactRootMenu(false)
{
    mStatus1Cmd[0] = 0;
    mStatus2Cmd[0] = 0;
    mStatus3Cmd[0] = 0;
    mStatus4Cmd[0] = 0;
}

SDeskScan::~SDeskScan()
{
    Log(7, "~SDeskScan()\n");

    Log(7, "~SDeskScan() done\n");
}

void SDeskScan::OnAppInit()
{
    char *t;

    Log(7, "SDeskScan::OnAppInit()\n");

    if (rootDir[0] != '\0') {
        strcpy(mRootDir, rootDir);
    } else {
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
    }

    LogFile(mRootDir, true);

    if (strcmp(scanType, "allflat") == 0) {
        Log(7, "SDeskScan::OnAppInit(%s) allflat\n", mRootDir);
        mScanType = S_ALL_FLAT;
        mScanMusic = true;
        mScanPictures = true;
        mScanVideos = true;
    } else if (strcmp(scanType, "videos") == 0) {
        Log(7, "SDeskScan::OnAppInit(%s) videos\n", mRootDir);
        mScanType = S_VIDEOS;
        mScanVideos = true;
    } else if (strcmp(scanType, "tags") == 0) {
        Log(7, "SDeskScan::OnAppInit(%s) tags\n", mRootDir);
        mScanType = S_TAGS;
        mScanMusic = true;
    } else if (strcmp(scanType, "radio") == 0) {
        Log(7, "SDeskScan::OnAppInit(%s) radios\n", mRootDir);
        mScanType = S_RADIO;
        mGenreType = G_ALL;
    } else if (strcmp(scanType, "radio_Ambient") == 0) {
        Log(7, "SDeskScan::OnAppInit(%s) radios ambient\n", mRootDir);
        mScanType = S_RADIO;
        mGenreType = G_AMBIENT;
    } else if (strcmp(scanType, "radio_Blues") == 0) {
        Log(7, "SDeskScan::OnAppInit(%s) radios blues\n", mRootDir);
        mScanType = S_RADIO;
        mGenreType = G_BLUES;
    } else if (strcmp(scanType, "radio_Christian") == 0) {
        Log(7, "SDeskScan::OnAppInit(%s) radios christian\n", mRootDir);
        mScanType = S_RADIO;
        mGenreType = G_CHRISTIAN;
    } else if (strcmp(scanType, "radio_Classic") == 0) {
        Log(7, "SDeskScan::OnAppInit(%s) radios classic\n", mRootDir);
        mScanType = S_RADIO;
        mGenreType = G_CLASSIC;
    } else if (strcmp(scanType, "radio_Country") == 0) {
        Log(7, "SDeskScan::OnAppInit(%s) radios country\n", mRootDir);
        mScanType = S_RADIO;
        mGenreType = G_COUNTRY;
    } else if (strcmp(scanType, "radio_Dance") == 0) {
        Log(7, "SDeskScan::OnAppInit(%s) radios dance\n", mRootDir);
        mScanType = S_RADIO;
        mGenreType = G_DANCE;
    } else if (strcmp(scanType, "radio_Folk") == 0) {
        Log(7, "SDeskScan::OnAppInit(%s) radios folk\n", mRootDir);
        mScanType = S_RADIO;
        mGenreType = G_FOLK;
    } else if (strcmp(scanType, "radio_Goth") == 0) {
        Log(7, "SDeskScan::OnAppInit(%s) radios goth\n", mRootDir);
        mScanType = S_RADIO;
        mGenreType = G_GOTH;
    } else if (strcmp(scanType, "radio_Jazz") == 0) {
        Log(7, "SDeskScan::OnAppInit(%s) radios jazz\n", mRootDir);
        mScanType = S_RADIO;
        mGenreType = G_JAZZ;
    } else if (strcmp(scanType, "radio_Latin") == 0) {
        Log(7, "SDeskScan::OnAppInit(%s) radios latin\n", mRootDir);
        mScanType = S_RADIO;
        mGenreType = G_LATIN;
    } else if (strcmp(scanType, "radio_National") == 0) {
        Log(7, "SDeskScan::OnAppInit(%s) radios national\n", mRootDir);
        mScanType = S_RADIO;
        mGenreType = G_NATIONAL;
    } else if (strcmp(scanType, "radio_New_Age") == 0) {
        Log(7, "SDeskScan::OnAppInit(%s) radios new age\n", mRootDir);
        mScanType = S_RADIO;
        mGenreType = G_NEW_AGE;
    } else if (strcmp(scanType, "radio_Oldies") == 0) {
        Log(7, "SDeskScan::OnAppInit(%s) radios oldies\n", mRootDir);
        mScanType = S_RADIO;
        mGenreType = G_OLDIES;
    } else if (strcmp(scanType, "radio_Other") == 0) {
        Log(7, "SDeskScan::OnAppInit(%s) radios other\n", mRootDir);
        mScanType = S_RADIO;
        mGenreType = G_ALL;
    } else if (strcmp(scanType, "radio_Pop") == 0) {
        Log(7, "SDeskScan::OnAppInit(%s) radios pop\n", mRootDir);
        mScanType = S_RADIO;
        mGenreType = G_POP;
    } else if (strcmp(scanType, "radio_Rap") == 0) {
        Log(7, "SDeskScan::OnAppInit(%s) radios rap\n", mRootDir);
        mScanType = S_RADIO;
        mGenreType = G_RB;
    } else if (strcmp(scanType, "radio_Reggae") == 0) {
        Log(7, "SDeskScan::OnAppInit(%s) radios reggae\n", mRootDir);
        mScanType = S_RADIO;
        mGenreType = G_REGGAE;
    } else if (strcmp(scanType, "radio_Rock") == 0) {
        Log(7, "SDeskScan::OnAppInit(%s) radios rock\n", mRootDir);
        mScanType = S_RADIO;
        mGenreType = G_ROCK;
    } else if (strcmp(scanType, "radio_Talk") == 0) {
        Log(7, "SDeskScan::OnAppInit(%s) radios talk\n", mRootDir);
        mScanType = S_RADIO;
        mGenreType = G_TALK;
    } else if (strcmp(scanType, "radio_Techno") == 0) {
        Log(7, "SDeskScan::OnAppInit(%s) radios techno\n", mRootDir);
        mScanType = S_RADIO;
        mGenreType = G_TECHNO;
    } else {
        Log(7, "SDeskScan::OnAppInit(%s) all\n", mRootDir);
        mScanType = S_ALL;
        mScanMusic = true;
        mScanPictures = true;
        mScanVideos = true;
    }

    // Build genre list
    SDeskScanGenre *genre;
    if (mGenreType == G_ALL) {
        genre = new SDeskScanGenre("Other", G_OTHER);
        mGenreList.push_back(genre);
    }
    if (mGenreType == G_ALL || mGenreType == G_BLUES) {
        genre = new SDeskScanGenre("Blues", G_BLUES);
        genre->addPattern("blues");
        mGenreList.push_back(genre);
    }
    if (mGenreType == G_ALL || mGenreType == G_POP) {
        genre = new SDeskScanGenre("Pop", G_POP);
        genre->addPattern("pop");
        mGenreList.push_back(genre);
    }
    if (mGenreType == G_ALL || mGenreType == G_JAZZ) {
        genre = new SDeskScanGenre("Jazz", G_JAZZ);
        genre->addPattern("jazz");
        mGenreList.push_back(genre);
    }
    if (mGenreType == G_ALL || mGenreType == G_CLASSIC) {
        genre = new SDeskScanGenre("Classic", G_CLASSIC);
        genre->addPattern("classic");
        mGenreList.push_back(genre);
    }
    if (mGenreType == G_ALL || mGenreType == G_DANCE) {
        genre = new SDeskScanGenre("Dance", G_DANCE);
        genre->addPattern("dance");
        mGenreList.push_back(genre);
    }
    if (mGenreType == G_ALL || mGenreType == G_TECHNO) {
        genre = new SDeskScanGenre("Techno", G_TECHNO);
        genre->addPattern("techno");
        mGenreList.push_back(genre);
    }
    if (mGenreType == G_ALL || mGenreType == G_GOTH) {
        genre = new SDeskScanGenre("Goth", G_GOTH);
        genre->addPattern("goth");
        mGenreList.push_back(genre);
    }
    if (mGenreType == G_ALL || mGenreType == G_ROCK) {
        genre = new SDeskScanGenre("Rock", G_ROCK);
        genre->addPattern("rock");
        genre->addPattern("metal");
        mGenreList.push_back(genre);
    }
    if (mGenreType == G_ALL || mGenreType == G_RB) {
        genre = new SDeskScanGenre("Rap", G_RB);
        genre->addPattern("rap");
        genre->addPattern("r&b");
        genre->addPattern("rnb");
        genre->addPattern("hiphop");
        genre->addPattern("hip hop");
        mGenreList.push_back(genre);
    }
    if (mGenreType == G_ALL || mGenreType == G_COUNTRY) {
        genre = new SDeskScanGenre("Country", G_COUNTRY);
        genre->addPattern("country");
        mGenreList.push_back(genre);
    }
    if (mGenreType == G_ALL || mGenreType == G_NEW_AGE) {
        genre = new SDeskScanGenre("New Age", G_NEW_AGE);
        genre->addPattern("new age");
        genre->addPattern("newage");
        mGenreList.push_back(genre);
    }
    if (mGenreType == G_ALL || mGenreType == G_TALK) {
        genre = new SDeskScanGenre("Talk", G_TALK);
        genre->addPattern("talk");
        genre->addPattern("sport");
        mGenreList.push_back(genre);
    }
    if (mGenreType == G_ALL || mGenreType == G_REGGAE) {
        genre = new SDeskScanGenre("Reggae", G_REGGAE);
        genre->addPattern("reggae");
        mGenreList.push_back(genre);
    }
    if (mGenreType == G_ALL || mGenreType == G_AMBIENT) {
        genre = new SDeskScanGenre("Ambient", G_AMBIENT);
        genre->addPattern("ambient");
        mGenreList.push_back(genre);
    }
    if (mGenreType == G_ALL || mGenreType == G_FOLK) {
        genre = new SDeskScanGenre("Folk", G_FOLK);
        genre->addPattern("folk");
        mGenreList.push_back(genre);
    }
    if (mGenreType == G_ALL || mGenreType == G_LATIN) {
        genre = new SDeskScanGenre("Latin", G_LATIN);
        genre->addPattern("latin");
        genre->addPattern("salsa");
        mGenreList.push_back(genre);
    }
    if (mGenreType == G_ALL || mGenreType == G_OLDIES) {
        genre = new SDeskScanGenre("Oldies", G_OLDIES);
        genre->addPattern("oldies");
        genre->addPattern("30s");
        genre->addPattern("40s");
        genre->addPattern("50s");
        genre->addPattern("60s");
        genre->addPattern("70s");
        genre->addPattern("80s");
        genre->addPattern("90s");
        genre->addPattern("30 s");
        genre->addPattern("40 s");
        genre->addPattern("50 s");
        genre->addPattern("60 s");
        genre->addPattern("70 s");
        genre->addPattern("80 s");
        genre->addPattern("90 s");
        mGenreList.push_back(genre);
    }
    if (mGenreType == G_ALL || mGenreType == G_CHRISTIAN) {
        genre = new SDeskScanGenre("Christian", G_CHRISTIAN);
        genre->addPattern("christ");
        genre->addPattern("worship");
        genre->addPattern("spiritual");
        genre->addPattern("prayer");
        genre->addPattern("gospel");
        mGenreList.push_back(genre);
    }
    if (mGenreType == G_ALL || mGenreType == G_NATIONAL) {
        genre = new SDeskScanGenre("National", G_NATIONAL);
        genre->addPattern("japan");
        genre->addPattern("francais");
        genre->addPattern("french");
        genre->addPattern("german");
        genre->addPattern("polska");
        genre->addPattern("polskie");
        genre->addPattern("polish");
        genre->addPattern("arab");
        genre->addPattern("russia");
        genre->addPattern("hindi");
        genre->addPattern("ghanaia");
        genre->addPattern("indie");
        genre->addPattern("india");
        genre->addPattern("apna");
        genre->addPattern("punjabi");
        genre->addPattern("india");
        genre->addPattern("world");
        genre->addPattern("europe");
        genre->addPattern("africa");
        genre->addPattern("asia");
        genre->addPattern("turkce");
        genre->addPattern("turkish");
        genre->addPattern("turky");
        genre->addPattern("irak");
        genre->addPattern("persia");
        genre->addPattern("iran");
        genre->addPattern("indonesia");
        genre->addPattern("hawaii");
        genre->addPattern("netherland");
        genre->addPattern("nederland");
        genre->addPattern("vietnam");
        genre->addPattern("thai");
        genre->addPattern("romania");
        genre->addPattern("greek");
        genre->addPattern("greece");
        genre->addPattern("hungary");
        mGenreList.push_back(genre);
    }

    // Don't scan the internal disk
    mExcludedList.insert(mExcludedList.end(), 
                         new string("flash0"));

    // Read config file
    ReadConfigFile();

    // Find music extensions that are supported.
    FindMusicExtensions();

    Log(1, "SDeskScan::OnAppInit(%s, %s, %s, %s, %s, %d)\n", mRootDir, 
        scanType, GetArgv()[0], GetArgv()[1], GetArgv()[2], mScanType);

    // Set default video and picture extensions...
    if (mMusicExtension.size() == 0) {
        mMusicExtension.insert(mMusicExtension.end(), 
                               new string(".mp3"));
        mMusicExtension.insert(mMusicExtension.end(), 
                               new string(".MP3"));
    }
    if (mPictureExtension.size() == 0) {
        mPictureExtension.insert(mPictureExtension.end(), 
                                 new string(".gif"));
        mPictureExtension.insert(mPictureExtension.end(), 
                                 new string(".jpg"));
        mPictureExtension.insert(mPictureExtension.end(), 
                                 new string(".jpeg"));
        mPictureExtension.insert(mPictureExtension.end(), 
                                 new string(".GIF"));
        mPictureExtension.insert(mPictureExtension.end(), 
                                 new string(".JPG"));
        mPictureExtension.insert(mPictureExtension.end(), 
                                 new string(".JPEG"));
    }
    if (mVideoExtension.size() == 0) {
        mVideoExtension.insert(mVideoExtension.end(), 
                               new string(".mpg"));
        mVideoExtension.insert(mVideoExtension.end(), 
                               new string(".mpeg"));
        mVideoExtension.insert(mVideoExtension.end(), 
                               new string(".vob"));
        mVideoExtension.insert(mVideoExtension.end(), 
                               new string(".ts"));
        mVideoExtension.insert(mVideoExtension.end(), 
                               new string(".ps"));
        mVideoExtension.insert(mVideoExtension.end(), 
                               new string(".MPG"));
        mVideoExtension.insert(mVideoExtension.end(), 
                               new string(".MPEG"));
        mVideoExtension.insert(mVideoExtension.end(), 
                               new string(".VOB"));
        mVideoExtension.insert(mVideoExtension.end(), 
                               new string(".TS"));
        mVideoExtension.insert(mVideoExtension.end(), 
                               new string(".PS"));
    }

    // Prefill default favorites
    if (mDefaultFavorites) {
        // Create a default list of video favorites for
        // user driven classification.
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Action"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Adventure"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Animation"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Biography"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Children"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Comedy"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Crime"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Documentary"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Drama"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Family"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Fantasy"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Film Noir"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Game Show"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("HDTV"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("History"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Home Video"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Horror"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Movie"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Music"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Musical"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Mystery"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("News"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Reality TV"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Romance"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Sci-Fi"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Short"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Sitcom"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Soap"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Sport"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Suspense"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Talk Show"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Thriller"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("War"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Western"));
        mFavoriteVideos.insert(mFavoriteVideos.begin(),
                               new string("Other"));
        mFavoriteRadios.insert(mFavoriteRadios.begin(),
                               new string("Stations"));
    }

    // Scan filesystem
    if (mScanType == S_ALL || mScanType == S_ALL_FLAT ||
        mScanType == S_TAGS || mScanType == S_VIDEOS) {
        ScanDirectories("/mnt");
//        ScanDirectories("/mnt/smb/terra/media/Apps/Develop/src/SDeskScan/test");
    }

    // Scan radio stations
    if (mScanType == S_ALL || mScanType == S_ALL_FLAT || 
        mScanType == S_RADIO) {
        ReadRadio("http://www.shoutcast.com/sbin/xmllister.phtml?limit=1000&service=aol&no_compress=1");
    }

    // Find directories that have pictures as well as music... those 
    // will be considered Slideshows.
    vector<string *>::const_iterator it, end = mPictureDirsTmp.end();
    for(it = mPictureDirsTmp.begin(); it != end; it++) {
        const char* tmpBuf1 = (*it)->c_str();
        vector<string *>::const_iterator it2, end2 = mMusicDirsTmp.end();
        for(it2 = mMusicDirsTmp.begin(); it2 != end2; it2++) {
            const char* tmpBuf2 = (*it2)->c_str();
            if (strcmp(tmpBuf1, tmpBuf2) == 0) {
                mSlideShowDirs.insert(mSlideShowDirs.end(),
                                      new string(tmpBuf1));
            }
        }
    }

    // Do a backwards search to weed out the duplicates
    if (mScanType == S_ALL || mScanType == S_TAGS || mScanType == S_VIDEOS) {
        vector<string *>::reverse_iterator it, end = mPictureDirsTmp.rend();
        for(it = mPictureDirsTmp.rbegin(); it != end; it++) {
            const char* tmpBuf = (*it)->c_str();
            bool addToList = true;
            vector<string *>::const_iterator it2, end2 = mPictureDirs.end();
            for(it2 = mPictureDirs.begin(); it2 != end2; it2++) {
                if (strncmp(tmpBuf, (*it2)->c_str(),
                            strlen((*it2)->c_str())) == 0) {
                    addToList = false;
                    break;
                }
            }
            vector<string *>::const_iterator it3, end3 = mSlideShowDirs.end();
            for(it3 = mSlideShowDirs.begin(); it3 != end3; it3++) {
                if (strcmp(tmpBuf, (*it3)->c_str()) == 0) {
                    addToList = false;
                    break;
                }
            }
            if (addToList) {
                mPictureDirs.insert(mPictureDirs.end(), new string(tmpBuf));
            }
        }
        end = mVideoDirsTmp.rend();
        for(it = mVideoDirsTmp.rbegin(); it != end; it++) {
            const char* tmpBuf = (*it)->c_str();
            bool addToList = true;
            vector<string *>::const_iterator it2, end2 = mVideoDirs.end();
            for(it2 = mVideoDirs.begin(); it2 != end2; it2++) {
                if (strncmp(tmpBuf, (*it2)->c_str(), 
                            strlen((*it2)->c_str())) == 0) {
                    addToList = false;
                    break;
                }
            }
            if (addToList) {
                mVideoDirs.insert(mVideoDirs.end(), new string(tmpBuf));
            }
        }
        end = mMusicDirsTmp.rend();
        for(it = mMusicDirsTmp.rbegin(); it != end; it++) {
            const char* tmpBuf = (*it)->c_str();
            bool addToList = true;
            vector<string *>::const_iterator it2, end2 = mMusicDirs.end();
            for(it2 = mMusicDirs.begin(); it2 != end2; it2++) {
                if (strncmp(tmpBuf, (*it2)->c_str(), 
                            strlen((*it2)->c_str())) == 0) {
                    addToList = false;
                    break;
                }
            }
            vector<string *>::const_iterator it3, end3 = mSlideShowDirs.end();
            for(it3 = mSlideShowDirs.begin(); it3 != end3; it3++) {
                if (strcmp(tmpBuf, (*it3)->c_str()) == 0) {
                    addToList = false;
                    break;
                }
            }
            if (addToList) {
                mMusicDirs.insert(mMusicDirs.end(), new string(tmpBuf));
            }
        }
    } else if (mScanType == S_ALL_FLAT) {
        vector<string *>::reverse_iterator it, end = mPictureDirsTmp.rend();
        for(it = mPictureDirsTmp.rbegin(); it != end; it++) {
            const char* tmpBuf = (*it)->c_str();
            mPictureDirs.insert(mPictureDirs.end(), new string(tmpBuf));
        }
        end = mVideoDirsTmp.rend();
        for(it = mVideoDirsTmp.rbegin(); it != end; it++) {
            const char* tmpBuf = (*it)->c_str();
            mVideoDirs.insert(mVideoDirs.end(), new string(tmpBuf));
        }
        end = mMusicDirsTmp.rend();
        for(it = mMusicDirsTmp.rbegin(); it != end; it++) {
            const char* tmpBuf = (*it)->c_str();
            mMusicDirs.insert(mMusicDirs.end(), new string(tmpBuf));
        }
    }

    // Sort the lists...
    sort(mVideoDirs.begin(),
         mVideoDirs.end(), CompareItem);
    sort(mPictureDirs.begin(),
         mPictureDirs.end(), CompareItem);
    sort(mMusicDirs.begin(),
         mMusicDirs.end(), CompareItem);
    sort(mSlideShowDirs.begin(),
         mSlideShowDirs.end(), CompareItem);
    sort(mFavoriteAll.begin(),
         mFavoriteAll.end(), CompareItem);
    sort(mFavoriteSongs.begin(),
         mFavoriteSongs.end(), CompareItem);
    sort(mFavoritePictures.begin(),
         mFavoritePictures.end(), CompareItem);
    sort(mFavoriteVideos.begin(), 
         mFavoriteVideos.end(), CompareItem);
    sort(mFavoriteSlideShows.begin(),
         mFavoriteSlideShows.end(), CompareItem);
    sort(mFavoriteRadios.begin(),
         mFavoriteRadios.end(), CompareItem);
    sort(mSongGenreTagList.begin(),
         mSongGenreTagList.end(), CompareTagItem);
    sort(mSongAlbumTagList.begin(),
         mSongAlbumTagList.end(), CompareTagItem);
    sort(mSongArtistTagList.begin(),
         mSongArtistTagList.end(), CompareTagItem);
    sort(mSongYearTagList.begin(),
         mSongYearTagList.end(), CompareTagItem);

    // sort the video genre lists...
    sort(mVideoGenreAction.begin(), 
         mVideoGenreAction.end(), CompareItem);
    sort(mVideoGenreAdventure.begin(), 
         mVideoGenreAdventure.end(), CompareItem);
    sort(mVideoGenreAnimation.begin(), 
         mVideoGenreAnimation.end(), CompareItem);
    sort(mVideoGenreBiography.begin(), 
         mVideoGenreBiography.end(), CompareItem);
    sort(mVideoGenreChildren.begin(), 
         mVideoGenreChildren.end(), CompareItem);
    sort(mVideoGenreComedy.begin(),
         mVideoGenreComedy.end(), CompareItem);
    sort(mVideoGenreCrime.begin(),
         mVideoGenreCrime.end(), CompareItem);
    sort(mVideoGenreDocumentary.begin(),
         mVideoGenreDocumentary.end(), CompareItem);
    sort(mVideoGenreDrama.begin(),
         mVideoGenreDrama.end(), CompareItem);
    sort(mVideoGenreFamily.begin(), 
         mVideoGenreFamily.end(), CompareItem);
    sort(mVideoGenreFantasy.begin(),
         mVideoGenreFantasy.end(), CompareItem);
    sort(mVideoGenreFilmNoir.begin(),
         mVideoGenreFilmNoir.end(), CompareItem);
    sort(mVideoGenreGameShow.begin(),
         mVideoGenreGameShow.end(), CompareItem);
    sort(mVideoGenreHDTV.begin(),
         mVideoGenreHDTV.end(), CompareItem);
    sort(mVideoGenreHistory.begin(),
         mVideoGenreHistory.end(), CompareItem);
    sort(mVideoGenreHomeVideo.begin(),
         mVideoGenreHomeVideo.end(), CompareItem);
    sort(mVideoGenreHorror.begin(),
         mVideoGenreHorror.end(), CompareItem);
    sort(mVideoGenreMovie.begin(),
         mVideoGenreMovie.end(), CompareItem);
    sort(mVideoGenreMusic.begin(),
         mVideoGenreMusic.end(), CompareItem);
    sort(mVideoGenreMusical.begin(),
         mVideoGenreMusical.end(), CompareItem);
    sort(mVideoGenreMystery.begin(),
         mVideoGenreMystery.end(), CompareItem);
    sort(mVideoGenreNews.begin(), 
         mVideoGenreNews.end(), CompareItem);
    sort(mVideoGenreRealityTV.begin(),
         mVideoGenreRealityTV.end(), CompareItem);
    sort(mVideoGenreRomance.begin(),
         mVideoGenreRomance.end(), CompareItem);
    sort(mVideoGenreSciFi.begin(),
         mVideoGenreSciFi.end(), CompareItem);
    sort(mVideoGenreShort.begin(),
         mVideoGenreShort.end(), CompareItem);
    sort(mVideoGenreSitcom.begin(),
         mVideoGenreSitcom.end(), CompareItem);
    sort(mVideoGenreSoap.begin(),
         mVideoGenreSoap.end(), CompareItem);
    sort(mVideoGenreSport.begin(),
         mVideoGenreSport.end(), CompareItem);
    sort(mVideoGenreSuspense.begin(),
         mVideoGenreSuspense.end(), CompareItem);
    sort(mVideoGenreTalkShow.begin(),
         mVideoGenreTalkShow.end(), CompareItem);
    sort(mVideoGenreThriller.begin(),
         mVideoGenreThriller.end(), CompareItem);
    sort(mVideoGenreWar.begin(),
         mVideoGenreWar.end(), CompareItem);
    sort(mVideoGenreWestern.begin(),
         mVideoGenreWestern.end(), CompareItem);
    sort(mVideoGenreOther.begin(), 
         mVideoGenreOther.end(), CompareItem);

    WriteFiles();
    
    // exit right away
    Terminate(0);

    Log(7, "SDeskScan::OnAppInit() done\n");
}

void SDeskScan::OnAppExit()
{
    Log(7, "SDeskScan::OnAppExit()\n");
}

void SDeskScan::OnActivate()
{
}

void SDeskScan::OnKeyDown(
    u32 nKey)
{
    Log(7, "SDeskScan::OnKeyDown() key %d\n", nKey);
}

void SDeskScan::OnMountMessage(
    const CascadeMountMessage& mountMessage)
{
    Log(7, "SDeskScan::OnMountMessage()\n");

    Log(7, "SDeskScan::OnMountMessage() done\n");
}

void SDeskScan::ScanDirectories(
    char *path)
{
    int pos = strlen(path);
    unsigned int extPos = 0;
    char parentPath[1024];
    struct stat filestat;

    // Its a directory... show the content.
    SDeskDirScanner dir(path);

    Log(7, "11SDeskScan::ScanDirectories(%s) scan\n", path);

    strcpy(parentPath, path);
    while(pos > 0) {
        if (parentPath[pos] == '/') {
            parentPath[pos] = 0;
            break;
        }
        parentPath[pos] = 0;
        pos--;
    }

	for(u32 pos = 0; pos < dir.GetNumEntries(); ++pos) {
		char buffer[1024];
		const char *entry = dir.GetEntry(pos);

		if (!entry) {
			continue;
        }

        // See if we need to exclude this entry
        Log(7, "11SDeskScan::ScanDirectories(%s) entry\n", entry);
        if (IsExcluded(entry)) {
            continue;
        }

        sprintf(buffer, "%s/%s", path, entry);
        Log(7, "11SDeskScan::ScanDirectories(%s) main scan\n", buffer);
        if (stat(buffer, &filestat) == 0 && S_ISDIR(filestat.st_mode)) {
            if (mScanType == S_ALL_FLAT && strcmp(entry, "VIDEO_TS") == 0) {
                mVideoDirsTmp.insert(mVideoDirsTmp.end(), 
                                     new string(path));
            }
            ScanDirectories(buffer);
		} else {
            Log(7, "22SDeskScan::ScanDirectories(%s) scan not dir\n", buffer);
            if (mMusicPlayer[0] == '\0' &&
                strcmp(entry, "sonica") == 0) {
                Log(7, "SDeskScan::ScanDirectories(%s/%s) found sonica\n", 
                    path, entry);
                strcpy(mMusicPlayer, path);
                strcat(mMusicPlayer, "/");
                strcat(mMusicPlayer, entry);
                if (mUseSonicaForRadio) {
                    strcpy(mRadioPlayer, path);
                    strcat(mRadioPlayer, "/");
                    strcat(mRadioPlayer, entry);
                }
            } else if (mRadioPlayer[0] == '\0' &&
                       strcmp(entry, "vlc.sdesk") == 0) {
                Log(7, "SDeskScan::ScanDirectories(%s/%s) found vlc\n", 
                    path, entry);
                if (mUseVLCForRadio) {
                    strcpy(mRadioPlayer, path);
                    strcat(mRadioPlayer, "/");
                    strcat(mRadioPlayer, entry);
                }
            } else if (mMPlay[0] == '\0' &&
                       strcmp(entry, "MPlay.app") == 0) {
                Log(7, "SDeskScan::ScanDirectories(%s/%s) found MPlay\n", 
                    path, entry);
                strcpy(mMPlay, path);
                strcat(mMPlay, "/");
                strcat(mMPlay, entry);
            } else if (mWeatherSix[0] == '\0' &&
                       (strcmp(entry, "WeatherSix.roku") == 0 ||
                        strcmp(entry, "WeatherSix") == 0)) {
                Log(7, "SDeskScan::ScanDirectories(%s/%s) found weathersix\n", 
                    path, entry);
                strcpy(mWeatherSix, path);
                strcat(mWeatherSix, "/");
                strcat(mWeatherSix, entry);
            } else if (strncmp(entry + strlen(entry) - 5, 
                               ".roku", 5) == 0) {
                if (strcmp(entry, "WeatherSix.roku") == 0 ||
                    strcmp(entry, "SDesk.roku") == 0 ||
                    strcmp(entry, "sonica_debug.roku") == 0) {
                    continue;
                }
                Log(7, "SDeskScan::ScanDirectories(%s/%s) found app\n", 
                    path, entry);
                strcpy(buffer, path);
                strcat(buffer, "/");
                strcat(buffer, entry);
                buffer[strlen(buffer) - 5] = 0;
                strcat(buffer, ".artpack");
                if (stat(buffer, &filestat) != 0) {
                    // This is not an artpack... add the app.
                    strcpy(buffer, path);
                    strcat(buffer, "/");
                    strcat(buffer, entry);
                    mAppsList.insert(mAppsList.end(), 
                                     new SDeskScanApp(entry, buffer, false));
                }
            } else if (strncmp(entry + strlen(entry) - 8, 
                               ".artpack", 8) == 0) {
                Log(7, "SDeskScan::ScanDirectories(%s/%s) found artpack\n", 
                    path, entry);
                strcpy(buffer, path);
                strcat(buffer, "/");
                strcat(buffer, entry);
                buffer[strlen(buffer) - 7] = 0;
                strcat(buffer, "roku");
                if (stat(buffer, &filestat) == 0) {
                    mSlideShowList.insert(mSlideShowList.end(), 
                                          new SDeskScanApp(entry, 
                                                           buffer,
                                                           false));
                }
            } else if (mScanPictures && IsPicture(entry, &extPos)) {
                if (mScanType == S_ALL_FLAT) {
                    bool addToList = true;
                    vector<string *>::const_iterator it, end =      
                        mPictureDirsTmp.end();
                    for(it = mPictureDirsTmp.begin(); it != end; it++) {
                        const char* tmpBuf = (*it)->c_str();
                        if (strncmp(tmpBuf, path, strlen(tmpBuf)) == 0) {
                            addToList = false;
                            break;
                        }
                    }
                    if (addToList) {
                        Log(7, "33SDeskScan::ScanDirectories(%s) add flat\n", 
                            path);
                        mPictureDirsTmp.insert(mPictureDirsTmp.end(), 
                                               new string(path));
                    }
                } else {
                    bool addToList = true;
                    vector<string *>::const_iterator it, end =      
                        mPictureDirsTmp.end();
                    for(it = mPictureDirsTmp.begin(); it != end; it++) {
                        const char* tmpBuf = (*it)->c_str();
                        if (strncmp(tmpBuf, parentPath, strlen(tmpBuf)) == 0) {
                            addToList = false;
                            break;
                        }
                    }
                    if (addToList) {
                        Log(7, "44SDeskScan::ScanDirectories(%s) add\n",
                            parentPath);
                        mPictureDirsTmp.insert(mPictureDirsTmp.end(), 
                                               new string(parentPath));
                    }
                }
            } else if (mScanVideos && IsVideo(entry, &extPos)) {
                if (mScanType == S_ALL_FLAT) {
                    bool addToList = true;
                    vector<string *>::const_iterator it, end =
                        mVideoDirsTmp.end();
                    for(it = mVideoDirsTmp.begin(); it != end; it++) {
                        const char* tmpBuf = (*it)->c_str();
                        if (strncmp(tmpBuf, path, strlen(tmpBuf)) == 0) {
                            addToList = false;
                            break;
                        }
                    }
                    if (addToList) {
                        Log(7, "55SDeskScan::ScanDirectories(%s) add flat\n",
                            path);
                        mVideoDirsTmp.insert(mVideoDirsTmp.end(), 
                                             new string(path));
                    }
                } else {
                    bool addToList = true;
                    vector<string *>::const_iterator it, end =
                        mVideoDirsTmp.end();
                    for(it = mVideoDirsTmp.begin(); it != end; it++) {
                        const char* tmpBuf = (*it)->c_str();
                        if (strncmp(tmpBuf, parentPath, strlen(tmpBuf)) == 0) {
                            addToList = false;
                            break;
                        }
                    }
                    if (addToList) {
                        Log(7, "66SDeskScan::ScanDirectories(%s) add\n",
                            parentPath);
                        mVideoDirsTmp.insert(mVideoDirsTmp.end(), 
                                             new string(parentPath));
                    }
                }
                ProcessInfoFile(buffer);
            } else if (mScanMusic && IsMusic(entry, &extPos)) {
                if (mScanType == S_ALL_FLAT) {
                    bool addToList = true;
                    vector<string *>::const_iterator it, end =
                        mMusicDirsTmp.end();
                    for(it = mMusicDirsTmp.begin(); it != end; it++) {
                        const char* tmpBuf = (*it)->c_str();
                        if (strncmp(tmpBuf, path, strlen(tmpBuf)) == 0) {
                            addToList = false;
                            break;
                        }
                    }
                    if (addToList) {
                        Log(7, "77SDeskScan::ScanDirectories(%s) add flat\n",
                            path);
                        mMusicDirsTmp.insert(mMusicDirsTmp.end(), 
                                             new string(path));
                    }
                } else {
                    bool addToList = true;
                    vector<string *>::const_iterator it, end =
                        mMusicDirsTmp.end();
                    for(it = mMusicDirsTmp.begin(); it != end; it++) {
                        const char* tmpBuf = (*it)->c_str();
                        if (strncmp(tmpBuf, parentPath, strlen(tmpBuf)) == 0) {
                            addToList = false;
                            break;
                        }
                    }
                    if (addToList) {
                        Log(7, "88SDeskScan::ScanDirectories(%s) add\n",
                            parentPath);
                        mMusicDirsTmp.insert(mMusicDirsTmp.end(), 
                                             new string(parentPath));
                    }
                }
                // Now handle the tag based lists.
                strcpy(buffer, path);
                strcat(buffer, "/");
                strcat(buffer, entry);
                AddSongByTag(entry, buffer);
            }
        }
	}
}

void SDeskScan::ProcessInfoFile(
    char *name)  
{
    char buf[1025];
    char entry[2060];
    char vobName[1024];
    char vobPath[1024];
    char *pos;
    char *slashPos;
    FILE *fd;

    vobName[0] = 0;

    // Ok... a little crude, but if the name ends with a .<number>.ext,
    // where <number> is larger than 1, we skip the file assuming it's
    // a sequence file.
    strcpy(buf, name);
    slashPos = strrchr(buf, '/');
    pos = strrchr(buf, '.');
    if (pos && slashPos < pos) {
        // found a file extension
        bool isFirstFile = true;

        *pos = 0;
        pos--;
        Log(7, "SDeskScan::ProcessInfoFile(%s) skip?\n", buf);
        if (pos > buf && *pos != '1') {
            // Not a 1 at the end... we are not the first file in the
            // sequence.
            isFirstFile = false;
            Log(7, "SDeskScan::ProcessInfoFile(%c) not first?\n", *pos);
        } else {
            if (pos > buf) {
                pos--;
            }
        }
        while(pos > buf && isdigit(*pos)) {
            Log(7, "SDeskScan::ProcessInfoFile(%c) char\n", *pos);
            if (*pos != '0') {
                // Not a leading 0... not the first file in the sequence.
                Log(7, "SDeskScan::ProcessInfoFile(%c) not first?\n", *pos);
                isFirstFile = false;
            }
            pos--;
        }
        Log(7, "SDeskScan::ProcessInfoFile(%c) dot?\n", *pos);
        if (*pos == '.' && !isFirstFile) {
            Log(7, "SDeskScan::ProcessInfoFile() dont add\n");
            // We are a sequence file, and we are not the first... so jsut
            // return without adding.
            return;
        }
    }

    Log(7, "SDeskScan::ProcessInfoFile(%s)\n", name);
    strcpy(buf, name);
    slashPos = strrchr(buf, '/');
    pos = strrchr(buf, '.');
    if (pos && slashPos < pos) {
        // found a file extension
        *pos = 0;
    }
    strcat(buf, ".nfo");
    Log(7, "SDeskScan::ProcessInfoFile(%s)\n", buf);
    if ((fd = fopen(buf, "r")) == NULL) {
        strcpy(buf, name);
        slashPos = strrchr(buf, '/');
        pos = strrchr(buf, '.');
        if (pos && slashPos < pos) {
            // found a file extension
            *pos = 0;
        }
        strcat(buf, ".my");
        Log(7, "SDeskScan::ProcessInfoFile(%s)\n", buf);
        if ((fd = fopen(buf, "r")) == NULL) {
            strcpy(buf, name);
            slashPos = strrchr(buf, '/');
            pos = strrchr(buf, '.');
            if (pos && slashPos < pos) {
                // found a file extension
                *pos = 0;
            }
            strcat(buf, ".txt");
            Log(7, "SDeskScan::ProcessInfoFile(%s)\n", buf);
            if ((fd = fopen(buf, "r")) == NULL) {
                // Look for the directory name plus .nfo.
                strcpy(buf, name);
                pos = strrchr(buf, '/');
                if (pos) {
                    // found a path
                    strcpy(vobName, pos + 1);
                    *pos = 0;
                    pos = strrchr(buf, '/');
                    if (pos) {
                        if (strcmp(pos, "/VIDEO_TS") == 0) {
                            strcpy(vobPath, buf);
                            *pos = 0;
                        }
                    }
                }
                strcat(buf, ".nfo");
                Log(7, "SDeskScan::ProcessInfoFile(%s)\n", buf);
                if ((fd = fopen(buf, "r")) == NULL) {
                    strcpy(buf, name);
                    pos = strrchr(buf, '/');
                    if (pos) {
                        // found a path
                        strcpy(vobName, pos + 1);
                        *pos = 0;
                        pos = strrchr(buf, '/');
                        if (pos) {
                            if (strcmp(pos, "/VIDEO_TS") == 0) {
                                strcpy(vobPath, buf);
                                *pos = 0;
                            }
                        }
                    }
                    strcat(buf, ".my");
                    Log(7, "SDeskScan::ProcessInfoFile(%s)\n", buf);
                    if ((fd = fopen(buf, "r")) == NULL) {
                        strcpy(buf, name);
                        pos = strrchr(buf, '/');
                        if (pos) {
                            // found a path
                            strcpy(vobName, pos + 1);
                            *pos = 0;
                            pos = strrchr(buf, '/');
                            if (pos) {
                                if (strcmp(pos, "/VIDEO_TS") == 0) {
                                    strcpy(vobPath, buf);
                                    *pos = 0;
                                }
                            }
                        }
                        strcat(buf, ".txt");
                        Log(7, "SDeskScan::ProcessInfoFile(%s)\n", buf);
                        if ((fd = fopen(buf, "r")) == NULL) {
                            // Look for the directory name plus folder.nfo.
                            strcpy(buf, name);
                            pos = strrchr(buf, '/');
                            if (pos) {
                                // found a path
                                strcpy(vobName, pos + 1);
                                *pos = 0;
                                pos = strrchr(buf, '/');
                                if (pos) {
                                    if (strcmp(pos, "/VIDEO_TS") == 0) {
                                        strcpy(vobPath, buf);
                                        *pos = 0;
                                    }
                                }
                            }
                            strcat(buf, "/folder.nfo");
                            Log(7, "SDeskScan::ProcessInfoFile(%s)\n", buf);
                            if ((fd = fopen(buf, "r")) == NULL) {
                                strcpy(buf, name);
                                pos = strrchr(buf, '/');
                                if (pos) {
                                    // found a path
                                    strcpy(vobName, pos + 1);
                                    *pos = 0;
                                    pos = strrchr(buf, '/');
                                    if (pos) {
                                        if (strcmp(pos, "/VIDEO_TS") == 0) {
                                            strcpy(vobPath, buf);
                                            *pos = 0;
                                        }
                                    }
                                }
                                strcat(buf, "/folder.my");
                                Log(7, "SDeskScan::ProcessInfoFile(%s)\n", buf);
                                if ((fd = fopen(buf, "r")) == NULL) {
                                    strcpy(buf, name);
                                    pos = strrchr(buf, '/');
                                    if (pos) {
                                        // found a path
                                        strcpy(vobName, pos + 1);
                                        *pos = 0;
                                        pos = strrchr(buf, '/');
                                        if (pos) {
                                            if (strcmp(pos, "/VIDEO_TS") == 0) {
                                                strcpy(vobPath, buf);
                                                *pos = 0;
                                            }
                                        }
                                    }
                                    strcat(buf, "/folder.txt");
                                    Log(7, "SDeskScan::ProcessInfoFile(%s)\n", buf);
                                    if ((fd = fopen(buf, "r")) == NULL) {
                                        Log(8, "fopen failed (%s)...\n", buf);
                                        return;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Check whether our file name is the beginning of the largets
    // vob sequence.
    if (vobName[0]) {
        char buf2[1024];
        char buf3[1024];
        char buf4[1024];
        u64 largestSize = 0;
        u64 largestFileSize = 0;
        int largestSequence = -1;
        struct stat filestat;

        sprintf(buf2, "%s/", vobPath);
        SDeskDirScanner *dir = new SDeskDirScanner(buf2, "VTS");
        for (int sequenceNum = 0; ; ++sequenceNum) {
            // Go through the sequences...
            u64 sequenceSize = 0;

            delete dir;
            sprintf(buf3, "VTS_%02d", sequenceNum);
            dir = new SDeskDirScanner(buf2, buf3);

            if (dir->GetNumEntries() == 0 && sequenceNum > 10) {
                // No more entries that match
                delete dir;
                break;
            }

            for (u32 pos2 = 0; pos2 < dir->GetNumEntries(); ++pos2) {
                // Go through all items in the directory.
                const char *entry2 = dir->GetEntry(pos2);
            
                if (!entry2) {
                    // Not a valid entry.
                    continue;
                }
                if (strncmp(entry2 + strlen(entry2) - 4, ".vob", 4) == 0 ||
                    strncmp(entry2 + strlen(entry2) - 4, ".VOB", 4) == 0) {
                    sequenceSize += dir->GetSize();
                }
                if (largestFileSize < dir->GetSize()) {
                    strcpy(buf4, entry2);
                    largestFileSize = dir->GetSize();
                }
            }
            if (largestSize < sequenceSize) {
                // This sequence is larger than the previous one.
                largestSize = sequenceSize;
                largestSequence = sequenceNum;
            }
        }

        // TODO... compare agains current name!
        if (largestSequence != -1) {
            sprintf(buf3, "%sVTS_%02d_1.VOB", buf2, largestSequence);
            if (stat(buf3, &filestat) != 0) {
                sprintf(buf3, "%sVTS_%02d_2.VOB", buf2, largestSequence);
                if (stat(buf3, &filestat) != 0) {
                    sprintf(buf3, "%s%s", buf2, buf4);
                    if (stat(buf3, &filestat) != 0) {
                        return;
                    } else {
                        if (strcmp(buf3, name) != 0) {
                            // Not the right vob
                            return;
                        }
                    }
                } else {
                    if (strcmp(buf3, name) != 0) {
                        // Not the right vob
                        return;
                    }
                }
            } else {
                if (strcmp(buf3, name) != 0) {
                    // Not the right vob
                    return;
                }
            }
        }
    }

    entry[0] = 0;
    while ((pos = fgets(buf, 1023, fd)) != 0) {
        buf[strlen(buf) - 1] = 0;
        if (buf[strlen(buf) - 1] == '\n' ||
            buf[strlen(buf) - 1] == '\r') {
            buf[strlen(buf) - 1] = 0;
        }
        
        if (strncmp(pos, "title=", 6) == 0 ||
            strncmp(pos, "TVEpisode=", 10) == 0) {
            pos = strchr(pos, '=');
            pos++;
            strcpy(entry, pos);
            strcat(entry, "\n");
            strcat(entry, name);
        }
        if (strncmp(pos, "genre=", 6) == 0 ||
                   strncmp(pos, "TVGenre=", 8) == 0) {
            bool addedSomewhere = false;
            pos = strchr(pos, '=');
            pos++;
            if (strstr(pos, "Action") != 0) {
                addedSomewhere = true;
                mVideoGenreAction.insert(mVideoGenreAction.end(),
                                         new string(entry));
            }
            if (strstr(pos, "Adventure") != 0) {
                addedSomewhere = true;
                mVideoGenreAdventure.insert(mVideoGenreAdventure.end(),
                                            new string(entry));
            }
            if (strstr(pos, "Animation") != 0 ||
                strstr(pos, "Animated") != 0) {
                addedSomewhere = true;
                mVideoGenreAnimation.insert(mVideoGenreAnimation.end(),
                                            new string(entry));
            }
            if (strstr(pos, "Biography") != 0) {
                addedSomewhere = true;
                mVideoGenreBiography.insert(mVideoGenreBiography.end(),
                                            new string(entry));
            }
            if (strstr(pos, "Children") != 0) {
                addedSomewhere = true;
                mVideoGenreChildren.insert(mVideoGenreChildren.end(),
                                           new string(entry));
            }
            if (strstr(pos, "Comedy") != 0) {
                addedSomewhere = true;
                mVideoGenreComedy.insert(mVideoGenreComedy.end(),
                                         new string(entry));
            }
            if (strstr(pos, "Crime") != 0) {
                addedSomewhere = true;
                mVideoGenreCrime.insert(mVideoGenreCrime.end(),
                                        new string(entry));
            }
            if (strstr(pos, "Documentary") != 0) {
                addedSomewhere = true;
                mVideoGenreDocumentary.insert(mVideoGenreDocumentary.end(),
                                              new string(entry));
            }
            if (strstr(pos, "Drama") != 0) {
                addedSomewhere = true;
                mVideoGenreDrama.insert(mVideoGenreDrama.end(),
                                        new string(entry));
            }
            if (strstr(pos, "Family") != 0) {
                addedSomewhere = true;
                mVideoGenreFamily.insert(mVideoGenreFamily.end(),
                                         new string(entry));
            }
            if (strstr(pos, "Fantasy") != 0) {
                addedSomewhere = true;
                mVideoGenreFantasy.insert(mVideoGenreFantasy.end(),
                                          new string(entry));
            }
            if (strstr(pos, "Film-Noir") != 0) {
                addedSomewhere = true;
                mVideoGenreFilmNoir.insert(mVideoGenreFilmNoir.end(),
                                           new string(entry));
            }
            if (strstr(pos, "Game-Show") != 0) {
                addedSomewhere = true;
                mVideoGenreGameShow.insert(mVideoGenreGameShow.end(),
                                           new string(entry));
            }
            if (strstr(pos, "HDTV") != 0) {
                addedSomewhere = true;
                mVideoGenreHDTV.insert(mVideoGenreHDTV.end(),
                                       new string(entry));
            }
            if (strstr(pos, "History") != 0) {
                addedSomewhere = true;
                mVideoGenreHistory.insert(mVideoGenreHistory.end(),
                                          new string(entry));
            }
            if (strstr(pos, "Home Video") != 0) {
                addedSomewhere = true;
                mVideoGenreHomeVideo.insert(mVideoGenreHomeVideo.end(),
                                            new string(entry));
            }
            if (strstr(pos, "Horror") != 0) {
                addedSomewhere = true;
                mVideoGenreHorror.insert(mVideoGenreHorror.end(),
                                         new string(entry));
            }
            if (strstr(pos, "Movie") != 0) {
                addedSomewhere = true;
                mVideoGenreMovie.insert(mVideoGenreMovie.end(),
                                        new string(entry));
            }
            if (strstr(pos, "Music") != 0 && strstr(pos, "Musical") == 0) {
                addedSomewhere = true;
                mVideoGenreMusic.insert(mVideoGenreMusic.end(),
                                        new string(entry));
            }
            if (strstr(pos, "Musical") != 0) {
                addedSomewhere = true;
                mVideoGenreMusical.insert(mVideoGenreMusical.end(),
                                          new string(entry));
            }
            if (strstr(pos, "Mystery") != 0) {
                addedSomewhere = true;
                mVideoGenreMystery.insert(mVideoGenreMystery.end(),
                                          new string(entry));
            }
            if (strstr(pos, "News") != 0) {
                addedSomewhere = true;
                mVideoGenreNews.insert(mVideoGenreNews.end(),
                                       new string(entry));
            }
            if (strstr(pos, "Reality") != 0) {
                addedSomewhere = true;
                mVideoGenreRealityTV.insert(mVideoGenreRealityTV.end(),
                                            new string(entry));
            }
            if (strstr(pos, "Romance") != 0) {
                addedSomewhere = true;
                mVideoGenreRomance.insert(mVideoGenreRomance.end(),
                                          new string(entry));
            }
            if (strstr(pos, "Sci-Fi") != 0) {
                addedSomewhere = true;
                mVideoGenreSciFi.insert(mVideoGenreSciFi.end(),
                                        new string(entry));
            }
            if (strstr(pos, "Short") != 0) {
                addedSomewhere = true;
                mVideoGenreShort.insert(mVideoGenreShort.end(),
                                        new string(entry));
            }
            if (strstr(pos, "Sitcom") != 0) {
                addedSomewhere = true;
                mVideoGenreSitcom.insert(mVideoGenreSitcom.end(),
                                         new string(entry));
            }
            if (strstr(pos, "Soap") != 0) {
                addedSomewhere = true;
                mVideoGenreSoap.insert(mVideoGenreSoap.end(),
                                       new string(entry));
            }
            if (strstr(pos, "Sport") != 0) {
                addedSomewhere = true;
                mVideoGenreSport.insert(mVideoGenreSport.end(),
                                        new string(entry));
            }
            if (strstr(pos, "Suspense") != 0) {
                addedSomewhere = true;
                mVideoGenreSuspense.insert(mVideoGenreSuspense.end(),
                                           new string(entry));
            }
            if (strstr(pos, "Talk-Show") != 0) {
                addedSomewhere = true;
                mVideoGenreTalkShow.insert(mVideoGenreTalkShow.end(),
                                           new string(entry));
            }
            if (strstr(pos, "Thriller") != 0) {
                addedSomewhere = true;
                mVideoGenreThriller.insert(mVideoGenreThriller.end(),
                                           new string(entry));
            }
            if (strstr(pos, "War") != 0) {
                addedSomewhere = true;
                mVideoGenreWar.insert(mVideoGenreWar.end(),
                                      new string(entry));
            }
            if (strstr(pos, "Western") != 0) {
                addedSomewhere = true;
                mVideoGenreWestern.insert(mVideoGenreWestern.end(),
                                      new string(entry));
            }
            if (!addedSomewhere) {
                mVideoGenreOther.insert(mVideoGenreOther.end(),
                                        new string(entry));
            }
        }
    }

    fclose(fd);
}

void SDeskScan::WriteFiles()
{
    char fileName[1024];
    char saveFileName[1024];
    char buffer[1024];
    char buf2[1024];
    char buf3[1024];
    const char* namePos;
    const char* tailName;
    FILE *fd;
    struct stat filestat;

    Log(7, "SDeskScan::WriteFiles()\n");

    strcpy(fileName, mRootDir);
    strcat(fileName, "/menus");
    if (stat(fileName, &filestat) != 0) {
        if (mkdir(fileName, 0777) != 0) {
            Log(1, "mkdir failed (%s)...\n", fileName);
        }
    }

    strcpy(fileName, mRootDir);
    strcat(fileName, "/menus/Radio");
    if (stat(fileName, &filestat) != 0) {
        if (mkdir(fileName, 0777) != 0) {
            Log(1, "mkdir failed (%s)...\n", fileName);
        }
    }

    strcpy(fileName, mRootDir);
    strcat(fileName, "/menus/Music");
    if (stat(fileName, &filestat) != 0) {
        if (mkdir(fileName, 0777) != 0) {
            Log(1, "mkdir failed (%s)...\n", fileName);
        }
    }

    if (mScanType == S_ALL || mScanType == S_ALL_FLAT) {
        // Write the main setup file.
        strcpy(fileName, mRootDir);
        strcat(fileName, "/SDesk.cfg");
        strcpy(saveFileName, mRootDir);
        strcat(saveFileName, "/SDesk.cfg.save");

        if (stat(fileName, &filestat) != 0) {
            Log(7, "SDeskScan::WriteFiles(%s)\n", fileName);
            if ((fd = fopen(fileName, "w+")) == NULL) {
                Log(1, "fopen failed (%s)...\n", fileName);
            } else {
                if (mMPlay[0] != '\0') {
                    fwrite(MPLAY, sizeof(char), strlen(MPLAY), fd);
                    fwrite(mMPlay, sizeof(char), strlen(mMPlay), fd);
                }
                if (mRadioPlayer[0] != '\0') {
                    fwrite(RADIOPLAYER, sizeof(char), strlen(RADIOPLAYER), fd);
                    fwrite(mRadioPlayer, sizeof(char), strlen(mRadioPlayer), fd);
                }
                if (mMusicPlayer[0] != '\0') {
                    fwrite(MUSICPLAYER, sizeof(char), strlen(MUSICPLAYER), fd);
                    fwrite(mMusicPlayer, sizeof(char), strlen(mMusicPlayer), fd);
                }
                if (mPictureViewer[0] != '\0') {
                    fwrite(PICTUREVIEWER, sizeof(char), strlen(PICTUREVIEWER), fd);
                    fwrite(mPictureViewer, sizeof(char), strlen(mPictureViewer), fd);
                }
                if (mCustomInfo[0] != '\0') {
                    fwrite(CUSTOM_INFO, sizeof(char), strlen(CUSTOM_INFO), fd);
                    fwrite(mCustomInfo, sizeof(char), strlen(mCustomInfo), fd);
                }
                if (mCustomRotate[0] != '\0') {
                    fwrite(CUSTOM_ROTATE, sizeof(char), strlen(CUSTOM_ROTATE), fd);
                    fwrite(mCustomRotate, sizeof(char), strlen(mCustomRotate), fd);
                }
                if (mCustomFF[0] != '\0') {
                    fwrite(CUSTOM_FF, sizeof(char), strlen(CUSTOM_FF), fd);
                    fwrite(mCustomFF, sizeof(char), strlen(mCustomFF), fd);
                }
                if (mCustomFB[0] != '\0') {
                    fwrite(CUSTOM_FB, sizeof(char), strlen(CUSTOM_FB), fd);
                    fwrite(mCustomFB, sizeof(char), strlen(mCustomFB), fd);
                }
                if (mNoSecurity) {
                    fwrite(NO_SECURITY, sizeof(char), strlen(NO_SECURITY), fd);
                } else {
                    fwrite(DO_SECURITY, sizeof(char), strlen(DO_SECURITY), fd);
                }
                fwrite(SDESK_CFG, sizeof(char), strlen(SDESK_CFG), fd);
                vector<string *>::const_iterator it,
                    end = mMusicExtension.end();
                for(it = mMusicExtension.begin(); it != end; it++) {
                    fwrite(MUSIC_EXTS, sizeof(char), 
                           strlen(MUSIC_EXTS), fd);
                    strcpy(buffer, (*it)->c_str());
                    strcat(buffer, "\n");
                    fwrite(buffer, sizeof(char), strlen(buffer), fd);
                }
                end = mPictureExtension.end();
                for(it = mPictureExtension.begin(); it != end; it++) {
                    fwrite(PICTURE_EXTS, sizeof(char), 
                           strlen(PICTURE_EXTS), fd);
                    strcpy(buffer, (*it)->c_str());
                    strcat(buffer, "\n");
                    fwrite(buffer, sizeof(char), strlen(buffer), fd);
                }
                end = mVideoExtension.end();
                for(it = mVideoExtension.begin(); it != end; it++) {
                    fwrite(VIDEO_EXTS, sizeof(char), 
                           strlen(VIDEO_EXTS), fd);
                    strcpy(buffer, (*it)->c_str());
                    strcat(buffer, "\n");
                    fwrite(buffer, sizeof(char), strlen(buffer), fd);
                }
                if (mStatus1Int > 0) {
                    sprintf(buffer, "%d\n", mStatus1Int);
                    fwrite("status1\n", sizeof(char), 8, fd);
                    fwrite(mStatus1Cmd, sizeof(char), strlen(mStatus1Cmd), fd);
                    fwrite("\nstatus1interval\n", sizeof(char), 17, fd);
                    fwrite(buffer, sizeof(char), strlen(buffer), fd);
                }
                if (mStatus2Int > 0) {
                    sprintf(buffer, "%d\n", mStatus2Int);
                    fwrite("status2\n", sizeof(char), 8, fd);
                    fwrite(mStatus2Cmd, sizeof(char), strlen(mStatus2Cmd), fd);
                    fwrite("\nstatus2interval\n", sizeof(char), 17, fd);
                    fwrite(buffer, sizeof(char), strlen(buffer), fd);
                }
                if (mStatus3Int > 0) {
                    sprintf(buffer, "%d\n", mStatus3Int);
                    fwrite("status3\n", sizeof(char), 8, fd);
                    fwrite(mStatus3Cmd, sizeof(char), strlen(mStatus3Cmd), fd);
                    fwrite("\nstatus3interval\n", sizeof(char), 17, fd);
                    fwrite(buffer, sizeof(char), strlen(buffer), fd);
                }
                if (mStatus4Int > 0) {
                    sprintf(buffer, "%d\n", mStatus4Int);
                    fwrite("status4\n", sizeof(char), 8, fd);
                    fwrite(mStatus4Cmd, sizeof(char), strlen(mStatus4Cmd), fd);
                    fwrite("\nstatus4interval\n", sizeof(char), 17, fd);
                    fwrite(buffer, sizeof(char), strlen(buffer), fd);
                }
                if (mFavoriteAll.size() == 0 && !mDefaultFavorites) {
                    fwrite("favorite\n", sizeof(char), 9, fd);
                    fwrite("%misc\n", sizeof(char), 6, fd);
                    fwrite("Add To Favorites\n", sizeof(char), 17, fd);
                    fwrite("menus/MyFavorites.mnu\n", sizeof(char), 22, fd);
                }
                end = mFavoriteAll.end();
                for(it = mFavoriteAll.begin(); it != end; it++) {
                    strcpy(buf2, (*it)->c_str());
                    FixFileName(buf2);
                    strcpy(buffer, mRootDir);
                    strcat(buffer, "/menus/");
                    strcat(buffer, buf2);
                    strcat(buffer, ".mnu");
                    fwrite("favorite\n", sizeof(char), 9, fd);
                    fwrite("%all\n", sizeof(char), 5, fd);
                    fwrite("Add To ", sizeof(char), 7, fd);
                    fwrite((*it)->c_str(), sizeof(char),
                           strlen((*it)->c_str()), fd);
                    fwrite("\n", sizeof(char), 1, fd);
                    fwrite(buffer, sizeof(char), strlen(buffer), fd);
                    fwrite("\n", sizeof(char), 1, fd);
                }
                end = mFavoriteSongs.end();
                for(it = mFavoriteSongs.begin(); it != end; it++) {
                    strcpy(buf2, (*it)->c_str());
                    FixFileName(buf2);
                    strcpy(buffer, mRootDir);
                    strcat(buffer, "/menus/");
                    strcat(buffer, buf2);
                    strcat(buffer, ".mnu");
                    fwrite("favorite\n", sizeof(char), 9, fd);
                    fwrite("%songs\n", sizeof(char), 7, fd);
                    fwrite("Add To ", sizeof(char), 7, fd);
                    fwrite((*it)->c_str(), sizeof(char),
                           strlen((*it)->c_str()), fd);
                    fwrite("\n", sizeof(char), 1, fd);
                    fwrite(buffer, sizeof(char), strlen(buffer), fd);
                    fwrite("\n", sizeof(char), 1, fd);
                }
                end = mFavoritePictures.end();
                for(it = mFavoritePictures.begin(); it != end; it++) {
                    strcpy(buf2, (*it)->c_str());
                    FixFileName(buf2);
                    strcpy(buffer, mRootDir);
                    strcat(buffer, "/menus/");
                    strcat(buffer, buf2);
                    strcat(buffer, ".mnu");
                    fwrite("favorite\n", sizeof(char), 9, fd);
                    fwrite("%pictures\n", sizeof(char), 10, fd);
                    fwrite("Add To ", sizeof(char), 7, fd);
                    fwrite((*it)->c_str(), sizeof(char),
                           strlen((*it)->c_str()), fd);
                    fwrite("\n", sizeof(char), 1, fd);
                    fwrite(buffer, sizeof(char), strlen(buffer), fd);
                    fwrite("\n", sizeof(char), 1, fd);
                }
                end = mFavoriteVideos.end();
                for(it = mFavoriteVideos.begin(); it != end; it++) {
                    strcpy(buf2, (*it)->c_str());
                    FixFileName(buf2);
                    strcpy(buffer, mRootDir);
                    strcat(buffer, "/menus/");
                    strcat(buffer, buf2);
                    strcat(buffer, ".mnu");
                    fwrite("favorite\n", sizeof(char), 9, fd);
                    fwrite("%movies\n", sizeof(char), 8, fd);
                    fwrite("Add To ", sizeof(char), 7, fd);
                    fwrite((*it)->c_str(), sizeof(char),
                           strlen((*it)->c_str()), fd);
                    fwrite("\n", sizeof(char), 1, fd);
                    fwrite(buffer, sizeof(char), strlen(buffer), fd);
                    fwrite("\n", sizeof(char), 1, fd);
                }
                if (mRadioPlayer[0] != '\0') {
                    end = mFavoriteRadios.end();
                    for(it = mFavoriteRadios.begin(); it != end; it++) {
                        strcpy(buf2, (*it)->c_str());
                        FixFileName(buf2);
                        strcpy(buffer, mRootDir);
                        strcat(buffer, "/menus/");
                        strcat(buffer, buf2);
                        strcat(buffer, ".mnu");
                        fwrite("favorite\n", sizeof(char), 9, fd);
                        fwrite("%radios\n", sizeof(char), 8, fd);
                        fwrite("Add To ", sizeof(char), 7, fd);
                        fwrite((*it)->c_str(), sizeof(char),
                               strlen((*it)->c_str()), fd);
                        fwrite("\n", sizeof(char), 1, fd);
                        fwrite(buffer, sizeof(char), strlen(buffer), fd);
                        fwrite("\n", sizeof(char), 1, fd);
                    }
                }
                end = mFavoriteSlideShows.end();
                for(it = mFavoriteSlideShows.begin(); it != end; it++) {
                    strcpy(buf2, (*it)->c_str());
                    FixFileName(buf2);
                    strcpy(buffer, mRootDir);
                    strcat(buffer, "/menus/");
                    strcat(buffer, buf2);
                    strcat(buffer, ".mnu");
                    fwrite("favorite\n", sizeof(char), 9, fd);
                    fwrite("%slideshow\n", sizeof(char), 11, fd);
                    fwrite("Add To ", sizeof(char), 7, fd);
                    fwrite((*it)->c_str(), sizeof(char),
                           strlen((*it)->c_str()), fd);
                    fwrite("\n", sizeof(char), 1, fd);
                    fwrite(buffer, sizeof(char), strlen(buffer), fd);
                    fwrite("\n", sizeof(char), 1, fd);
                }
                end = mExcludedList.end();
                for(it = mExcludedList.begin(); it != end; it++) {
                    fwrite("excluded\n", sizeof(char), 9, fd);
                    fwrite((*it)->c_str(), sizeof(char),
                           strlen((*it)->c_str()), fd);
                    fwrite("\n", sizeof(char), 1, fd);
                }
                fclose(fd);
            }
        }
        
        // Write the setup file.
        strcpy(fileName, mRootDir);
        strcat(fileName, "/menus/Setup.mnu");
        if (SDeskScan::getInstance().DoBackup()) {
            strcpy(saveFileName, mRootDir);
            strcat(saveFileName, "/menus/Setup.mnu.save");
            rename(fileName, saveFileName);
        }

        Log(7, "SDeskScan::WriteFiles(%s)\n", fileName);
        if ((fd = fopen(fileName, "w+")) == NULL) {
            Log(1, "fopen failed (%s)...\n", fileName);
        } else {
            fwrite(SETUP, sizeof(char), strlen(SETUP), fd);
            fclose(fd);
        }
        
        // Write the radio rescan file.
        strcpy(fileName, mRootDir);
        strcat(fileName, "/menus/RescanRadio.mnu");
        if (SDeskScan::getInstance().DoBackup()) {
            strcpy(saveFileName, mRootDir);
            strcat(saveFileName, "/menus/RescanRadio.mnu.save");
            rename(fileName, saveFileName);
        }

        Log(7, "SDeskScan::WriteFiles(%s)\n", fileName);
        if ((fd = fopen(fileName, "w+")) == NULL) {
            Log(1, "fopen failed (%s)...\n", fileName);
        } else {
            fwrite(RESCAN_RADIO, sizeof(char), strlen(RESCAN_RADIO), fd);
            fclose(fd);
        }
        
        // Write the MyMusic file.
        strcpy(fileName, mRootDir);
        strcat(fileName, "/menus/MyMusic.mnu");
        if (SDeskScan::getInstance().DoBackup()) {
            strcpy(saveFileName, mRootDir);
            strcat(saveFileName, "/menus/MyMusic.mnu.save");
            rename(fileName, saveFileName);
        }

        Log(7, "SDeskScan::WriteFiles(%s)\n", fileName);
        if ((fd = fopen(fileName, "w+")) == NULL) {
            Log(1, "fopen failed (%s)...\n", fileName);
        } else {
            vector<string *>::const_iterator it,
                end = mFavoriteSongs.end();
            for(it = mFavoriteSongs.begin(); it != end; it++) {
                strcpy(buf2, (*it)->c_str());
                FixFileName(buf2);
                strcpy(buffer, mRootDir);
                strcat(buffer, "/menus/");
                strcat(buffer, buf2);
                strcat(buffer, ".mnu");
                fwrite("%menu\n", sizeof(char), 6, fd);
                fwrite("favorites\n", sizeof(char), 10, fd);
                fwrite((*it)->c_str(), sizeof(char),
                       strlen((*it)->c_str()), fd);
                fwrite("\n", sizeof(char), 1, fd);
                fwrite(buffer, sizeof(char), strlen(buffer), fd);
                fwrite("\n0\n", sizeof(char), 3, fd);
            }
            fwrite("%space\n", sizeof(char), 7, fd);
            fwrite("15\n", sizeof(char), 3, fd);
            fwrite("0\n", sizeof(char), 2, fd);
            sprintf(buffer, "%s/menus/Music/ByGenre.mnu", mRootDir);
            fwrite("%menu\n", sizeof(char), 6, fd);
            fwrite("songs\n", sizeof(char), 6, fd);
            fwrite("By Genre\n", sizeof(char), 9, fd);
            fwrite(buffer, sizeof(char), strlen(buffer), fd);
            fwrite("\n0\n", sizeof(char), 3, fd);
            sprintf(buffer, "%s/menus/Music/ByAlbum.mnu", mRootDir);
            fwrite("%menu\n", sizeof(char), 6, fd);
            fwrite("songs\n", sizeof(char), 6, fd);
            fwrite("By Album\n", sizeof(char), 9, fd);
            fwrite(buffer, sizeof(char), strlen(buffer), fd);
            fwrite("\n0\n", sizeof(char), 3, fd);
            sprintf(buffer, "%s/menus/Music/ByArtist.mnu", mRootDir);
            fwrite("%menu\n", sizeof(char), 6, fd);
            fwrite("songs\n", sizeof(char), 6, fd);
            fwrite("By Artist\n", sizeof(char), 10, fd);
            fwrite(buffer, sizeof(char), strlen(buffer), fd);
            fwrite("\n0\n", sizeof(char), 3, fd);
            sprintf(buffer, "%s/menus/Music/ByYear.mnu", mRootDir);
            fwrite("%menu\n", sizeof(char), 6, fd);
            fwrite("songs\n", sizeof(char), 6, fd);
            fwrite("By Year\n", sizeof(char), 8, fd);
            fwrite(buffer, sizeof(char), strlen(buffer), fd);
            fwrite("\n0\n", sizeof(char), 3, fd);
            fwrite("%space\n", sizeof(char), 7, fd);
            fwrite("15\n", sizeof(char), 3, fd);
            fwrite("0\n", sizeof(char), 2, fd);
            fwrite("%rescan\n", sizeof(char), 8, fd);
            fwrite("Rescan music files\n", sizeof(char), 19, fd);
            fwrite("tags\n", sizeof(char), 5, fd);
            fwrite("1\n", sizeof(char), 2, fd);
            fwrite("%space\n", sizeof(char), 7, fd);
            fwrite("15\n", sizeof(char), 3, fd);
            fwrite("0\n", sizeof(char), 2, fd);
            fwrite("%showplaylist\n", sizeof(char), 14, fd);
            fwrite("Show Play List\n", sizeof(char), 15, fd);
            fwrite("%stopplay\n", sizeof(char), 10, fd);
            fwrite("Stop Music\n", sizeof(char), 11, fd);
            fwrite("%space\n", sizeof(char), 7, fd);
            fwrite("15\n", sizeof(char), 3, fd);
            fwrite("0\n", sizeof(char), 2, fd);
            if (mScanType == S_ALL_FLAT) {
                sprintf(buffer, "%s/menus/Music/ByDirectory.mnu", mRootDir);
                fwrite("%menu\n", sizeof(char), 6, fd);
                fwrite("songs\n", sizeof(char), 6, fd);
                fwrite("By Directory\n", sizeof(char), 13, fd);
                fwrite(buffer, sizeof(char), strlen(buffer), fd);
                fwrite("\n0\n", sizeof(char), 3, fd);
            } else {
                end = mMusicDirs.end();
                for(it = mMusicDirs.begin(); it != end; it++) {
                    bool isSlideShow = false;
                    vector<SDeskScanApp *>::const_iterator it2,
                        end2 = mSlideShowList.end();
                    for(it2 = mSlideShowList.begin(); it2 != end2; it2++) {
                        const char* name = (*it2)->getFileName();
                        int len = strlen(name);
                        while (len > 0 && name[len] != '/') {
                            len--;
                        }
                        if (name[len] == '/') {
                            len--;
                        }
                        if (strncmp((*it)->c_str(), name, len) == 0) {
                            isSlideShow = true;
                        }
                    }
                    if (!isSlideShow) {
                        namePos = (*it)->c_str();
                        tailName = (*it)->c_str();
                        for (; *namePos != 0; namePos++) {
                            if (*namePos == '/') {
                                namePos++;
                                if (*namePos != 0) {
                                    tailName = namePos;
                                }
                            }
                        }
                        fwrite("%menu\n", sizeof(char), 6, fd);
                        fwrite("songs\n", sizeof(char), 6, fd);
                        fwrite(tailName, sizeof(char), strlen(tailName), fd);
                        fwrite("\n", sizeof(char), 1, fd);
                        fwrite((*it)->c_str(), sizeof(char),
                               strlen((*it)->c_str()), fd);
                        fwrite("\n0\n", sizeof(char), 3, fd);
                    }
                }
            }
            fclose(fd);
        }

        // The directory list...
        strcpy(fileName, mRootDir);
        strcat(fileName, "/menus/Music/ByDirectory.mnu");
        if (SDeskScan::getInstance().DoBackup()) {
            strcpy(saveFileName, mRootDir);
            strcat(saveFileName, "/menus/Music/ByDirectory.mnu.save");
            rename(fileName, saveFileName);
        }

        Log(7, "SDeskScan::WriteFiles(%s)\n", fileName);
        if ((fd = fopen(fileName, "w+")) == NULL) {
            Log(1, "fopen failed (%s)...\n", fileName);
        } else {
            vector<string *>::const_iterator it, end = mMusicDirs.end();
            for(it = mMusicDirs.begin(); it != end; it++) {
                namePos = (*it)->c_str();
                tailName = (*it)->c_str();
                for (; *namePos != 0; namePos++) {
                    if (*namePos == '/') {
                        namePos++;
                        if (*namePos != 0) {
                            tailName = namePos;
                        }
                    }
                }
                fwrite("%menu\n", sizeof(char), 6, fd);
                fwrite("songs\n", sizeof(char), 6, fd);
                fwrite(tailName, sizeof(char), strlen(tailName), fd);
                fwrite("\n", sizeof(char), 1, fd);
                fwrite((*it)->c_str(), sizeof(char),
                       strlen((*it)->c_str()), fd);
                fwrite("\n0\n", sizeof(char), 3, fd);
            }
            fclose(fd);
        }

        // Write the favorites files
        strcpy(fileName, mRootDir);
        strcat(fileName, "/menus/MyFavorites.mnu");
        if (SDeskScan::getInstance().DoBackup()) {
            strcpy(saveFileName, mRootDir);
            strcat(saveFileName, "/menus/MyFavorites.mnu.save");
            rename(fileName, saveFileName);
        }

        Log(7, "SDeskScan::WriteFiles(%s)\n", fileName);
        if ((fd = fopen(fileName, "w+")) == NULL) {
            Log(1, "fopen failed (%s)...\n", fileName);
        } else {
            vector<string *>::const_iterator it, end = mFavoriteAll.end();
            for(it = mFavoriteAll.begin(); it != end; it++) {
                strcpy(buf2, (*it)->c_str());
                FixFileName(buf2);
                strcpy(buffer, mRootDir);
                strcat(buffer, "/menus/");
                strcat(buffer, buf2);
                strcat(buffer, ".mnu");
                fwrite("%menu\n", sizeof(char), 6, fd);
                fwrite("favorites\n", sizeof(char), 10, fd);
                fwrite((*it)->c_str(), sizeof(char),
                       strlen((*it)->c_str()), fd);
                fwrite("\n", sizeof(char), 1, fd);
                fwrite(buffer, sizeof(char), strlen(buffer), fd);
                fwrite("\n0\n", sizeof(char), 3, fd);
                if (stat(buffer, &filestat) != 0) {
                    FILE *fdNew;
                    Log(7, "SDeskScan::WriteFiles(%s)\n", buffer);
                    if ((fdNew = fopen(buffer, "w+")) == NULL) {
                        Log(1, "fopen failed (%s)...\n", buffer);
                    } else {
                        fclose(fdNew);
                    }
                }
            }
            end = mFavoriteSongs.end();
            for(it = mFavoriteSongs.begin(); it != end; it++) {
                strcpy(buf2, (*it)->c_str());
                FixFileName(buf2);
                strcpy(buffer, mRootDir);
                strcat(buffer, "/menus/");
                strcat(buffer, buf2);
                strcat(buffer, ".mnu");
                fwrite("%menu\n", sizeof(char), 6, fd);
                fwrite("favorites\n", sizeof(char), 10, fd);
                fwrite((*it)->c_str(), sizeof(char),
                       strlen((*it)->c_str()), fd);
                fwrite("\n", sizeof(char), 1, fd);
                fwrite(buffer, sizeof(char), strlen(buffer), fd);
                fwrite("\n0\n", sizeof(char), 3, fd);
                if (stat(buffer, &filestat) != 0) {
                    FILE *fdNew;
                    Log(7, "SDeskScan::WriteFiles(%s)\n", buffer);
                    if ((fdNew = fopen(buffer, "w+")) == NULL) {
                        Log(1, "fopen failed (%s)...\n", buffer);
                    } else {
                        fclose(fdNew);
                    }
                }
            }
            end = mFavoritePictures.end();
            for(it = mFavoritePictures.begin(); it != end; it++) {
                strcpy(buf2, (*it)->c_str());
                FixFileName(buf2);
                strcpy(buffer, mRootDir);
                strcat(buffer, "/menus/");
                strcat(buffer, buf2);
                strcat(buffer, ".mnu");
                fwrite("%menu\n", sizeof(char), 6, fd);
                fwrite("favorites\n", sizeof(char), 10, fd);
                fwrite((*it)->c_str(), sizeof(char),
                       strlen((*it)->c_str()), fd);
                fwrite("\n", sizeof(char), 1, fd);
                fwrite(buffer, sizeof(char), strlen(buffer), fd);
                fwrite("\n0\n", sizeof(char), 3, fd);
                if (stat(buffer, &filestat) != 0) {
                    FILE *fdNew;
                    Log(7, "SDeskScan::WriteFiles(%s)\n", buffer);
                    if ((fdNew = fopen(buffer, "w+")) == NULL) {
                        Log(1, "fopen failed (%s)...\n", buffer);
                    } else {
                        fclose(fdNew);
                    }
                }
            }
            end = mFavoriteVideos.end();
            for(it = mFavoriteVideos.begin(); it != end; it++) {
                strcpy(buf2, (*it)->c_str());
                FixFileName(buf2);
                strcpy(buffer, mRootDir);
                strcat(buffer, "/menus/");
                strcat(buffer, buf2);
                strcat(buffer, ".mnu");
                fwrite("%menu\n", sizeof(char), 6, fd);
                fwrite("favorites\n", sizeof(char), 10, fd);
                fwrite((*it)->c_str(), sizeof(char),
                       strlen((*it)->c_str()), fd);
                fwrite("\n", sizeof(char), 1, fd);
                fwrite(buffer, sizeof(char), strlen(buffer), fd);
                fwrite("\n0\n", sizeof(char), 3, fd);
                if (stat(buffer, &filestat) != 0) {
                    FILE *fdNew;
                    Log(7, "SDeskScan::WriteFiles(%s)\n", buffer);
                    if ((fdNew = fopen(buffer, "w+")) == NULL) {
                        Log(1, "fopen failed (%s)...\n", buffer);
                    } else {
                        fclose(fdNew);
                    }
                }
            }
            end = mFavoriteRadios.end();
            for(it = mFavoriteRadios.begin(); it != end; it++) {
                strcpy(buf2, (*it)->c_str());
                FixFileName(buf2);
                strcpy(buffer, mRootDir);
                strcat(buffer, "/menus/");
                strcat(buffer, buf2);
                strcat(buffer, ".mnu");
                fwrite("%menu\n", sizeof(char), 6, fd);
                fwrite("favorites\n", sizeof(char), 10, fd);
                fwrite((*it)->c_str(), sizeof(char),
                       strlen((*it)->c_str()), fd);
                fwrite("\n", sizeof(char), 1, fd);
                fwrite(buffer, sizeof(char), strlen(buffer), fd);
                fwrite("\n0\n", sizeof(char), 3, fd);
                if (stat(buffer, &filestat) != 0) {
                    FILE *fdNew;
                    Log(7, "SDeskScan::WriteFiles(%s)\n", buffer);
                    if ((fdNew = fopen(buffer, "w+")) == NULL) {
                        Log(1, "fopen failed (%s)...\n", buffer);
                    } else {
                        fclose(fdNew);
                    }
                }
            }
            end = mFavoriteSlideShows.end();
            for(it = mFavoriteSlideShows.begin(); it != end; it++) {
                strcpy(buf2, (*it)->c_str());
                FixFileName(buf2);
                strcpy(buffer, mRootDir);
                strcat(buffer, "/menus/");
                strcat(buffer, buf2);
                strcat(buffer, ".mnu");
                fwrite("%menu\n", sizeof(char), 6, fd);
                fwrite("favorites\n", sizeof(char), 10, fd);
                fwrite((*it)->c_str(), sizeof(char),
                       strlen((*it)->c_str()), fd);
                fwrite("\n", sizeof(char), 1, fd);
                fwrite(buffer, sizeof(char), strlen(buffer), fd);
                fwrite("\n0\n", sizeof(char), 3, fd);
                if (stat(buffer, &filestat) != 0) {
                    FILE *fdNew;
                    Log(7, "SDeskScan::WriteFiles(%s)\n", buffer);
                    if ((fdNew = fopen(buffer, "w+")) == NULL) {
                        Log(1, "fopen failed (%s)...\n", buffer);
                    } else {
                        fclose(fdNew);
                    }
                }
            }
            fclose(fd);
        }
        
        // Write the MyPictures file.
        if (mPictureDirs.size() > 1 || mFavoritePictures.size() > 0) {
            strcpy(fileName, mRootDir);
            strcat(fileName, "/menus/MyPictures.mnu");
            if (SDeskScan::getInstance().DoBackup()) {
                strcpy(saveFileName, mRootDir);
                strcat(saveFileName, "/menus/MyPictures.mnu.save");
                rename(fileName, saveFileName);
            }

            Log(7, "SDeskScan::WriteFiles(%s)\n", fileName);
            if ((fd = fopen(fileName, "w+")) == NULL) {
                Log(1, "fopen failed (%s)...\n", fileName);
            } else {
                fwrite(SDESK_MNU_SLIDESHOWS, sizeof(char), 
                       strlen(SDESK_MNU_SLIDESHOWS), fd);
                vector<string *>::const_iterator it,
                    end = mFavoritePictures.end();
                for(it = mFavoritePictures.begin(); it != end; it++) {
                    strcpy(buf2, (*it)->c_str());
                    FixFileName(buf2);
                    strcpy(buffer, mRootDir);
                    strcat(buffer, "/menus/");
                    strcat(buffer, buf2);
                    strcat(buffer, ".mnu");
                    fwrite("%menu\n", sizeof(char), 6, fd);
                    fwrite("favorites\n", sizeof(char), 10, fd);
                    fwrite((*it)->c_str(), sizeof(char),
                           strlen((*it)->c_str()), fd);
                    fwrite("\n", sizeof(char), 1, fd);
                    fwrite(buffer, sizeof(char), strlen(buffer), fd);
                    fwrite("\n0\n", sizeof(char), 3, fd);
                }
                fwrite("%space\n", sizeof(char), 7, fd);
                fwrite("15\n", sizeof(char), 3, fd);
                fwrite("0\n", sizeof(char), 2, fd);
                if (mScanType == S_ALL_FLAT) {
                    sprintf(buffer, "%s/menus/ByPictureDirs.mnu", mRootDir);
                    fwrite("%menu\n", sizeof(char), 6, fd);
                    fwrite("songs\n", sizeof(char), 6, fd);
                    fwrite("By Directory\n", sizeof(char), 13, fd);
                    fwrite(buffer, sizeof(char), strlen(buffer), fd);
                    fwrite("\n0\n", sizeof(char), 3, fd);
                } else {
                    end = mPictureDirs.end();
                    for(it = mPictureDirs.begin(); it != end; it++) {
                        namePos = (*it)->c_str();
                        tailName = (*it)->c_str();
                        for (; *namePos != 0; namePos++) {
                            if (*namePos == '/') {
                                namePos++;
                                if (*namePos != 0) {
                                    tailName = namePos;
                                }
                            }
                        }
                        fwrite("%menu\n", sizeof(char), 6, fd);
                        fwrite("pictures\n", sizeof(char), 9, fd);
                        fwrite(tailName, sizeof(char), strlen(tailName), fd);
                        fwrite("\n", sizeof(char), 1, fd);
                        fwrite((*it)->c_str(), sizeof(char), 
                               strlen((*it)->c_str()), fd);
                        fwrite("\n0\n", sizeof(char), 3, fd);
                    }
                }
                fclose(fd);
            }
        }
        
        // Write the ByPictureDirs file.
        strcpy(fileName, mRootDir);
        strcat(fileName, "/menus/ByPictureDirs.mnu");
        if (SDeskScan::getInstance().DoBackup()) {
            strcpy(saveFileName, mRootDir);
            strcat(saveFileName, "/menus/ByPictureDirs.mnu.save");
            rename(fileName, saveFileName);
        }

        Log(7, "SDeskScan::WriteFiles(%s)\n", fileName);
        if ((fd = fopen(fileName, "w+")) == NULL) {
            Log(1, "fopen failed (%s)...\n", fileName);
        } else {
            vector<string *>::const_iterator it,
                end = mPictureDirs.end();
            for(it = mPictureDirs.begin(); it != end; it++) {
                namePos = (*it)->c_str();
                tailName = (*it)->c_str();
                for (; *namePos != 0; namePos++) {
                    if (*namePos == '/') {
                        namePos++;
                        if (*namePos != 0) {
                            tailName = namePos;
                        }
                    }
                }
                fwrite("%menu\n", sizeof(char), 6, fd);
                fwrite("pictures\n", sizeof(char), 9, fd);
                fwrite(tailName, sizeof(char), strlen(tailName), fd);
                fwrite("\n", sizeof(char), 1, fd);
                fwrite((*it)->c_str(), sizeof(char), 
                       strlen((*it)->c_str()), fd);
                fwrite("\n0\n", sizeof(char), 3, fd);
            }
            fclose(fd);
        }

        // Write the slideshow file
        strcpy(fileName, mRootDir);
        strcat(fileName, "/menus/MySlideShows.mnu");
        if (SDeskScan::getInstance().DoBackup()) {
            strcpy(saveFileName, mRootDir);
            strcat(saveFileName, "/menus/MySlideShows.mnu.save");
            rename(fileName, saveFileName);
        }

        Log(7, "SDeskScan::WriteFiles(%s)\n", fileName);
        if ((fd = fopen(fileName, "w+")) == NULL) {
            Log(1, "fopen failed (%s)...\n", fileName);
        } else {
            vector<string *>::const_iterator it, 
                end = mFavoriteSlideShows.end();
            for(it = mFavoriteSlideShows.begin(); it != end; it++) {
                strcpy(buf2, (*it)->c_str());
                FixFileName(buf2);
                strcpy(buffer, mRootDir);
                strcat(buffer, "/menus/");
                strcat(buffer, buf2);
                strcat(buffer, ".mnu");
                fwrite("%menu\n", sizeof(char), 6, fd);
                fwrite("favorites\n", sizeof(char), 10, fd);
                fwrite((*it)->c_str(), sizeof(char),
                       strlen((*it)->c_str()), fd);
                fwrite("\n", sizeof(char), 1, fd);
                fwrite(buffer, sizeof(char), strlen(buffer), fd);
                fwrite("\n0\n", sizeof(char), 3, fd);
                if (stat(buffer, &filestat) != 0) {
                    FILE *fdNew;
                    Log(7, "SDeskScan::WriteFiles(%s)\n", buffer);
                    if ((fdNew = fopen(buffer, "w+")) == NULL) {
                        Log(1, "fopen failed (%s)...\n", buffer);
                    } else {
                        fclose(fdNew);
                    }
                }
            }
            vector<SDeskScanApp *>::const_iterator it2,
                end2 = mSlideShowList.end();
            for(it2 = mSlideShowList.begin(); it2 != end2; it2++) {
                const char* name = (*it2)->getName();
                const char* location = (*it2)->getFileName();
                    
                strcpy(buffer, name);
                buffer[strlen(name) - 8] = 0;
                fwrite("%application\n", sizeof(char), 13, fd);
                fwrite(buffer, sizeof(char), strlen(buffer), fd);
                fwrite("\n", sizeof(char), 1, fd);
                fwrite(location, sizeof(char), strlen(location), fd);
                fwrite("\n", sizeof(char), 1, fd);
                if ((*it2)->getProtected()) {
                    fwrite("1\n", sizeof(char), 2, fd);
                } else {
                    fwrite("0\n", sizeof(char), 2, fd);
                }
            }
            end = mSlideShowDirs.end();
            for(it = mSlideShowDirs.begin(); it != end; it++) {
                namePos = (*it)->c_str();
                tailName = (*it)->c_str();
                for (; *namePos != 0; namePos++) {
                    if (*namePos == '/') {
                        namePos++;
                        if (*namePos != 0) {
                            tailName = namePos;
                        }
                    }
                }
                bool hadApp = false;
                end2 = mSlideShowList.end();
                for(it2 = mSlideShowList.begin(); it2 != end2; it2++) {
                    const char* name = (*it2)->getName();
                    if (strncmp(name, tailName, strlen(tailName)) == 0) {
                        hadApp = true;
                    }
                }
                if (!hadApp) {
                    fwrite("%menu\n", sizeof(char), 6, fd);
                    fwrite("favorites\n", sizeof(char), 10, fd);
                    fwrite(tailName, sizeof(char), strlen(tailName), fd);
                    fwrite("\n", sizeof(char), 1, fd);
                    fwrite((*it)->c_str(), sizeof(char),
                           strlen((*it)->c_str()), fd);
                    fwrite("\n0\n", sizeof(char), 3, fd);
                }
            }
            fclose(fd);
        }
        
        // Write the applications file
        strcpy(fileName, mRootDir);
        strcat(fileName, "/menus/MyApplications.mnu");
        if (SDeskScan::getInstance().DoBackup()) {
            strcpy(saveFileName, mRootDir);
            strcat(saveFileName, "/menus/MyApplications.mnu.save");
            rename(fileName, saveFileName);
        }

        Log(7, "SDeskScan::WriteFiles(%s)\n", fileName);
        if ((fd = fopen(fileName, "w+")) == NULL) {
            Log(1, "fopen failed (%s)...\n", fileName);
        } else {
            if (mCompactRootMenu) {
                if (mNewsList.size() == 0) {
                    fwrite(SDESK_MNU_NEWS, sizeof(char), 
                           strlen(SDESK_MNU_NEWS), fd);
                } else if (mNewsList.size() == 1) {
                    vector<SDeskScanNews *>::const_iterator it,
                        end = mNewsList.end();
                    for(it = mNewsList.begin(); it != end; it++) {
                        const char* name = (*it)->getName();
                        const char* location = (*it)->getFileName();
                        
                        fwrite("%rssfeed\n", sizeof(char), 9, fd);
                        fwrite(name, sizeof(char), strlen(name), fd);
                        fwrite("\n", sizeof(char), 1, fd);
                        fwrite(location, sizeof(char), strlen(location), fd);
                        fwrite("\n", sizeof(char), 1, fd);
                        if ((*it)->getProtected()) {
                            fwrite("1\n", sizeof(char), 2, fd);
                        } else {
                            fwrite("0\n", sizeof(char), 2, fd);
                        }
                    }
                } else {
                    fwrite(SDESK_MNU_NEWS_FILE, sizeof(char), 
                           strlen(SDESK_MNU_NEWS_FILE), fd);
                }
                if (mWeatherSix[0] != '\0') {
                    fwrite("%application\n", sizeof(char), 13, fd);
                    fwrite("Weather\n", sizeof(char), 8, fd);
                    fwrite(mWeatherSix, sizeof(char), strlen(mWeatherSix), fd);
                    fwrite("\n0\n", sizeof(char), 3, fd);
                }
                fwrite(SDESK_APPS_SPACE, sizeof(char),
                       strlen(SDESK_APPS_SPACE), fd);
            }
            if (mAppsList.size() > 0) {
                vector<SDeskScanApp *>::const_iterator it,
                    end = mAppsList.end();
                for(it = mAppsList.begin(); it != end; it++) {
                    const char* name = (*it)->getName();
                    const char* location = (*it)->getFileName();
                    
                    fwrite("%application\n", sizeof(char), 13, fd);
                    fwrite(name, sizeof(char), strlen(name), fd);
                    fwrite("\n", sizeof(char), 1, fd);
                    fwrite(location, sizeof(char), strlen(location), fd);
                    fwrite("\n", sizeof(char), 1, fd);
                    if ((*it)->getProtected()) {
                        fwrite("1\n", sizeof(char), 2, fd);
                    } else {
                        fwrite("0\n", sizeof(char), 2, fd);
                    }
                }
                fwrite(SDESK_APPS_SPACE, sizeof(char),
                       strlen(SDESK_APPS_SPACE), fd);
            }
            fwrite(SDESK_APPS_BOTTOM, sizeof(char),
                   strlen(SDESK_APPS_BOTTOM), fd);
        }
        
        // Write the news file
        if (mNewsList.size() > 1) {
            strcpy(fileName, mRootDir);
            strcat(fileName, "/menus/MyNews.mnu");
            if (SDeskScan::getInstance().DoBackup()) {
                strcpy(saveFileName, mRootDir);
                strcat(saveFileName, "/menus/MyNews.mnu.save");
                rename(fileName, saveFileName);
            }
            
            Log(7, "SDeskScan::WriteFiles(%s)\n", fileName);
            if ((fd = fopen(fileName, "w+")) == NULL) {
                Log(1, "fopen failed (%s)...\n", fileName);
            } else {
                if (mNewsList.size() > 0) {
                    vector<SDeskScanNews *>::const_iterator it,
                        end = mNewsList.end();
                    for(it = mNewsList.begin(); it != end; it++) {
                        const char* name = (*it)->getName();
                        const char* location = (*it)->getFileName();
                    
                        fwrite("%rssfeed\n", sizeof(char), 9, fd);
                        fwrite(name, sizeof(char), strlen(name), fd);
                        fwrite("\n", sizeof(char), 1, fd);
                        fwrite(location, sizeof(char), strlen(location), fd);
                        fwrite("\n", sizeof(char), 1, fd);
                        if ((*it)->getProtected()) {
                            fwrite("1\n", sizeof(char), 2, fd);
                        } else {
                            fwrite("0\n", sizeof(char), 2, fd);
                        }
                    }
                }
            }
        }
        
        // Write the SDesk.mnu file.
        strcpy(fileName, mRootDir);
        strcat(fileName, "/menus/SDesk.mnu");
        if (SDeskScan::getInstance().DoBackup()) {
            strcpy(saveFileName, mRootDir);
            strcat(saveFileName, "/menus/SDesk.mnu.save");
            rename(fileName, saveFileName);
        }

        Log(7, "SDeskScan::WriteFiles(%s)\n", fileName);
        if ((fd = fopen(fileName, "w+")) == NULL) {
            Log(1, "fopen failed (%s)...\n", fileName);
        } else {
            strcpy(fileName, "/mnt/smb/dune/tv");
            Log(7, "SDeskScan::WriteFiles(%s)\n", fileName);
            if (stat(fileName, &filestat) == 0) {
                fwrite(SDESK_MNU_TV, sizeof(char), 
                       strlen(SDESK_MNU_TV), fd);
            }
            if (mFavoriteAll.size() == 1) {
                vector<string *>::const_iterator it,
                    end = mFavoriteAll.end();
                fwrite("%menu\n", sizeof(char), 6, fd);
                fwrite("favorites\n", sizeof(char), 10, fd);
                fwrite("Favorites\n", sizeof(char), 10, fd);
                for(it = mFavoriteAll.begin(); it != end; it++) {
                    fwrite((*it)->c_str(), sizeof(char),
                           strlen((*it)->c_str()), fd);
                }
                fwrite("\n0\n", sizeof(char), 3, fd);
            } else if (mFavoriteAll.size() > 0) {
                fwrite(SDESK_MNU_FAVORITES, sizeof(char), 
                       strlen(SDESK_MNU_FAVORITES), fd);
            }
            if (mMPlay[0] != '\0') {
                if (mVideoDirs.size() == 1 && mFavoriteVideos.size() == 0) {
                    vector<string *>::const_iterator it, 
                        end = mVideoDirs.end();
                    fwrite("%menu\n", sizeof(char), 6, fd);
                    fwrite("movies\n", sizeof(char), 7, fd);
                    fwrite("Videos\n", sizeof(char), 7, fd);
                    for(it = mVideoDirs.begin(); it != end; it++) {
                        fwrite((*it)->c_str(), sizeof(char),
                               strlen((*it)->c_str()), fd);
                    }
                    fwrite("\n0\n", sizeof(char), 3, fd);
                } else if (mVideoDirs.size() > 0) {
                    fwrite(SDESK_MNU_VIDEOS, sizeof(char), 
                           strlen(SDESK_MNU_VIDEOS), fd);
                }
            }
            if (mMusicDirs.size() > 0) {
                fwrite(SDESK_MNU_MUSIC, sizeof(char), 
                       strlen(SDESK_MNU_MUSIC), fd);
            }
            if (mRadioPlayer[0] != '\0') {
                fwrite(SDESK_MNU_RADIO, sizeof(char), 
                       strlen(SDESK_MNU_RADIO), fd);
            }
            if (mPictureDirs.size() == 1 && mFavoritePictures.size() == 0) {
                vector<string *>::const_iterator it, 
                    end = mPictureDirs.end();
                fwrite("%menu\n", sizeof(char), 6, fd);
                fwrite("pictures\n", sizeof(char), 9, fd);
                fwrite("Pictures\n", sizeof(char), 9, fd);
                for(it = mPictureDirs.begin(); it != end; it++) {
                    fwrite((*it)->c_str(), sizeof(char),
                           strlen((*it)->c_str()), fd);
                }
                fwrite("\n0\n", sizeof(char), 3, fd);
            } else if (mPictureDirs.size() > 0) {
                fwrite(SDESK_MNU_PICTURES, sizeof(char), 
                       strlen(SDESK_MNU_PICTURES), fd);
            }
            if (mHomeManagement) {
                fwrite(SDESK_MNU_HOME_MANAGEMENT, sizeof(char), 
                       strlen(SDESK_MNU_HOME_MANAGEMENT), fd);
            }
            if (!mCompactRootMenu) {
                if (mNewsList.size() == 0) {
                    fwrite(SDESK_MNU_NEWS, sizeof(char), 
                           strlen(SDESK_MNU_NEWS), fd);
                } else if (mNewsList.size() == 1) {
                    vector<SDeskScanNews *>::const_iterator it,
                        end = mNewsList.end();
                    for(it = mNewsList.begin(); it != end; it++) {
                        const char* name = (*it)->getName();
                        const char* location = (*it)->getFileName();
                        
                        fwrite("%rssfeed\n", sizeof(char), 9, fd);
                        fwrite(name, sizeof(char), strlen(name), fd);
                        fwrite("\n", sizeof(char), 1, fd);
                        fwrite(location, sizeof(char), strlen(location), fd);
                        fwrite("\n", sizeof(char), 1, fd);
                        if ((*it)->getProtected()) {
                            fwrite("1\n", sizeof(char), 2, fd);
                        } else {
                            fwrite("0\n", sizeof(char), 2, fd);
                        }
                    }
                } else {
                    fwrite(SDESK_MNU_NEWS_FILE, sizeof(char), 
                           strlen(SDESK_MNU_NEWS_FILE), fd);
                }
                if (mWeatherSix[0] != '\0') {
                    fwrite("%application\n", sizeof(char), 13, fd);
                    fwrite("Weather\n", sizeof(char), 8, fd);
                    fwrite(mWeatherSix, sizeof(char), strlen(mWeatherSix), fd);
                    fwrite("\n0\n", sizeof(char), 3, fd);
                }
            }
            fwrite(SDESK_MNU_BOTTOM, sizeof(char),
                   strlen(SDESK_MNU_BOTTOM), fd);
            fclose(fd);
        }

        // Write daily script
        sprintf(fileName, "%s/apps/sdeskdaily", mRootDir);
        sprintf(saveFileName, "%s/apps/sdeskdaily.save", mRootDir);
        if (stat(fileName, &filestat) != 0) {
            Log(7, "SDeskScan::WriteFiles(%s)\n", fileName);
            if ((fd = fopen(fileName, "w+")) == NULL) {
                Log(1, "fopen failed (%s)...\n", fileName);
            } else {
                fwrite("#!/bin/sh\n", sizeof(char), 10, fd);
                fwrite("/bin/date > /tmp/sdeskdaily.log\n",
                       sizeof(char), 32, fd);
                fwrite("/bin/ntpdate ntp.apple.com\n",
                       sizeof(char), 27, fd);
                sprintf(buffer, "cd \"%s\"\n", mRootDir);
                fwrite(buffer, sizeof(char), strlen(buffer), fd);
                fwrite("/bin/echo start >> /tmp/sdeskdaily.log\n",
                       sizeof(char), 39, fd);
                sprintf(buffer, "\"%s/SDeskScan\" tags \"%s\"\n",
                        mRootDir, mRootDir);
                fwrite(buffer, sizeof(char), strlen(buffer), fd);
                fwrite("/bin/echo next >> /tmp/sdeskdaily.log\n",
                       sizeof(char), 38, fd);
                sprintf(buffer, "\"%s/SDeskScan\" radio \"%s\"\n",
                        mRootDir, mRootDir);
                fwrite(buffer, sizeof(char), strlen(buffer), fd);
                fwrite("/bin/echo next >> /tmp/sdeskdaily.log\n",
                       sizeof(char), 38, fd);
                sprintf(buffer, "\"%s/SDeskScan\" videos \"%s\"\n",
                        mRootDir, mRootDir);
                fwrite(buffer, sizeof(char), strlen(buffer), fd);
                fwrite("/bin/echo done >> /tmp/sdeskdaily.log\n",
                       sizeof(char), 38, fd);
                fclose(fd);
                chmod(fileName, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP |
                      S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);
            }
        }
    }
        
    if (mScanType == S_ALL || mScanType == S_ALL_FLAT ||
        mScanType == S_VIDEOS) {
        // Write the MyVideo file.
        if (mVideoDirs.size() > 1 || mFavoriteVideos.size() > 0) {
            strcpy(fileName, mRootDir);
            strcat(fileName, "/menus/MyVideos.mnu");
            if (SDeskScan::getInstance().DoBackup()) {
                strcpy(saveFileName, mRootDir);
                strcat(saveFileName, "/menus/MyVideos.mnu.save");
                rename(fileName, saveFileName);
            }

            Log(7, "SDeskScan::WriteFiles(%s)\n", fileName);
            if ((fd = fopen(fileName, "w+")) == NULL) {
                Log(1, "fopen failed (%s)...\n", fileName);
            } else {
                vector<string *>::const_iterator it,
                    end = mFavoriteVideos.end();
                for(it = mFavoriteVideos.begin(); it != end; it++) {
                    strcpy(buf2, (*it)->c_str());
                    FixFileName(buf2);
                    strcpy(buffer, mRootDir);
                    strcat(buffer, "/menus/");
                    strcat(buffer, buf2);
                    strcat(buffer, ".mnu");
                    fwrite("%menu\n", sizeof(char), 6, fd);
                    fwrite("favorites\n", sizeof(char), 10, fd);
                    fwrite((*it)->c_str(), sizeof(char),
                           strlen((*it)->c_str()), fd);
                    fwrite("\n", sizeof(char), 1, fd);
                    fwrite(buffer, sizeof(char), strlen(buffer), fd);
                    fwrite("\n0\n", sizeof(char), 3, fd);
                    if (mOverwriteVideoFavorites ||
                        stat(buffer, &filestat) != 0) {
                        FILE *fdNew;
                        Log(7, "SDeskScan::WriteFiles(%s)\n", buffer);
                        if ((fdNew = fopen(buffer, "w+")) == NULL) {
                            Log(1, "fopen failed (%s)...\n", buffer);
                        } else {
                            // Write actual files...
                            vector<string *>::const_iterator it2,
                                end2, begin2;
                            end2 = 0;
                            if (strcmp((*it)->c_str(), "Action") == 0) {
                                end2 = mVideoGenreAction.end();
                                begin2 = mVideoGenreAction.begin();
                            } else if (strcmp((*it)->c_str(), "Adventure") == 0) {
                                end2 = mVideoGenreAdventure.end();
                                begin2 = mVideoGenreAdventure.begin();
                            } else if (strcmp((*it)->c_str(), "Animation") == 0) {
                                end2 = mVideoGenreAnimation.end();
                                begin2 = mVideoGenreAnimation.begin();
                            } else if (strcmp((*it)->c_str(), "Biography") == 0) {
                                end2 = mVideoGenreBiography.end();
                                begin2 = mVideoGenreBiography.begin();
                            } else if (strcmp((*it)->c_str(), "Children") == 0) {
                                end2 = mVideoGenreChildren.end();
                                begin2 = mVideoGenreChildren.begin();
                            } else if (strcmp((*it)->c_str(), "Comedy") == 0) {
                                end2 = mVideoGenreComedy.end();
                                begin2 = mVideoGenreComedy.begin();
                            } else if (strcmp((*it)->c_str(), "Crime") == 0) {
                                end2 = mVideoGenreCrime.end();
                                begin2 = mVideoGenreCrime.begin();
                            } else if (strcmp((*it)->c_str(), "Documentary") == 0) {
                                end2 = mVideoGenreDocumentary.end();
                                begin2 = mVideoGenreDocumentary.begin();
                            } else if (strcmp((*it)->c_str(), "Drama") == 0) {
                                end2 = mVideoGenreDrama.end();
                                begin2 = mVideoGenreDrama.begin();
                            } else if (strcmp((*it)->c_str(), "Family") == 0) {
                                end2 = mVideoGenreFamily.end();
                                begin2 = mVideoGenreFamily.begin();
                            } else if (strcmp((*it)->c_str(), "Fantasy") == 0) {
                                end2 = mVideoGenreFantasy.end();
                                begin2 = mVideoGenreFantasy.begin();
                            } else if (strcmp((*it)->c_str(), "Film-Noir") == 0) {
                                end2 = mVideoGenreFilmNoir.end();
                                begin2 = mVideoGenreFilmNoir.begin();
                            } else if (strcmp((*it)->c_str(), "Game-Show") == 0) {
                                end2 = mVideoGenreGameShow.end();
                                begin2 = mVideoGenreGameShow.begin();
                            } else if (strcmp((*it)->c_str(), "HDTV") == 0) {
                                end2 = mVideoGenreHDTV.end();
                                begin2 = mVideoGenreHDTV.begin();
                            } else if (strcmp((*it)->c_str(), "History") == 0) {
                                end2 = mVideoGenreHistory.end();
                                begin2 = mVideoGenreHistory.begin();
                            } else if (strcmp((*it)->c_str(), "Home Video") == 0) {
                                end2 = mVideoGenreHomeVideo.end();
                                begin2 = mVideoGenreHomeVideo.begin();
                            } else if (strcmp((*it)->c_str(), "Horror") == 0) {
                                end2 = mVideoGenreHorror.end();
                                begin2 = mVideoGenreHorror.begin();
                            } else if (strcmp((*it)->c_str(), "Movie") == 0) {
                                end2 = mVideoGenreMovie.end();
                                begin2 = mVideoGenreMovie.begin();
                            } else if (strcmp((*it)->c_str(), "Musical") == 0) {
                                end2 = mVideoGenreMusical.end();
                                begin2 = mVideoGenreMusical.begin();
                            } else if (strcmp((*it)->c_str(), "Music") == 0) {
                                end2 = mVideoGenreMusic.end();
                                begin2 = mVideoGenreMusic.begin();
                            } else if (strcmp((*it)->c_str(), "Mystery") == 0) {
                                end2 = mVideoGenreMystery.end();
                                begin2 = mVideoGenreMystery.begin();
                            } else if (strcmp((*it)->c_str(), "News") == 0) {
                                end2 = mVideoGenreNews.end();
                                begin2 = mVideoGenreNews.begin();
                            } else if (strcmp((*it)->c_str(), "Reality-TV") == 0) {
                                end2 = mVideoGenreRealityTV.end();
                                begin2 = mVideoGenreRealityTV.begin();
                            } else if (strcmp((*it)->c_str(), "Romance") == 0) {
                                end2 = mVideoGenreRomance.end();
                                begin2 = mVideoGenreRomance.begin();
                            } else if (strcmp((*it)->c_str(), "Sci-Fi") == 0) {
                                end2 = mVideoGenreSciFi.end();
                                begin2 = mVideoGenreSciFi.begin();
                            } else if (strcmp((*it)->c_str(), "Short") == 0) {
                                end2 = mVideoGenreShort.end();
                                begin2 = mVideoGenreShort.begin();
                            } else if (strcmp((*it)->c_str(), "Sitcom") == 0) {
                                end2 = mVideoGenreSitcom.end();
                                begin2 = mVideoGenreSitcom.begin();
                            } else if (strcmp((*it)->c_str(), "Soap") == 0) {
                                end2 = mVideoGenreSoap.end();
                                begin2 = mVideoGenreSoap.begin();
                            } else if (strcmp((*it)->c_str(), "Sport") == 0) {
                                end2 = mVideoGenreSport.end();
                                begin2 = mVideoGenreSport.begin();
                            } else if (strcmp((*it)->c_str(), "Suspense") == 0) {
                                end2 = mVideoGenreSuspense.end();
                                begin2 = mVideoGenreSuspense.begin();
                            } else if (strcmp((*it)->c_str(), "Talk-Show") == 0) {
                                end2 = mVideoGenreTalkShow.end();
                                begin2 = mVideoGenreTalkShow.begin();
                            } else if (strcmp((*it)->c_str(), "Thriller") == 0) {
                                end2 = mVideoGenreThriller.end();
                                begin2 = mVideoGenreThriller.begin();
                            } else if (strcmp((*it)->c_str(), "War") == 0) {
                                end2 = mVideoGenreWar.end();
                                begin2 = mVideoGenreWar.begin();
                            } else if (strcmp((*it)->c_str(), "Western") == 0) {
                                end2 = mVideoGenreWestern.end();
                                begin2 = mVideoGenreWestern.begin();
                            } else if (strcmp((*it)->c_str(), "Other") == 0) {
                                end2 = mVideoGenreOther.end();
                                begin2 = mVideoGenreOther.begin();
                            }
                            if (end2 != 0) {
                                for(it2 = begin2; it2 != end2; it2++) {
                                    strcpy(buf3, (*it2)->c_str());
                                    fwrite("%movie\n", sizeof(char), 7, fdNew);
                                    fwrite(buf3, sizeof(char), strlen(buf3),
                                           fdNew);
                                    fwrite("\n0\n", sizeof(char), 3, fdNew);
                                }
                            }
                            fclose(fdNew);
                        }
                    }
                }
                fwrite("%space\n", sizeof(char), 7, fd);
                fwrite("15\n", sizeof(char), 3, fd);
                fwrite("0\n", sizeof(char), 2, fd);
                fwrite("%rescan\n", sizeof(char), 8, fd);
                fwrite("Rescan video files\n", sizeof(char), 19, fd);
                fwrite("videos\n", sizeof(char), 7, fd);
                fwrite("1\n", sizeof(char), 2, fd);
                fwrite("%space\n", sizeof(char), 7, fd);
                fwrite("15\n", sizeof(char), 3, fd);
                fwrite("0\n", sizeof(char), 2, fd);
                fwrite("%playvideoqueue\n", sizeof(char), 16, fd);
                fwrite("Play Video Queue\n", sizeof(char), 17, fd);
                fwrite("%clearvideoqueue\n", sizeof(char), 17, fd);
                fwrite("Clear Video Queue\n", sizeof(char), 18, fd);
                fwrite("%space\n", sizeof(char), 7, fd);
                fwrite("15\n", sizeof(char), 3, fd);
                fwrite("0\n", sizeof(char), 2, fd);
                if (mScanType == S_ALL_FLAT) {
                    sprintf(buffer, "%s/menus/ByVideoDirs.mnu",
                            mRootDir);
                    fwrite("%menu\n", sizeof(char), 6, fd);
                    fwrite("movies\n", sizeof(char), 7, fd);
                    fwrite("By Directory\n", sizeof(char), 13, fd);
                    fwrite(buffer, sizeof(char), strlen(buffer), fd);
                    fwrite("\n0\n", sizeof(char), 3, fd);
                } else {
                    end = mVideoDirs.end();
                    for(it = mVideoDirs.begin(); it != end; it++) {
                        namePos = (*it)->c_str();
                        tailName = (*it)->c_str();
                        for (; *namePos != 0; namePos++) {
                            if (*namePos == '/') {
                                namePos++;
                                if (*namePos != 0) {
                                    tailName = namePos;
                                }
                            }
                        }
                        sprintf(buf2, "%s", (*it)->c_str());
                        SDeskDirScanner *dir = new SDeskDirScanner(
                            buf2, "VTS");
                        if (dir->GetNumEntries() == 0) {
                            delete dir;
                            sprintf(buf2, "%s/VIDEO_TS", (*it)->c_str());
                            dir = new SDeskDirScanner(buf2, "VTS");
                        }
                        if (dir->GetNumEntries() > 0) {
                            fwrite("%dvd\n", sizeof(char), 5, fd);
                        } else {
                            fwrite("%menu\n", sizeof(char), 6, fd);
                            fwrite("movies\n", sizeof(char), 7, fd);
                        }
                        delete dir;
                        fwrite(tailName, sizeof(char), strlen(tailName), fd);
                        fwrite("\n", sizeof(char), 1, fd);
                        fwrite((*it)->c_str(), sizeof(char),
                               strlen((*it)->c_str()), fd);
                        fwrite("\n0\n", sizeof(char), 3, fd);
                    }
                }
                fclose(fd);
            }
        }

        // Write the ByVideoDirs file.
        strcpy(fileName, mRootDir);
        strcat(fileName, "/menus/ByVideoDirs.mnu");
        if (SDeskScan::getInstance().DoBackup()) {
            strcpy(saveFileName, mRootDir);
            strcat(saveFileName, "/menus/ByVideoDirs.mnu.save");
            rename(fileName, saveFileName);
        }

        Log(7, "SDeskScan::WriteFiles(%s)\n", fileName);
        if ((fd = fopen(fileName, "w+")) == NULL) {
            Log(1, "fopen failed (%s)...\n", fileName);
        } else {
            vector<string *>::const_iterator it,
                end = mVideoDirs.end();
            for(it = mVideoDirs.begin(); it != end; it++) {
                namePos = (*it)->c_str();
                tailName = (*it)->c_str();
                for (; *namePos != 0; namePos++) {
                    if (*namePos == '/') {
                        namePos++;
                        if (*namePos != 0) {
                            tailName = namePos;
                        }
                    }
                }
                sprintf(buf2, "%s", (*it)->c_str());
                SDeskDirScanner *dir = new SDeskDirScanner(buf2, "VTS");
                if (dir->GetNumEntries() == 0) {
                    delete dir;
                    sprintf(buf2, "%s/VIDEO_TS", (*it)->c_str());
                    dir = new SDeskDirScanner(buf2, "VTS");
                }
                if (dir->GetNumEntries() > 0) {
                    fwrite("%dvd\n", sizeof(char), 5, fd);
                } else {
                    fwrite("%menu\n", sizeof(char), 6, fd);
                    fwrite("movies\n", sizeof(char), 7, fd);
                }
                delete dir;
                fwrite(tailName, sizeof(char), strlen(tailName), fd);
                fwrite("\n", sizeof(char), 1, fd);
                fwrite((*it)->c_str(), sizeof(char),
                       strlen((*it)->c_str()), fd);
                fwrite("\n0\n", sizeof(char), 3, fd);
            }
            fclose(fd);
        }
    }

    if (mScanType == S_ALL || mScanType == S_ALL_FLAT ||
        mScanType == S_RADIO) {
        // Write the MyRadio file.
        strcpy(fileName, mRootDir);
        strcat(fileName, "/menus/Radio/MyRadio.mnu");
        if (SDeskScan::getInstance().DoBackup()) {
            strcpy(saveFileName, mRootDir);
            strcat(saveFileName, "/menus/Radio/MyRadio.mnu.save");
            rename(fileName, saveFileName);
        }

        Log(7, "SDeskScan::WriteFiles(%s)\n", fileName);
        if ((fd = fopen(fileName, "w+")) == NULL) {
            Log(1, "fopen failed (%s)...\n", fileName);
        } else {
            vector<string *>::const_iterator it, end = mFavoriteRadios.end();
            for(it = mFavoriteRadios.begin(); it != end; it++) {
                strcpy(buf2, (*it)->c_str());
                FixFileName(buf2);
                strcpy(buffer, mRootDir);
                strcat(buffer, "/menus/");
                strcat(buffer, buf2);
                strcat(buffer, ".mnu");
                fwrite("%menu\n", sizeof(char), 6, fd);
                fwrite("favorites\n", sizeof(char), 10, fd);
                fwrite((*it)->c_str(), sizeof(char),
                       strlen((*it)->c_str()), fd);
                fwrite("\n", sizeof(char), 1, fd);
                fwrite(buffer, sizeof(char), strlen(buffer), fd);
                fwrite("\n0\n", sizeof(char), 3, fd);
            }
            fwrite("%space\n", sizeof(char), 7, fd);
            fwrite("15\n", sizeof(char), 3, fd);
            fwrite("0\n", sizeof(char), 2, fd);
            fwrite(MYRADIO, sizeof(char), strlen(MYRADIO), fd);
            fclose(fd);
        }
        
        // Write the genre list.
        vector<SDeskScanGenre *>::const_iterator it, 
            end = mGenreList.end();
        for(it = mGenreList.begin(); it != end; it++) {
            if (mGenreType == G_ALL || (*it)->getGenre() == mGenreType) {
                (*it)->DumpGenre();
            }
        }
    }

    if (mScanType == S_ALL || mScanType == S_ALL_FLAT ||
        mScanType == S_TAGS) {
        // Write tag specific files (Genre)
        sprintf(fileName, "%s/menus/Music/ByGenre.mnu", mRootDir);
        if (SDeskScan::getInstance().DoBackup()) {
            sprintf(saveFileName, "%s/menus/Music/ByGenre.mnu.save", mRootDir);
            rename(fileName, saveFileName);
        }

        Log(7, "SDeskScan::WriteFiles(%s)\n", fileName);
        if ((fd = fopen(fileName, "w+")) == NULL) {
            Log(1, "fopen failed (%s)...\n", fileName);
        } else {
            vector<SDeskScanSongByTag *>::const_iterator it, 
                end = mSongGenreTagList.end();
            for(it = mSongGenreTagList.begin(); it != end; it++) {
                namePos = (*it)->getName();
                strcpy(buf2, namePos);
                FixFileName(buf2);
                sprintf(buffer, "%s/menus/Music/ByGenre_%s.mnu",
                        mRootDir, buf2);
                fwrite("%menu\n", sizeof(char), 6, fd);
                fwrite("songs\n", sizeof(char), 6, fd);
                fwrite(namePos, sizeof(char), strlen(namePos), fd);
                fwrite("\n", sizeof(char), 1, fd);
                fwrite(buffer, sizeof(char), strlen(buffer), fd);
                fwrite("\n0\n", sizeof(char), 3, fd);
                (*it)->DumpTagList("Genre");
            }
        }

        // Write tag specific files (Artist)
        sprintf(fileName, "%s/menus/Music/ByArtist.mnu", mRootDir);
        if (SDeskScan::getInstance().DoBackup()) {
            sprintf(saveFileName, "%s/menus/Music/ByArtist.mnu.save", 
                    mRootDir);
            rename(fileName, saveFileName);
        }

        Log(7, "SDeskScan::WriteFiles(%s)\n", fileName);
        if ((fd = fopen(fileName, "w+")) == NULL) {
            Log(1, "fopen failed (%s)...\n", fileName);
        } else {
            vector<SDeskScanSongByTag *>::const_iterator it, 
                end = mSongArtistTagList.end();
            for(it = mSongArtistTagList.begin(); it != end; it++) {
                namePos = (*it)->getName();
                strcpy(buf2, namePos);
                FixFileName(buf2);
                sprintf(buffer, "%s/menus/Music/ByArtist_%s.mnu",
                        mRootDir, buf2);
                fwrite("%menu\n", sizeof(char), 6, fd);
                fwrite("songs\n", sizeof(char), 6, fd);
                fwrite(namePos, sizeof(char), strlen(namePos), fd);
                fwrite("\n", sizeof(char), 1, fd);
                fwrite(buffer, sizeof(char), strlen(buffer), fd);
                fwrite("\n0\n", sizeof(char), 3, fd);
                (*it)->DumpTagList("Artist");
            }
        }

        // Write tag specific files (Album)
        sprintf(fileName, "%s/menus/Music/ByAlbum.mnu", mRootDir);
        if (SDeskScan::getInstance().DoBackup()) {
            sprintf(saveFileName, "%s/menus/Music/ByAlbum.mnu.save", mRootDir);
            rename(fileName, saveFileName);
        }

        Log(7, "SDeskScan::WriteFiles(%s)\n", fileName);
        if ((fd = fopen(fileName, "w+")) == NULL) {
            Log(1, "fopen failed (%s)...\n", fileName);
        } else {
            vector<SDeskScanSongByTag *>::const_iterator it, 
                end = mSongAlbumTagList.end();
            for(it = mSongAlbumTagList.begin(); it != end; it++) {
                namePos = (*it)->getName();
                strcpy(buf2, namePos);
                FixFileName(buf2);
                sprintf(buffer, "%s/menus/Music/ByAlbum_%s.mnu",
                        mRootDir, buf2);
                fwrite("%menu\n", sizeof(char), 6, fd);
                fwrite("songs\n", sizeof(char), 6, fd);
                fwrite(namePos, sizeof(char), strlen(namePos), fd);
                fwrite("\n", sizeof(char), 1, fd);
                fwrite(buffer, sizeof(char), strlen(buffer), fd);
                fwrite("\n0\n", sizeof(char), 3, fd);
                (*it)->DumpTagList("Album");
            }
            fclose(fd);
        }

        // Write tag specific files (Year)
        sprintf(fileName, "%s/menus/Music/ByYear.mnu", mRootDir);
        if (SDeskScan::getInstance().DoBackup()) {
            sprintf(saveFileName, "%s/menus/Music/ByYear.mnu.save", mRootDir);
            rename(fileName, saveFileName);
        }

        Log(7, "SDeskScan::WriteFiles(%s)\n", fileName);
        if ((fd = fopen(fileName, "w+")) == NULL) {
            Log(1, "fopen failed (%s)...\n", fileName);
        } else {
            vector<SDeskScanSongByTag *>::const_iterator it, 
                end = mSongYearTagList.end();
            for(it = mSongYearTagList.begin(); it != end; it++) {
                namePos = (*it)->getName();
                strcpy(buf2, namePos);
                FixFileName(buf2);
                sprintf(buffer, "%s/menus/Music/ByYear_%s.mnu",
                        mRootDir, buf2);
                fwrite("%menu\n", sizeof(char), 6, fd);
                fwrite("songs\n", sizeof(char), 6, fd);
                fwrite(namePos, sizeof(char), strlen(namePos), fd);
                fwrite("\n", sizeof(char), 1, fd);
                fwrite(buffer, sizeof(char), strlen(buffer), fd);
                fwrite("\n0\n", sizeof(char), 3, fd);
                (*it)->DumpTagList("Year");
            }
        }
    }

    // Write the home management file
    if (mHomeManagement) {
        strcpy(fileName, mRootDir);
        strcat(fileName, "/menus/HomeManagement.mnu");
        if (stat(fileName, &filestat) != 0) {
            FILE *fdNew;
            Log(7, "SDeskScan::WriteFiles(%s)\n", fileName);
            if ((fdNew = fopen(fileName, "w+")) == NULL) {
                Log(1, "fopen failed (%s)...\n", fileName);
            } else {
                Log(7, "SDeskScan::WriteFiles(%s) data\n", fileName);
                fwrite(SDESK_MNU_HOME_MANAGEMENT_MENU, sizeof(char),
                       strlen(SDESK_MNU_HOME_MANAGEMENT_MENU), fdNew);
                fclose(fdNew);
            }
        }
    }

    Log(7, "SDeskScan::WriteFiles() done\n");
}

bool SDeskScan::IsExcluded(
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

bool CompareItem(
    const string* a, const string* b)
{
    const char* string1;
    const char* string2;

    if ((string1 = strrchr(a->c_str(), '/')) != 0) {
        string1++;
    } else {
        string1 = a->c_str();
    }
    if ((string2 = strrchr(b->c_str(), '/')) != 0) {
        string2++;
    } else {
        string2 = b->c_str();
    }
    if (string1 != 0 && string2 != 0) {
        return strcmp(string1, string2) < 0;
    }
    return false;
}

bool CompareTagItem(
    SDeskScanSongByTag* a, SDeskScanSongByTag* b)
{
    const char* string1 = a->getName();
    const char* string2 = b->getName();

    if (string1 != 0 && string2 != 0) {
        return strcmp(string1, string2) < 0;
    }
    return false;
}

void SDeskScan::ReadRadio(
    const char *url)
{
    Log(7, "SDeskScan::ReadRadio(%s)\n", url);

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
    strcpy(filename, mRootDir);
    strcat(filename, "/temp.xml");
    FILE *pOutputFile = fopen(filename, "wb"); 
    if (NULL == pOutputFile) { 
        Log(1, "SDeskScan::ReadRadio(%s) write error\n", url);
        return;
    }

    while(1) { 
        bytesReturned = myClient.ReadBytes(buff,buffsize); 
        if (0 == bytesReturned) {
            break; 
        }
        if (bytesReturned != fwrite(buff, 1, bytesReturned, pOutputFile)) { 
            Log(1, "SDeskScan::ReadRadio(%s) read error\n", url);
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
    char name[1024];
    char genre[1024];
    char playstring[1024];
    
    name[0] = 0;
    genre[0] = 0;
    playstring[0] = 0;

    for (node = mxmlFindElement(tree, tree, NULL, NULL, NULL, MXML_DESCEND); 
         node != NULL; 
         node = mxmlFindElement(node, tree, NULL, NULL, NULL, MXML_DESCEND)) { 

        if (strcmp("Genre", node->value.element.name) == 0) { 
            mxml_node_t *child_node; 
            child_node = mxmlWalkNext(node, tree, MXML_DESCEND); 
            if (child_node == NULL) { 
                Log(1, "SDeskScan::ReadRadio(%s) no genre\n", url);
            } else {
                strcpy(genre, child_node->value.text.string); 
            }
        }
        if (strcmp("Name", node->value.element.name) == 0) { 
            mxml_node_t *child_node; 
            child_node = mxmlWalkNext(node, tree, MXML_DESCEND); 
            if(child_node == NULL) { 
                Log(1, "SDeskScan::ReadRadio(%s) no name\n", url);
            } else {
                strcpy(name, child_node->value.text.string);
                child_node = mxmlWalkNext(child_node, node, MXML_DESCEND); 
                while (child_node != NULL) { 
                    strcat(name, " ");
                    strcat(name, child_node->value.text.string); 
                    child_node = mxmlWalkNext(child_node, node, MXML_DESCEND); 
                } 
            } 
        } 
        if (strcmp("entry", node->value.element.name) == 0) { 
            /* 
            ** This should have an attribute, i.e. Playstring 
            */
            if (node->value.element.num_attrs > 0) { 
                /* 
                ** really should check first one is actually Playstring 
                */
                strcpy(playstring, node->value.element.attrs->value); 
            }
        } 
        if (name[0] != 0 && genre[0] != 0 && playstring[0] != 0) {
            if (playstring[0] == 'h') {
                AddRadioStation(name, genre, playstring);
            }
            name[0] = 0;
            genre[0] = 0;
            playstring[0] = 0;
        }
    }

    Log(7, "SDeskScan::ReadRadio()\n");
}

void SDeskScan::AddRadioStation(
    const char* name,
    const char* genre,
    const char* url)
{
    Log(7, "SDeskScan::AddStation(%s, %s, %s)\n", name, genre, url);

    vector<SDeskScanGenre *>::const_iterator it, 
        end = mGenreList.end();
    for(it = mGenreList.begin(); it != end; it++) {
        if ((*it)->isGenre(genre)) {
            if (mGenreType == G_ALL || (*it)->getGenre() == mGenreType) {
                (*it)->addRadio(new SDeskScanRadio(name, url));
            }
        }
    }

    Log(7, "SDeskScan::AddStation() done\n");
}

void SDeskScan::ReadConfigFile()
{
    Log(7, "SDeskScan::ReadConfigFile()\n");

    // Build the item list.
    FILE *fd;
    char *pos;
    char buf[1024];
    char name[1024];
    char genre[1024];
    char location[1024];
    bool prot;

    strcpy(name, mRootDir);
    strcat(name, "/SDeskScan.cfg");
    if ((fd = fopen(name, "r")) == NULL) {
        Log(1, "fopen failed (%s)...\n", name);
        return;
    }
    
    while ((pos = fgets(buf, 1023, fd)) != 0) {
        if (strcmp(buf, "%security\n") == 0) {
            Log(7, "SDeskScan::ReadConfigFile() security\n");
            mNoSecurity = false;
        } else if (strcmp(buf, "%debug\n") == 0) {
            Log(7, "SDeskScan::ReadConfigFile() debug\n");
            SDeskLog::LogLevel(9);
        } else if (strcmp(buf, "%dobackup\n") == 0) {
            Log(7, "SDeskScan::ReadConfigFile() dobackup\n");
            mDoBackup = true;
        } else if (strcmp(buf, "%nosecurity\n") == 0) {
            Log(7, "SDeskScan::ReadConfigFile() nosecurity\n");
            mNoSecurity = true;
        } else if (strcmp(buf, "%defaultfavorites\n") == 0) {
            Log(7, "SDeskScan::ReadConfigFile() defaultfavorites\n");
            mDefaultFavorites = true;
        } else if (strcmp(buf, "%nodefaultfavorites\n") == 0) {
            Log(7, "SDeskScan::ReadConfigFile() nodefaultfavorites\n");
            mDefaultFavorites = false;
        } else if (strcmp(buf, "%usevlcforradio\n") == 0) {
            Log(7, "SDeskScan::ReadConfigFile() usevlcforradio\n");
            mUseVLCForRadio = true;
            mUseSonicaForRadio = false;
        } else if (strcmp(buf, "%usesonicaforradio\n") == 0) {
            Log(7, "SDeskScan::ReadConfigFile() usesonicaforradio\n");
            mUseVLCForRadio = false;
            mUseSonicaForRadio = true;
        } else if (strcmp(buf, "%dontoverwritevideofavorites\n") == 0) {
            Log(7, "SDeskScan::ReadConfigFile() dontoverwritevideofavorites\n");
            mOverwriteVideoFavorites = false;
        } else if (strcmp(buf, "%homemanagement\n") == 0) {
            Log(7, "SDeskScan::ReadConfigFile() homemanagement\n");
            mHomeManagement = true;
        } else if (strcmp(buf, "%compactrootmenu\n") == 0) {
            Log(7, "SDeskScan::ReadConfigFile() compactrootmenu\n");
            mCompactRootMenu = true;
        } else if (strcmp(buf, "%pictures\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) + strlen(mRootDir);
                    strcpy(location, mRootDir);
                    strcat(location, "/");
                    strcat(location, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(location, buf);
                }
                location[endPos] = 0;
            }
            Log(7, "SDeskScan::ReadConfigFile() pictures\n");
            mScanPictures = false;
            mPictureDirs.insert(mPictureDirs.end(), 
                                new string(location));
        } else if (strcmp(buf, "%movies\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) + strlen(mRootDir);
                    strcpy(location, mRootDir);
                    strcat(location, "/");
                    strcat(location, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(location, buf);
                }
                location[endPos] = 0;
            }
            Log(7, "SDeskScan::ReadConfigFile() videos\n");
            mScanVideos = false;
            mVideoDirs.insert(mVideoDirs.end(), 
                              new string(location));
        } else if (strcmp(buf, "%songs\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) + strlen(mRootDir);
                    strcpy(location, mRootDir);
                    strcat(location, "/");
                    strcat(location, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(location, buf);
                }
                location[endPos] = 0;
            }
            Log(7, "SDeskScan::ReadConfigFile() songs\n");
            mScanMusic = false;
            mMusicDirs.insert(mMusicDirs.end(), 
                              new string(location));
        } else if (strcmp(buf, "%weathersix\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) + strlen(mRootDir);
                    strcpy(location, mRootDir);
                    strcat(location, "/");
                    strcat(location, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(location, buf);
                }
                location[endPos] = 0;
            }
            Log(7, "SDeskScan::ReadConfigFile() weathersix\n");
            strcpy(mWeatherSix, location);
        } else if (strcmp(buf, "%mplay\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) + strlen(mRootDir);
                    strcpy(location, mRootDir);
                    strcat(location, "/");
                    strcat(location, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(location, buf);
                }
                location[endPos] = 0;
            }
            Log(7, "SDeskScan::ReadConfigFile() mplay\n");
            strcpy(mMPlay, location);
        } else if (strcmp(buf, "%radioplayer\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) + strlen(mRootDir);
                    strcpy(location, mRootDir);
                    strcat(location, "/");
                    strcat(location, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(location, buf);
                }
                location[endPos] = 0;
            }
            Log(7, "SDeskScan::ReadConfigFile() radioplayer\n");
            strcpy(mRadioPlayer, location);
        } else if (strcmp(buf, "%musicplayer\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) + strlen(mRootDir);
                    strcpy(location, mRootDir);
                    strcat(location, "/");
                    strcat(location, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(location, buf);
                }
                location[endPos] = 0;
            }
            Log(7, "SDeskScan::ReadConfigFile() musicplayer\n");
            strcpy(mMusicPlayer, location);
        } else if (strcmp(buf, "%pictureviewer\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) + strlen(mRootDir);
                    strcpy(location, mRootDir);
                    strcat(location, "/");
                    strcat(location, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(location, buf);
                }
                location[endPos] = 0;
            }
            Log(7, "SDeskScan::ReadConfigFile() pictureviewer\n");
            strcpy(mPictureViewer, location);
        } else if (strcmp(buf, "%application\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) + strlen(mRootDir);
                    strcpy(location, mRootDir);
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
            Log(7, "SDeskScan::ReadConfigFile() app\n");
            mAppsList.insert(mAppsList.end(), 
                             new SDeskScanApp(name, location, prot));
        } else if (strcmp(buf, "%rssfeed\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                strcpy(location, buf);
                location[strlen(buf) - 1] = 0;
            }
            prot = false;
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                if (strcmp(buf, "1\n") == 0) {
                    prot = true;
                }
            }
            Log(7, "SDeskScan::ReadConfigFile() rss\n");
            mNewsList.insert(mNewsList.end(), 
                             new SDeskScanNews(name, location, prot));
        } else if (strcmp(buf, "%radio\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                strcpy(genre, buf);
                genre[strlen(buf) - 1] = 0;
            }
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) + strlen(mRootDir);
                    strcpy(location, mRootDir);
                    strcat(location, "/");
                    strcat(location, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(location, buf);
                }
                location[endPos] = 0;
            }
            Log(7, "SDeskScan::ReadConfigFile() radio\n");
            AddRadioStation(name, genre, location);
        } else if (strcmp(buf, "%favorite_all\n") == 0 ||
                   strcmp(buf, "%favoritelist\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            Log(7, "SDeskScan::ReadConfigFile() favorite_all\n");
            mFavoriteAll.insert(mFavoriteAll.end(), 
                                new string(name));
        } else if (strcmp(buf, "%favorite_slideshows\n") == 0 ||
                   strcmp(buf, "%slideshow\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            Log(7, "SDeskScan::ReadConfigFile() favorite_slideshows\n");
            mFavoriteSlideShows.insert(mFavoriteSlideShows.end(), 
                                       new string(name));
        } else if (strcmp(buf, "%favorite_songs\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            Log(7, "SDeskScan::ReadConfigFile() favorite_songs\n");
            mFavoriteSongs.insert(mFavoriteSongs.end(), 
                                  new string(name));
        } else if (strcmp(buf, "%favorite_pictures\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            Log(7, "SDeskScan::ReadConfigFile() favorite_pictures\n");
            mFavoritePictures.insert(mFavoritePictures.end(), 
                                     new string(name));
        } else if (strcmp(buf, "%favorite_movies\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            Log(7, "SDeskScan::ReadConfigFile() favorite_movies\n");
            mFavoriteVideos.insert(mFavoriteVideos.end(), 
                                   new string(name));
        } else if (strcmp(buf, "%favorite_radios\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                strcpy(name, buf);
                name[strlen(buf) - 1] = 0;
            }
            Log(7, "SDeskScan::ReadConfigFile() favorite_radios\n");
            mFavoriteRadios.insert(mFavoriteRadios.end(), 
                                   new string(name));
        } else if (strcmp(buf, "%status1\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) + strlen(mRootDir);
                    strcpy(location, mRootDir);
                    strcat(location, "/");
                    strcat(location, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(location, buf);
                }
                location[endPos] = 0;
            }
            Log(7, "SDeskScan::ReadConfigFile() status\n");
            strcpy(mStatus1Cmd, location);
        } else if (strcmp(buf, "%status1interval\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mStatus1Int = atol(buf);
            }
        } else if (strcmp(buf, "%status2\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) + strlen(mRootDir);
                    strcpy(location, mRootDir);
                    strcat(location, "/");
                    strcat(location, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(location, buf);
                }
                location[endPos] = 0;
            }
            Log(7, "SDeskScan::ReadConfigFile() status\n");
            strcpy(mStatus2Cmd, location);
        } else if (strcmp(buf, "%status2interval\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mStatus2Int = atol(buf);
            }
        } else if (strcmp(buf, "%status3\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) + strlen(mRootDir);
                    strcpy(location, mRootDir);
                    strcat(location, "/");
                    strcat(location, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(location, buf);
                }
                location[endPos] = 0;
            }
            Log(7, "SDeskScan::ReadConfigFile() status\n");
            strcpy(mStatus3Cmd, location);
        } else if (strcmp(buf, "%status3interval\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mStatus3Int = atol(buf);
            }
        } else if (strcmp(buf, "%status4\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) + strlen(mRootDir);
                    strcpy(location, mRootDir);
                    strcat(location, "/");
                    strcat(location, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(location, buf);
                }
                location[endPos] = 0;
            }
            Log(7, "SDeskScan::ReadConfigFile() status\n");
            strcpy(mStatus4Cmd, location);
        } else if (strcmp(buf, "%status4interval\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mStatus4Int = atol(buf);
            }
        } else if (strcmp(buf, "%custominfo\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) + strlen(mRootDir);
                    strcpy(mCustomInfo, mRootDir);
                    strcat(mCustomInfo, "/");
                    strcat(mCustomInfo, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(mCustomInfo, buf);
                }
                mCustomInfo[endPos] = 0;
            }
        } else if (strcmp(buf, "%customrotate\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) + strlen(mRootDir);
                    strcpy(mCustomRotate, mRootDir);
                    strcat(mCustomRotate, "/");
                    strcat(mCustomRotate, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(mCustomRotate, buf);
                }
                mCustomRotate[endPos] = 0;
            }
        } else if (strcmp(buf, "%customff\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) + strlen(mRootDir);
                    strcpy(mCustomFF, mRootDir);
                    strcat(mCustomFF, "/");
                    strcat(mCustomFF, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(mCustomFF, buf);
                }
                mCustomFF[endPos] = 0;
            }
        } else if (strcmp(buf, "%customfb\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                int endPos = 0;
                if (buf[0] != '/') {
                    endPos = strlen(buf) + strlen(mRootDir);
                    strcpy(mCustomFB, mRootDir);
                    strcat(mCustomFB, "/");
                    strcat(mCustomFB, buf);
                } else {
                    endPos = strlen(buf) - 1;
                    strcpy(mCustomFB, buf);
                }
                mCustomFB[endPos] = 0;
            }
        } else if (strcmp(buf, "%excluded\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mExcludedList.insert(mExcludedList.end(), 
                                      new string(buf));
            }
        } else if (strcmp(buf, "%musicextension\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mMusicExtension.insert(mMusicExtension.end(), 
                                       new string(buf));
            }
        } else if (strcmp(buf, "%pictureextension\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mPictureExtension.insert(mPictureExtension.end(), 
                                         new string(buf));
            }
        } else if (strcmp(buf, "%videoextension\n") == 0) {
            if ((pos = fgets(buf, 1023, fd)) != 0) {
                buf[strlen(buf) - 1] = 0;
                mVideoExtension.insert(mVideoExtension.end(), 
                                       new string(buf));
            }
        }
    }

    fclose(fd);
                                                    
    Log(7, "SDeskScan::ReadConfigFile() done\n");
}

void SDeskScan::FindMusicExtensions()
{
   CascadeDirScanner codec_dir("/etc/rokucascade/codecs", 
         "libCascadeAudioCodec"); 
   int codecs, c; 
   char *s, *tmp, *t; 

   if (mMusicExtension.size() != 0) {
       // If we haven't gotten any extensions in the config file...
       // try to find them.
       return;
   }

   /* 
    * this is kind of a hack to get the list of supported codecs on 
    * the system. Since there doesn't seem to be a way to query the 
    * audioPlayer for the list, we'll just read /etc/rokucascasde/codecs 
    * and see what's there.  Currently the codecs are named: 
    *     libCascadeAudioCodecXXX.so 
    */ 
   codecs = codec_dir.GetNumEntries(); 
   codecs = (codecs > 25) ? 25 : codecs; /* support 25 max for now */ 
   for (c = 0; c < codecs; c++) { 
      s = (char *) codec_dir.GetEntry(c); 
      s += 20; /* Should be pointing at start of Codec name */ 
      tmp = (char *) malloc(strlen(s) + 2); 
      t = tmp; 
      *t++ = '.';
      while (s) { 
         if (*s == '.') { 
            *t = '\0'; 
            break; 
         } 
         *t++ = tolower(*s++);
      } 
      mMusicExtension.push_back(new string(tmp));
      free(tmp); 
   }

   if (mMusicExtension.size() != 0) {
       // If we haven't gotten any extensions in the config file...
       // try to find them.
       return;
   }

   // Fallback...
   mMusicExtension.push_back(new string(".mp3"));
}

bool SDeskScan::IsPicture(
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

bool SDeskScan::IsMusic(
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

bool SDeskScan::IsVideo(
    const char* name,
    unsigned int *extPos)
{
    char buffer[1024];
    const char *fromPos = name;
    char *toPos = buffer;
    vector<string *>::const_iterator it, end = mVideoExtension.end();

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

void SDeskScan::FixFileName(
    char* name)
{
    for (unsigned int counter = 0; counter < strlen(name); counter++) {
        if (name[counter] == '/' || name[counter] == '"' ||
            name[counter] == '\\' || name[counter] == '\'' ||
            name[counter] == '&' || name[counter] == '(' ||
            name[counter] == ')' || name[counter] == '*' ||
            name[counter] == '?' || name[counter] == ':' ||
            !isascii(name[counter])) {
            name[counter] = '_';
        }
    }
}

void SDeskScan::AddSongByTag(
    const char* name,
    const char* fileName)
{
    int len = 0;
    char buffer[1024];

    CascadeFile file(fileName);
    CascadeFileStream fstream;

    fstream.Open(file);
    TagProcessor * myTagTest = new TagProcessor();

    if(myTagTest->SetInputStream(&fstream, "tta")) {
        // Add by genre...
        vector<SDeskScanSongByTag *>::iterator it, 
            end = mSongGenreTagList.end();
            
        // Get the tag...
        len = strlen(myTagTest->GetGenre());
        if (len > 1023) {
            len = 1023;
        }
        strncpy(buffer, myTagTest->GetGenre(), len);
        buffer[len] = 0;
        if (len == 0) {
            strcpy(buffer, "Other");
        }
        
        // Locate the correct song list.
        for(it = mSongGenreTagList.begin(); it != end; it++) {
            if (strcmp((*it)->getName(), buffer) == 0) {
                break;
            }
        }
        if (it == end) {
            // Create a new song list for the genre.
            mSongGenreTagList.insert(mSongGenreTagList.end(),
                                     new SDeskScanSongByTag(buffer));
            it = mSongGenreTagList.end() - 1;
        }
        // Insert song in the list.
        (*it)->addSong(new SDeskScanSong(name, fileName));

        // Add by artist...
        end = mSongArtistTagList.end();
            
        // Get the tag...
        len = strlen(myTagTest->GetArtist());
        if (len > 1023) {
            len = 1023;
        }
        strncpy(buffer, myTagTest->GetArtist(), len);
        buffer[len] = 0;
        if (len == 0) {
            strcpy(buffer, "Unknown");
        }
        
        // Locate the correct song list.
        for(it = mSongArtistTagList.begin(); it != end; it++) {
            if (strcmp((*it)->getName(), buffer) == 0) {
                break;
            }
        }
        if (it == end) {
            // Create a new song list for the artist.
            mSongArtistTagList.insert(mSongArtistTagList.end(),
                                     new SDeskScanSongByTag(buffer));
            it = mSongArtistTagList.end() - 1;
        }
        // Insert song in the list.
        (*it)->addSong(new SDeskScanSong(name, fileName));

        // Add by album...
        end = mSongAlbumTagList.end();
            
        // Get the tag...
        len = strlen(myTagTest->GetAlbum());
        if (len > 1023) {
            len = 1023;
        }
        strncpy(buffer, myTagTest->GetAlbum(), len);
        buffer[len] = 0;
        if (len == 0) {
            strcpy(buffer, "Unknown");
        }
        
        // Locate the correct song list.
        for(it = mSongAlbumTagList.begin(); it != end; it++) {
            if (strcmp((*it)->getName(), buffer) == 0) {
                break;
            }
        }
        if (it == end) {
            // Create a new song list for the album.
            mSongAlbumTagList.insert(mSongAlbumTagList.end(),
                                     new SDeskScanSongByTag(buffer));
            it = mSongAlbumTagList.end() - 1;
        }
        // Insert song in the list.
        (*it)->addSong(new SDeskScanSong(name, fileName));

        // Add by year...
        end = mSongYearTagList.end();
            
        // Get the tag...
        len = strlen(myTagTest->GetYear());
        if (len > 1023) {
            len = 1023;
        }
        strncpy(buffer, myTagTest->GetYear(), len);
        buffer[len] = 0;
        if (len == 0) {
            strcpy(buffer, "Unknown");
        }
        
        // Locate the correct song list.
        for(it = mSongYearTagList.begin(); it != end; it++) {
            if (strcmp((*it)->getName(), buffer) == 0) {
                break;
            }
        }
        if (it == end) {
            // Create a new song list for the year.
            mSongYearTagList.insert(mSongYearTagList.end(),
                                     new SDeskScanSongByTag(buffer));
            it = mSongYearTagList.end() - 1;
        }
        // Insert song in the list.
        (*it)->addSong(new SDeskScanSong(name, fileName));
    }
}
