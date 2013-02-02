#include <unistd.h>

#include "SDesk.h"
#include "SDeskItemMovieInfo.h"

// Positions and sizes for the various screen elements
#define LABEL_X 30
#define LABEL_WIDTH 100
#define LABEL_HEIGHT 20
#define VALUE_X 170
#define VALUE_OFFSET 20

#define TITLE_Y 90
#define TITLE_HEIGHT 20

#define SUMMARY_Y (TITLE_Y + TITLE_HEIGHT + VALUE_OFFSET)
#define SUMMARY_HEIGHT 100

#define GENRE_Y (SUMMARY_Y + SUMMARY_HEIGHT + VALUE_OFFSET)
#define GENRE_HEIGHT 20

#define YEAR_Y (GENRE_Y + GENRE_HEIGHT + VALUE_OFFSET)
#define YEAR_HEIGHT 20

#define RATING_Y (YEAR_Y + YEAR_HEIGHT + VALUE_OFFSET)
#define RATING_HEIGHT 20

#define DIRECTOR_Y (RATING_Y + RATING_HEIGHT + VALUE_OFFSET)
#define DIRECTOR_HEIGHT 20

#define WRITER_Y (DIRECTOR_Y + DIRECTOR_HEIGHT + VALUE_OFFSET)
#define WRITER_HEIGHT 40

#define ACTORS_Y (WRITER_Y + WRITER_HEIGHT + VALUE_OFFSET)
#define ACTORS_HEIGHT 80


SDeskItemMovieInfo::SDeskItemMovieInfo(
    const char* name,
    const char* filename)
    : SDeskItem("", filename, false)
{
    Log(7, "SDeskItemMovieInfo(%s,%s)\n", name, filename);

    // Track this items type.
    mType = I_MOVIE_INFO;

    mName = "Movie Info";

    Log(7, "SDeskItemMovieInfo() done\n");
}

SDeskItemMovieInfo::~SDeskItemMovieInfo()
{
    Log(7, "~SDeskItemMovieInfo()\n");

    Log(7, "~SDeskItemMovieInfo() done\n");
}

void SDeskItemMovieInfo::DrawMessage(
    CascadeBitmap& bitmap)
{
    const char* str = mName;
    SDeskBitmap b;
    CascadeRect r;
    CascadeString label;
    CascadeString value;
    FILE *fd;
    char *pos;
    char buf[1024];
    int valueWidth = 0;
    int valueY = 0;
    int valueHeight = 0;

    Log(7, "SDeskItemMovieInfo::DrawMessage(%s)\n", str);
    strcpy(buf, mFilename);
    pos = strrchr(buf, '.');
    if (pos) {
        // found a file extension
        *pos = 0;
    }
    strcat(buf, ".nfo");
    Log(7, "SDeskItemMovieInfo::DrawMessage(%s)\n", buf);
    if ((fd = fopen(buf, "r")) == NULL) {
        strcpy(buf, mFilename);
        pos = strrchr(buf, '.');
        if (pos) {
            // found a file extension
            *pos = 0;
        }
        strcat(buf, ".my");
        Log(7, "SDeskItemMovieInfo::DrawMessage(%s)\n", buf);
        if ((fd = fopen(buf, "r")) == NULL) {
            strcpy(buf, mFilename);
            pos = strrchr(buf, '.');
            if (pos) {
                // found a file extension
                *pos = 0;
            }
            strcat(buf, ".txt");
            Log(7, "SDeskItemMovieInfo::DrawMessage(%s)\n", buf);
            if ((fd = fopen(buf, "r")) == NULL) {
                strcpy(buf, mFilename);
                strcat(buf, "/folder.nfo");
                Log(7, "SDeskItemMovieInfo::DrawMessage(%s)\n", buf);
                if ((fd = fopen(buf, "r")) == NULL) {
                    strcpy(buf, mFilename);
                    strcat(buf, "/folder.my");
                    Log(7, "SDeskItemMovieInfo::DrawMessage(%s)\n", buf);
                    if ((fd = fopen(buf, "r")) == NULL) {
                        strcpy(buf, mFilename);
                        strcat(buf, "/folder.txt");
                        Log(7, "SDeskItemMovieInfo::DrawMessage(%s)\n", buf);
                        if ((fd = fopen(buf, "r")) == NULL) {
                            strcpy(buf, mFilename);
                            pos = strrchr(buf, '/');
                            if (pos) {
                                // found a file extension
                                *pos = 0;
                                if (pos) {
                                    if (strcmp(pos, "/VIDEO_TS") == 0) {
                                        *pos = 0;
                                    }
                                }
                            }
                            strcat(buf, ".nfo");
                            Log(7, "SDeskItemMovieInfo::DrawMessage(%s)\n", buf);
                            if ((fd = fopen(buf, "r")) == NULL) {
                                strcpy(buf, mFilename);
                                pos = strrchr(buf, '/');
                                if (pos) {
                                    // found a file extension
                                    *pos = 0;
                                    if (strcmp(pos, "/VIDEO_TS") == 0) {
                                        *pos = 0;
                                    }
                                }
                                strcat(buf, ".my");
                                Log(7, "SDeskItemMovieInfo::DrawMessage(%s)\n", buf);
                                if ((fd = fopen(buf, "r")) == NULL) {
                                    strcpy(buf, mFilename);
                                    pos = strrchr(buf, '/');
                                    if (pos) {
                                        // found a file extension
                                        *pos = 0;
                                        if (strcmp(pos, "/VIDEO_TS") == 0) {
                                            *pos = 0;
                                        }
                                    }
                                    strcat(buf, ".txt");
                                    Log(7, "SDeskItemMovieInfo::DrawMessage(%s)\n", buf);
                                    if ((fd = fopen(buf, "r")) == NULL) {
                                        strcpy(buf, mFilename);
                                        pos = strrchr(buf, '/');
                                        if (pos) {
                                            // found a file extension
                                            *pos = 0;
                                        }
                                        strcat(buf, "/folder.nfo");
                                        Log(7, "SDeskItemMovieInfo::DrawMessage(%s)\n", buf);
                                        if ((fd = fopen(buf, "r")) == NULL) {
                                            strcpy(buf, mFilename);
                                            pos = strrchr(buf, '/');
                                            if (pos) {
                                                // found a file extension
                                                *pos = 0;
                                                if (strcmp(pos, "/VIDEO_TS") == 0) {
                                                    *pos = 0;
                                                }
                                            }
                                            strcat(buf, "/folder.my");
                                            Log(7, "SDeskItemMovieInfo::DrawMessage(%s)\n", buf);
                                            if ((fd = fopen(buf, "r")) == NULL) {
                                                strcpy(buf, mFilename);
                                                pos = strrchr(buf, '/');
                                                if (pos) {
                                                    // found a file extension
                                                    *pos = 0;
                                                    if (strcmp(pos, "/VIDEO_TS") == 0) {
                                                        *pos = 0;
                                                    }
                                                }
                                                strcat(buf, "/folder.txt");
                                                Log(7, "SDeskItemMovieInfo::DrawMessage(%s)\n", buf);
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
            }
        }
    }

    // Compute the width for the value items.
    valueWidth = SDesk::getInstance().getCurrentTheme().getScreenWidth() -
        LABEL_WIDTH - 40;

    while ((pos = fgets(buf, 1023, fd)) != 0) {
        buf[strlen(buf) - 1] = 0;
        if (buf[strlen(buf) - 1] == '\n' ||
            buf[strlen(buf) - 1] == '\r') {
            buf[strlen(buf) - 1] = 0;
        }

        valueY = 0;
        valueHeight = 0;
        if (strncmp(pos, "title=", 6) == 0 ||
            strncmp(pos, "TVEpisode=", 10) == 0) {
            pos = strchr(pos, '=');
            pos++;
            label = CascadeString("Title:");
            value = CascadeString(pos);
            valueY = TITLE_Y;
            valueHeight = TITLE_HEIGHT;
        } else if (strncmp(pos, "summary=", 8) == 0 ||
                   strncmp(pos, "TVOverview=", 11) == 0) {
            pos = strchr(pos, '=');
            pos++;
            label = CascadeString("Summary:");
            value = CascadeString(pos);
            valueY = SUMMARY_Y;
            valueHeight = SUMMARY_HEIGHT;
        } else if (strncmp(pos, "genre=", 6) == 0 ||
                   strncmp(pos, "TVGenre=", 8) == 0) {
            pos = strchr(pos, '=');
            pos++;
            label = CascadeString("Genre:");
            value = CascadeString(pos);
            valueY = GENRE_Y;
            valueHeight = GENRE_HEIGHT;
        } else if (strncmp(pos, "year=", 5) == 0 ||
                   strncmp(pos, "TVOriginalAiringDate=", 21) == 0) {
            pos = strchr(pos, '=');
            pos++;
            label = CascadeString("Year:");
            value = CascadeString(pos);
            valueY = YEAR_Y;
            valueHeight = YEAR_HEIGHT;
        } else if (strncmp(pos, "rating=", 7) == 0 ||
                   strncmp(pos, "TVParentalRatings=", 18) == 0) {
            pos = strchr(pos, '=');
            pos++;
            label = CascadeString("Rating:");
            value = CascadeString(pos);
            valueY = RATING_Y;
            valueHeight = RATING_HEIGHT;
        } else if (strncmp(pos, "director=", 9) == 0) {
            pos = strchr(pos, '=');
            pos++;
            label = CascadeString("Director:");
            value = CascadeString(pos);
            valueY = DIRECTOR_Y;
            valueHeight = DIRECTOR_HEIGHT;
        } else if (strncmp(pos, "writer=", 7) == 0) {
            pos = strchr(pos, '=');
            pos++;
            label = CascadeString("Writer:");
            value = CascadeString(pos);
            valueY = WRITER_Y;
            valueHeight = WRITER_HEIGHT;
        } else if (strncmp(pos, "actors=", 7) == 0 ||
                   strncmp(pos, "TVActors=", 9) == 0) {
            if (strncmp(pos, "TVActors=", 9) == 0) {
                valueY = DIRECTOR_Y;
            } else {
                valueY = ACTORS_Y;
            }
            pos = strchr(pos, '=');
            pos++;
            label = CascadeString("Actors:");
            value = CascadeString(pos);
            valueHeight = ACTORS_HEIGHT;
        }

        if (valueY != 0) {
            // This is the rectangle we draw in.
            r = CascadeRect(LABEL_X, valueY,
                            LABEL_WIDTH, LABEL_HEIGHT);

            // Show the label..
            bitmap.DrawText(
                SDesk::getInstance().getCurrentTheme().getNormalFont(),
                r, label,
                SDesk::getInstance().getCurrentTheme().getNormalColor(),
                TEXT_JUSTIFY_TOP |
                TEXT_JUSTIFY_LEFT);

            r = CascadeRect(VALUE_X, valueY, valueWidth, valueHeight);
            
            // Shopw the actual value...
            bitmap.DrawText(
                SDesk::getInstance().getCurrentTheme().getNormalFont(),
                r, value,
                SDesk::getInstance().getCurrentTheme().getNormalColor(),
                TEXT_JUSTIFY_TOP |
                TEXT_JUSTIFY_LEFT | TEXT_WORD_WRAP);
        }
    }

    fclose(fd);

    Log(7, "SDeskItemMovieInfo::DrawMessage() done\n");
}

bool SDeskItemMovieInfo::OnKeyDown(u32 nKey)
{
    Log(7, "SDeskItemMovieInfo::OnKeyDown(%d)\n", nKey);

    Log(7, "SDeskItemMovieInfo::OnKeyDown() done\n");
    return false;
}
