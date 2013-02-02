#include <cascade/util/CascadeFileStream.h> 

#include <unistd.h>

#include <id3/id3lib_streams.h>
#include <id3/tag.h>
#include <id3/misc_support.h>

#include "SDesk.h"
#include "SDeskItem.h"

SDeskItem::SDeskItem(
    const char* name,
    const char* filename,
    bool prot)
    : mItemDefinition(0),
      mSelected(false),
      mTopOffset(0),
      mProtected(prot)
{
    Log(7, "SDeskItem(%s,%s)\n", name, filename);

    // Store this item's properties.
    mName = CascadeString(name);
    mFilename = CascadeString(filename);

    Log(7, "SDeskItem() done\n");
}

SDeskItem::~SDeskItem()
{
    Log(7, "~SDeskItem()\n");

    Log(7, "~SDeskItem() done\n");
}

void SDeskItem::DrawText(
    CascadeBitmap& bitmap,
    int x,
    int y,
    int menuType)
{
    const char* str = mName;
    SDeskBitmap b;
    int x0, y0;

    Log(7, "SDeskItem::DrawText(%s)\n", str);

    // This is the rectangle we draw in.
    CascadeRect r = CascadeRect(x, y,
             SDesk::getInstance().getCurrentTheme().getTextCellWidth(),
             SDesk::getInstance().getCurrentTheme().getTextCellHeight());

    // The item is not selected.
    bitmap.DrawText(
        SDesk::getInstance().getCurrentTheme().getNormalFont(),
        r, mName,
        SDesk::getInstance().getCurrentTheme().getNormalColor(),
        TEXT_CENTER_VERTICALLY |
        TEXT_JUSTIFY_LEFT | TEXT_CLIP_WITH_DOTDOTDOT);

    if (mSelected/* && strncmp(mFilename, "http://", 7) != 0*/) {
        // Draw the items image...
        bool foundIt = false;
        char buf[1024];
        char *pos;
        char *slashPos;

        // Find the extension and replace it.
        strcpy(buf, mFilename);
        slashPos = strrchr(buf, '/');
        pos = strrchr(buf, '.');
        if (pos && slashPos < pos) {
            *pos = 0;
        }
        strcat(buf, ".sdthm");
        Log(7, "SDeskItem::DrawText(%s)\n", buf);
        if (b.Load(buf, 0)) {
            foundIt = true;
        }

        // Strip the directory off, including DVD folders.
        if (!foundIt) {
            strcpy(buf, mFilename);
            pos = strrchr(buf, '/');
            if (pos) {
                *pos = 0;
                pos = strrchr(buf, '/');
                if (pos) {
                    if (strcmp(pos, "/VIDEO_TS") == 0) {
                        *pos = 0;
                    }
                }
            }
            strcat(buf, ".sdthm");
            Log(7, "SDeskItem::DrawText(%s)\n", buf);
            if (b.Load(buf, 0)) {
                foundIt = true;
            }
        }

        // Find the .thm file
        if (!foundIt && !SDesk::getInstance().getJustSDTHM()) {
            strcpy(buf, mFilename);
            slashPos = strrchr(buf, '/');
            pos = strrchr(buf, '.');
            if (pos && slashPos < pos) {
                *pos = 0;
            }
            strcat(buf, ".thm");
            Log(7, "SDeskItem::DrawText(%s)\n", buf);
            if (b.Load(buf, 0)) {
                foundIt = true;
            }
        }

        // See if this is an image file.
        if (!foundIt) {
            unsigned int extPos = 0;
            if (SDesk::getInstance().IsPicture(mFilename, &extPos) &&
                SDesk::getInstance().getShowThumb()) {
                if (b.Load(mFilename, 0)) {
                    foundIt = true;
                }
            }
        }
    
        // Look for some additional special files.
        if (!SDesk::getInstance().getJustSDTHM()) {
            // See of there is a special icon.
            if (!foundIt) {
                strcpy(buf, mFilename);
                slashPos = strrchr(buf, '/');
                pos = strrchr(buf, '.');
                if (pos && slashPos < pos) {
                    *pos = 0;
                }
                strcat(buf, ".jpg");
                Log(7, "SDeskItem::DrawText(%s)\n", buf);
                if (b.Load(buf, 0)) {
                    foundIt = true;
                }
            }
            if (!foundIt) {
                // Look in subdirectory.
                strcpy(buf, mFilename);
                pos = strrchr(buf, '/');
                if (pos) {
                    *pos = 0;
                    pos = strrchr(buf, '/');
                    if (pos) {
                        if (strcmp(pos, "/VIDEO_TS") == 0) {
                            *pos = 0;
                        }
                    }
                }
                strcat(buf, "/folder.jpg");
                Log(7, "SDeskItem::DrawText(%s)\n", buf);
                if (b.Load(buf, 0)) {
                    foundIt = true;
                }
            }
            if (!foundIt) {
                // Look in subdirectory.
                strcpy(buf, mFilename);
                strcat(buf, "/folder.jpg");
                Log(7, "SDeskItem::DrawText(%s)\n", buf);
                if (b.Load(buf, 0)) {
                    foundIt = true;
                }
            }
            if (!foundIt) {
                strcpy(buf, mFilename);
                pos = strrchr(buf, '/');
                if (pos) {
                    *pos = 0;
                    pos = strrchr(buf, '/');
                    if (pos) {
                        if (strcmp(pos, "/VIDEO_TS") == 0) {
                            *pos = 0;
                        }
                    }
                }
                strcat(buf, "/folder.thm");
                Log(7, "SDeskItem::DrawText(%s)\n", buf);
                if (b.Load(buf, 0)) {
                    foundIt = true;
                }
            }
            if (!foundIt) {
                strcpy(buf, mFilename);
                strcat(buf, "/folder.thm");
                Log(7, "SDeskItem::DrawText(%s)\n", buf);
                if (b.Load(buf, 0)) {
                    foundIt = true;
                }
            }
            if (!foundIt) {
                strcpy(buf, mFilename);
                pos = strrchr(buf, '/');
                if (pos) {
                    *pos = 0;
                    pos = strrchr(buf, '/');
                    if (pos) {
                        if (strcmp(pos, "/VIDEO_TS") == 0) {
                            *pos = 0;
                        }
                    }
                }
                strcat(buf, "/cover.jpg");
                Log(7, "SDeskItem::DrawText(%s)\n", buf);
                if (b.Load(buf, 0)) {
                    foundIt = true;
                }
            }
            if (!foundIt) {
                strcpy(buf, mFilename);
                strcat(buf, "/cover.jpg");
                Log(7, "SDeskItem::DrawText(%s)\n", buf);
                if (b.Load(buf, 0)) {
                    foundIt = true;
                }
            }
            if (!foundIt) {
                strcpy(buf, mFilename);
                pos = strrchr(buf, '/');
                if (pos) {
                    *pos = 0;
                    pos = strrchr(buf, '/');
                    if (pos) {
                        if (strcmp(pos, "/VIDEO_TS") == 0) {
                            *pos = 0;
                        }
                    }
                }
                strcat(buf, "/cover.thm");
                Log(7, "SDeskItem::DrawText(%s)\n", buf);
                if (b.Load(buf, 0)) {
                    foundIt = true;
                }
            }
            if (!foundIt) {
                strcpy(buf, mFilename);
                strcat(buf, "/cover.thm");
                Log(7, "SDeskItem::DrawText(%s)\n", buf);
                if (b.Load(buf, 0)) {
                    foundIt = true;
                }
            }
        }

        // Look in the central images directory
        if (!foundIt) {
            char tmpBuf[1024];

            slashPos = strrchr(mFilename, '/');
            strcpy(tmpBuf, SDesk::getInstance().getImagesDir());
            if (slashPos && *slashPos) {
                strcat(tmpBuf, slashPos);
            } else {
                strcat(tmpBuf, mFilename);
            }
            slashPos = strrchr(tmpBuf, '/');
            pos = strrchr(tmpBuf, '.');
            if (pos && slashPos < pos) {
                *pos = 0;
            }
            strcat(tmpBuf, ".sdthm");
            Log(7, "SDeskItem::DrawText(%s)\n", tmpBuf);
            if (b.Load(tmpBuf, 0)) {
                foundIt = true;
            }
        }

        if (!foundIt) {
            // Use embedded image, if there is one.
            unsigned int extPos = 0;
            const char *fileName = mFilename;
            if (SDesk::getInstance().IsMusic(fileName, &extPos)) {
                ID3_Tag tag(fileName);
                const ID3_Frame* frame = tag.Find(ID3FID_PICTURE);
                Log(7, "SDeskItem::DrawText(%s) embedded\n", fileName);
                if (frame && frame->Contains(ID3FN_DATA)) {
                    char buffer[1024];

                    strcpy(buffer, SDesk::getInstance().getTmpDir());
                    strcat(buffer, "/sdeskpic.jpg");
                    Log(7, "SDeskItem::DrawText(%s) embedded found\n", buffer);
                    unlink(buffer);
                    frame->Field(ID3FN_DATA).ToFile(buffer);
                    if (b.Load(buffer, 0)) {
                        foundIt = true;
                    } else {
                        strcpy(buffer, SDesk::getInstance().getTmpDir());
                        strcat(buffer, "/sdeskpic.png");
                        Log(7, "SDeskItem::DrawText(%s) embedded found\n", buffer);
                        unlink(buffer);
                        frame->Field(ID3FN_DATA).ToFile(buffer);
                        if (b.Load(buffer, 0)) {
                            foundIt = true;
                        }
                    }
                    unlink(buffer);
                }
            }
        }

        if (foundIt) {
            x0 = SDesk::getInstance().getCurrentTheme().getScreenWidth()
                - b.GetWidth()
                - SDesk::getInstance().getCurrentTheme().getImageXOffset();
            y0 = SDesk::getInstance().getCurrentTheme().getImageYOffset();
        
            Log(7, "SDeskItem::DrawText(%ld, %ld, %ld, %ld) \n",
                x0, y0, b.GetWidth(), b.GetHeight());
            b.Draw(bitmap, x0, y0, b.GetWidth(), b.GetHeight());
        } else {
            Log(7, "SDeskItem::DrawText() default\n");
            SDesk::getInstance().getCurrentTheme().DrawType(
                bitmap, mType, menuType, 0, 0);
        }

        Log(7, "SDeskItem::DrawText()\n");
    }

    Log(7, "SDeskItem::DrawText() done\n");
}

void SDeskItem::DrawDetailText(
    CascadeBitmap& bitmap)
{
    const char* str = mName;

    Log(7, "SDeskItem::DrawDetailText(%s)\n", str);

    if (mDetailText == 0) {
        // Load the detail text from the nfo file.
        FILE *fd;
        char *pos;
        char *slashPos;
        char buf[1024];

        strcpy(buf, mFilename);
        Log(7, "SDeskItem::DrawDetailText(%s) filename\n", buf);
        slashPos = strrchr(buf, '/');
        pos = strrchr(buf, '.');
        if (pos && slashPos < pos) {
            // found a file extension
            *pos = 0;
        }
        strcat(buf, ".nfo");
        Log(7, "SDeskItem::DrawDetailText(%s)\n", buf);
        if ((fd = fopen(buf, "r")) == NULL) {
            strcpy(buf, mFilename);
            slashPos = strrchr(buf, '/');
            pos = strrchr(buf, '.');
            if (pos && slashPos < pos) {
                // found a file extension
                *pos = 0;
            }
            strcat(buf, ".my");
            Log(7, "SDeskItem::DrawDetailText(%s)\n", buf);
            if ((fd = fopen(buf, "r")) == NULL) {
                strcpy(buf, mFilename);
                slashPos = strrchr(buf, '/');
                pos = strrchr(buf, '.');
                if (pos && slashPos < pos) {
                    // found a file extension
                    *pos = 0;
                }
                strcat(buf, ".txt");
                Log(7, "SDeskItem::DrawDetailText(%s)\n", buf);
                if ((fd = fopen(buf, "r")) == NULL) {
                    strcpy(buf, mFilename);
                    strcat(buf, "/folder.nfo");
                    Log(7, "SDeskItem::DrawDetailText(%s)\n", buf);
                    if ((fd = fopen(buf, "r")) == NULL) {
                        strcpy(buf, mFilename);
                        strcat(buf, "/folder.my");
                        Log(7, "SDeskItem::DrawDetailText(%s)\n", buf);
                        if ((fd = fopen(buf, "r")) == NULL) {
                            strcpy(buf, mFilename);
                            strcat(buf, "/folder.txt");
                            Log(7, "SDeskItem::DrawDetailText(%s)\n", buf);
                            if ((fd = fopen(buf, "r")) == NULL) {
                                strcpy(buf, mFilename);
                                pos = strrchr(buf, '/');
                                if (pos) {
                                    // found a file extension
                                    *pos = 0;
                                    pos = strrchr(buf, '/');
                                    if (pos) {
                                        if (strcmp(pos, "/VIDEO_TS") == 0) {
                                            *pos = 0;
                                        }
                                    }
                                }
                                strcat(buf, ".nfo");
                                Log(7, "SDeskItem::DrawDetailText(%s)\n", buf);
                                if ((fd = fopen(buf, "r")) == NULL) {
                                    strcpy(buf, mFilename);
                                    pos = strrchr(buf, '/');
                                    if (pos) {
                                        // found a file extension
                                        *pos = 0;
                                        pos = strrchr(buf, '/');
                                        if (pos) {
                                            if (strcmp(pos, "/VIDEO_TS") == 0) {
                                                *pos = 0;
                                            }
                                        }
                                    }
                                    strcat(buf, ".my");
                                    Log(7, "SDeskItem::DrawDetailText(%s)\n", buf);
                                    if ((fd = fopen(buf, "r")) == NULL) {
                                        strcpy(buf, mFilename);
                                        pos = strrchr(buf, '/');
                                        if (pos) {
                                            // found a file extension
                                            *pos = 0;
                                            pos = strrchr(buf, '/');
                                            if (pos) {
                                                if (strcmp(pos, "/VIDEO_TS") == 0) {
                                                    *pos = 0;
                                                }
                                            }
                                        }
                                        strcat(buf, ".txt");
                                        Log(7, "SDeskItem::DrawDetailText(%s)\n", buf);
                                        if ((fd = fopen(buf, "r")) == NULL) {
                                            strcpy(buf, mFilename);
                                            pos = strrchr(buf, '/');
                                            if (pos) {
                                                // found a file extension
                                                *pos = 0;
                                                pos = strrchr(buf, '/');
                                                if (pos) {
                                                    if (strcmp(pos, "/VIDEO_TS") == 0) {
                                                        *pos = 0;
                                                    }
                                                }
                                            }
                                            strcat(buf, "/folder.nfo");
                                            Log(7, "SDeskItem::DrawDetailText(%s)\n", buf);
                                            if ((fd = fopen(buf, "r")) == NULL) {
                                                strcpy(buf, mFilename);
                                                pos = strrchr(buf, '/');
                                                if (pos) {
                                                    // found a file extension
                                                    *pos = 0;
                                                    pos = strrchr(buf, '/');
                                                    if (pos) {
                                                        if (strcmp(pos, "/VIDEO_TS") == 0) {
                                                            *pos = 0;
                                                        }
                                                    }
                                                }
                                                strcat(buf, "/folder.my");
                                                Log(7, "SDeskItem::DrawDetailText(%s)\n", buf);
                                                if ((fd = fopen(buf, "r")) == NULL) {
                                                    strcpy(buf, mFilename);
                                                    pos = strrchr(buf, '/');
                                                    if (pos) {
                                                        // found a file extension
                                                        *pos = 0;
                                                        pos = strrchr(buf, '/');
                                                        if (pos) {
                                                            if (strcmp(pos, "/VIDEO_TS") == 0) {
                                                                *pos = 0;
                                                            }
                                                        }
                                                    }
                                                    strcat(buf, "/folder.txt");
                                                    Log(7, "SDeskItem::DrawDetailText(%s)\n", buf);
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

        while ((pos = fgets(buf, 1023, fd)) != 0) {
            // Summary?
            if (strncmp(pos, "summary=", 8) == 0 ||
                strncmp(pos, "TVOverview=", 11) == 0) {
                buf[strlen(buf) - 1] = 0;
                if (buf[strlen(buf) - 1] == '\n' ||
                    buf[strlen(buf) - 1] == '\r') {
                    buf[strlen(buf) - 1] = 0;
                }
                pos = strchr(pos, '=');
                pos++;
                mDetailText = CascadeString(pos);
                break;
            }
        }

        fclose(fd);
    }

    if (mDetailText != 0) {
        const char* str = mDetailText;

        Log(7, "SDeskItem::DrawDetailText(%s)\n", str);

        // This is the rectangle we draw in.
        CascadeRect r = CascadeRect(
            SDesk::getInstance().getCurrentTheme().getDetailTextXOffset(),
            SDesk::getInstance().getCurrentTheme().getDetailTextYOffset(),
            SDesk::getInstance().getCurrentTheme().getDetailTextWidth(),
            SDesk::getInstance().getCurrentTheme().getDetailTextHeight());

        // The item is not selected.
        bitmap.DrawText(
            SDesk::getInstance().getCurrentTheme().getSmallFont(),
            r, mDetailText,
            SDesk::getInstance().getCurrentTheme().getNormalColor(),
            TEXT_JUSTIFY_TOP |
            TEXT_JUSTIFY_LEFT | TEXT_WORD_WRAP);
    }

    Log(7, "SDeskItem::DrawDetailText() done\n");
}

