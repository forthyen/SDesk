#include <stdio.h>
#include <ctype.h>

#include <cascade/util/CascadeStream.h>
#include <cascade/graphics/CascadeRect.h>
#include <cascade/util/CascadeJpegFile.h>
#include <cascade/interprocess/CascadeSharedMemZone.h>

#include "SDesk.h"
#include "SDeskBitmap.h"

static CascadeSharedMemZone sSharedJPGMem;
static bool sSharedMemInitialized = false;

void SDeskBitmap::InitSharedMem(
    u32 width,
    u32 height)
{
    CloseSharedMem();

    if (!sSharedJPGMem.Open("SDeskJPG", width * height * 4, true)) {
        Log(1, "failed to create shared memory\n");
    }
    sSharedMemInitialized = true;
}

void SDeskBitmap::CloseSharedMem()
{
    if (sSharedMemInitialized) {
        sSharedJPGMem.Close();
        sSharedMemInitialized = false;
    }
}

SDeskBitmap::SDeskBitmap()
    : mLoaded(false),
      mIsJPG(false)
{
}

bool SDeskBitmap::Load(
    const char* fn,
    const char* dir)
{
    CascadeFile file;
    char fn2[400];
    unsigned int len = strlen(fn);

    if (dir && dir[0]) {
        strcpy(fn2, dir);
        strcat(fn2, "/");
        strcat(fn2, fn);
        fn = fn2;
    }

    
    if (((tolower(fn[len - 1]) == 'g' &&
          tolower(fn[len - 2]) == 'p' &&
          tolower(fn[len - 3]) == 'j') ||
         (tolower(fn[len - 1]) == 'm' &&
          tolower(fn[len - 2]) == 'h' &&
          tolower(fn[len - 3]) == 't')) &&
         fn[len - 4] == '.') {
        Log(7, "SDeskBitmap::ISJPG\n");
        mIsJPG = true;
    } else {
        if (((tolower(fn[len - 1]) == 'g' &&
              tolower(fn[len - 2]) == 'n' &&
              tolower(fn[len - 3]) == 'p' &&
              fn[len - 4] == '.') ||
             (tolower(fn[len - 1]) == 'm' &&
              tolower(fn[len - 2]) == 'h' &&
              tolower(fn[len - 3]) == 't' &&
              tolower(fn[len - 4]) == 'd' &&
              tolower(fn[len - 5]) == 's') &&
             fn[len - 6] == '.')) {
            mIsJPG = false;
        }
    }

    file.SetName(fn);
    if (!mIsJPG) {
        if (!CreateFromBitmapFile(file)) {
            return false;
        } else {
            Log(9, "loaded bitmap '%s'\n", fn);
            mLoaded = true;
            return true;
        }
    } else if (sSharedMemInitialized) {
        CascadeFileStream *file_stream;
        CascadeJpegFile jpg;
        CascadeRect clip(0, 0, 0, 0);
        const CascadeString name("SDesk Image");
        u32 jpg_size, bm_size;
        u8 *data;
        u32 *to, r, g, b;
        int l, z;
        u32 pixel, width, height, bpp;
        float aspect;

        file_stream = new CascadeFileStream();
        if (!file_stream->Open(file)) {
            Log(9, "failed to open jpg '%s'\n", fn);
            delete file_stream;
            return false;
        }

        // Initialize jpg object.
        if (!jpg.Init(file_stream, name, CascadeJpegFile::eRGB, false)) {
            Log(1, "failed to init jpg '%s'\n", fn);
            delete file_stream;
            return false;
        }
        jpg.GetImageInfo(width, height, bpp);

        // Maintain aspect ration...
        if (width > SDesk::getInstance().getCurrentTheme().getImageWidth()) {
            aspect = SDesk::getInstance().getCurrentTheme().getImageWidth() /
                (width  * 1.0);
            width = SDesk::getInstance().getCurrentTheme().getImageWidth();
            height = (u32) (height * aspect);
        }
        if (height > SDesk::getInstance().getCurrentTheme().getImageHeight()) {
            aspect = SDesk::getInstance().getCurrentTheme().getImageHeight() /
                (height * 1.0);
            height = SDesk::getInstance().getCurrentTheme().getImageHeight();
            width = (u32) (width * aspect);
        }

        jpg_size = jpg.GetBitmapDataSize(width, height);

        data = (u8 *) sSharedJPGMem.MapLock();
        if (data == NULL) {
            Log(1, "failed to acquire share memory lock '%s'\n", fn);
            delete file_stream;
            return false;
        }

        // Clear data to all white
        bm_size = width * height * 4;
        memset(data, 255, bm_size);

        // Create an empty bitmap
        Create(CascadeDims(width, height));
 
        // Get and decode JPEG data
        clip = CascadeRect(0, 0, width, height);
        if (!jpg.GetBitmapData(data, bm_size, clip, width, height, 0)) {
            Log(1, "failed to get bitmap data '%s'\n", fn);
        } else {
            // Convert from 24bpp to 32bpp
            z = ((width * height * 4) - 4);
            l = ((width * height * 3) - 3);
            while (l >= 0) {
                r = *(data + l);
                g = *(data + l + 1);
                b = *(data + l + 2);
                to = (u32 *) (data + z);
                pixel = (b << 24) | (r << 16) | (g << 8) | 0x000000ff;
                *to = pixel;
                z -= 4;
                l -= 3;
            }
        }

        // Unlock the shared memory zone and blit the data into a bitmap 
        sSharedJPGMem.Unlock();
        clip = CascadeRect(0, 0, width, height);
        Blit(CascadePoint(0, 0), sSharedJPGMem, 0, width, height, 32, clip);
        delete file_stream;
        file.Close();
        return true;
    }
    return false;
}


void SDeskBitmap::Draw(
    CascadeBitmap& bitmap,
    int x,
    int y,
    int w,
    int h)
{
    CascadeRect r = GetRect();
    CascadeRect clip(0, 0, 0, 0);

    if (w != -1) {
        r.w = w;
    }
    if (h != -1) {
        r.h = h;
    }
    bitmap.Blit(CascadePoint(x, y), *this, r);
    clip = CascadeRect(x, y, r.w, r.h);
}

