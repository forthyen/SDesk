/*
 * ReadTags
 * Copyright (c) 2005 Peter McQuillan
 * All rights reserved.
 *
 * This is a simple Tag parser for audio files for the Roku
 * PhotoBridge.
 *
 * This parser currently handles 
 * APEV2 (as used by Monkeys Audio, MPC/Musepack and WavPack)
 * Vorbis (as used by FLAC files and Ogg Vorbis)
 * ID3V1 (as used by TTA, WavPack, MPC/Musepack and MP3) 
 * QT Tags (as used by Apple Lossless and AAC)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <cascade/graphics/CascadeBitmap.h>

#define  GENRE_LENGTH     128
#define  YEAR_LENGTH      20
#define  TRACKNUM_LENGTH  8
#define  ARTIST_LENGTH    256
#define  ALBUM_LENGTH     256
#define  COMMENT_LENGTH   512
#define  TITLE_LENGTH     256
#define WavpackHeaderFormat "4LS2LLLLL"
#define WAVPACK_SRATE_LSB       23
#define WAVPACK_SRATE_MASK      (0xfL << WAVPACK_SRATE_LSB)
#define MPC_FRAME_LENGTH  1152    // samples per mpc frame (36 * 32)


struct APETagFooterStruct {
    unsigned char ID[8];        // APETAGEX
    unsigned char Version[4];
    unsigned char Length[4];
    unsigned char TagCount[4];
    unsigned char Flags[4];
    unsigned char Reserved[8];
};

typedef u32 fourcc_t;

typedef struct
{
        u32 sample_count;
        u32 sample_duration;
} time_to_sample_struct;


typedef struct
{
    u16 num_channels;
    u16 sample_size;
    u32 sample_rate;
    fourcc_t format;
    void *buf;

    u32 *sample_byte_size;
    u32 num_sample_byte_sizes;
    bool sizesCalculated;

    time_to_sample_struct *time_to_sample;
    u32 num_time_to_samples;
    bool timesCalculated;

} demux_res_t;

typedef struct
{
    demux_res_t *res;
} qtmovie_t;

typedef struct {
    char ckID [4];
    u32 ckSize;
    short version;
    u8 track_no, index_no;
    u32 total_samples, block_index, block_samples, flags, crc;
} WavpackHeader;

const u32 wavpack_sample_rates [] = { 6000, 8000, 9600, 11025, 12000, 16000, 22050,
    24000, 32000, 44100, 48000, 64000, 88200, 96000, 192000 };


class TagProcessor
{
public:

    TagProcessor();
    virtual ~TagProcessor();

public:

bool SetInputStream(CascadeFileStream * pInputStream);
bool SetInputStream(CascadeFileStream * pInputStream, char * fileExtension); /* overload */
char * GetGenre();
char * GetYear();
char * GetTrack();
char * GetTitle();
char * GetArtist();
char * GetAlbum();
char * GetComment();
bool GetHasImage();
void SetProcessImage(bool active);
bool GetProcessImageEnabled();
bool GetArtwork(CascadeBitmap * bm);
bool SetImageScaleDimensions(CascadeDims dims);
u32 GetDurationInMilliseconds();
u32 GetImageType();


private:
u32 ReadLE24(const u8 * pBuff);
u32 ReadLE32(const u8 * pBuff);
u32 ReadBE24(const u8 * pBuff);
void little_endian_to_native (void *data, char *format);
void memcpy_crop ( char* dst, u8* src, size_t len );
u32 Read_LE_Uint32 ( const unsigned char* p );
bool LoadJPGIntoBitmap (CascadeStream *jpeg, CascadeBitmap *bm);
bool ReadID3Tag();
bool Read_APE_Tags();
bool Read_Vorbis_Tags();
bool DoInitialDecode();
bool MatchTag(const u8 * pTag1, const char * pTag2, u8 len);
bool MatchTagIgnoreCase(const u8 * pTag1, const char * pTag2, u8 len);
void CleanSweep();
bool CheckForAPETags();
bool CheckForID3V1Tags();
bool CheckForFLACTags();
bool CheckForQTTags();
bool CheckForOggTags();
u32 WavPackGetDuration();
u32 MPCGetDuration();
void CalculateQTDuration(qtmovie_t *qtmovie);
int get_sample_info(demux_res_t *demux_res, u32 samplenum, u32 *sample_duration, u32 *sample_byte_size);
int qtmovie_read(demux_res_t *demux_res);
bool read_chunk_ftyp(size_t chunk_len);
void read_chunk_tkhd(size_t chunk_len);
void read_chunk_mdhd(size_t chunk_len);
void read_chunk_hdlr(qtmovie_t *qtmovie, size_t chunk_len);
void read_chunk_stsd(qtmovie_t *qtmovie, size_t chunk_len);
void read_chunk_stts(qtmovie_t *qtmovie, size_t chunk_len);
void read_chunk_stbl(qtmovie_t *qtmovie, size_t chunk_len);
void read_chunk_minf(qtmovie_t *qtmovie, size_t chunk_len);
void read_chunk_mdia(qtmovie_t *qtmovie, size_t chunk_len);
void read_chunk_trak(qtmovie_t *qtmovie, size_t chunk_len);
void read_chunk_mvhd(qtmovie_t *qtmovie, size_t chunk_len);
void read_chunk_stsz(qtmovie_t *qtmovie, size_t chunk_len);
void read_chunk_generic_content(size_t chunk_len);
void read_chunk_COPYnam(size_t chunk_len);
void read_chunk_gnre(size_t chunk_len);
void read_chunk_COPYgen(size_t chunk_len);
void read_chunk_COPYcmt(size_t chunk_len);
void read_chunk_COPYart(size_t chunk_len);
void read_chunk_COPYalb(size_t chunk_len);
void read_chunk_trkn(size_t chunk_len);
void read_chunk_COPYday(size_t chunk_len);
void read_chunk_covr(size_t chunk_len);
void read_chunk_ilst(size_t chunk_len);
void read_chunk_meta(qtmovie_t *qtmovie, size_t chunk_len);
void read_chunk_udta(qtmovie_t *qtmovie, size_t chunk_len);
void read_chunk_moov(qtmovie_t *qtmovie, size_t chunk_len);
void skip_chunk_generic(qtmovie_t *qtmovie, size_t chunk_len);

private:
    int           GenreNo;
    int           TrackNo;
    char          Genre   [GENRE_LENGTH+1];
    char          Year    [YEAR_LENGTH+1];
    char          Track   [TRACKNUM_LENGTH+1];
    char          Title   [TITLE_LENGTH+1];
    char          Artist  [ARTIST_LENGTH+1];
    char          Album   [ALBUM_LENGTH+1];
    char          Comment [COMMENT_LENGTH+1];

    u32 m_nDurationInMilliseconds;

    bool hasAPEv2;
    bool hasID3v1;
    bool hasVorbis;
    bool hasQT;
    bool hasValidTags;
    bool hasImage;	/* used for embedded art */
    bool processImage;	/* used when we want to process embedded image rather than skipping */

    bool testFirstForID3;
    bool testFirstForAPE;
    bool testFirstForFLAC;
    bool testFirstForQT;
    bool testFirstForOgg;

    CascadeFileStream * fp;
    u64 stream_size;
    u64 VorbisFilePos;  

    u32 imageType; 	/* 0 = unknown, 1 = PNG, 2 = JPG */
    u32 imageScaleHeight;	/* used as height to scale to for artwork */
    u32 imageScaleWidth;	/* usee as width to scale to for artwork */

};


