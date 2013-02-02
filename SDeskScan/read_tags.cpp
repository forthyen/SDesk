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

 // This Version V0.5.1


#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <cascade/util/CascadeFileStream.h>
#include <cascade/util/CascadeMemoryStream.h>
#include <cascade/graphics/CascadeBitmap.h> 
#include <cascade/util/CascadeJpegFile.h> 
#include "read_tags.h"

#define MAKEFOURCC(ch0, ch1, ch2, ch3) ( \
    ( (int32_t)(char)(ch0) << 24 ) | \
    ( (int32_t)(char)(ch1) << 16 ) | \
    ( (int32_t)(char)(ch2) << 8 ) | \
    ( (int32_t)(char)(ch3) ) )

/* splits it into ch0, ch1, ch2, ch3 - use for printf's */
#define SPLITFOURCC(code) \
    (char)((int32_t)code >> 24), \
    (char)((int32_t)code >> 16), \
    (char)((int32_t)code >> 8), \
    (char)code

#define _Swap32(v) do { \
                   v = (((v) & 0x000000FF) << 0x18) | \
                       (((v) & 0x0000FF00) << 0x08) | \
                       (((v) & 0x00FF0000) >> 0x08) | \
                       (((v) & 0xFF000000) >> 0x18); } while(0)

#define _Swap16(v) do { \
                   v = (((v) & 0x00FF) << 0x08) | \
                       (((v) & 0xFF00) >> 0x08); } while (0)


static const char* GenreList[] = {
 "Blues", "Classic Rock", "Country", "Dance", "Disco", "Funk",
 "Grunge", "Hip-Hop", "Jazz", "Metal", "New Age", "Oldies",
 "Other", "Pop", "R&B", "Rap", "Reggae", "Rock",
 "Techno", "Industrial", "Alternative", "Ska", "Death Metal", "Pranks",
 "Soundtrack", "Euro-Techno", "Ambient", "Trip-Hop", "Vocal", "Jazz+Funk",
 "Fusion", "Trance", "Classical", "Instrumental", "Acid", "House",
 "Game", "Sound Clip", "Gospel", "Noise", "AlternRock", "Bass",
 "Soul", "Punk", "Space", "Meditative", "Instrumental Pop", "Instrumental Rock",
 "Ethnic", "Gothic", "Darkwave", "Techno-Industrial", "Electronic", "Pop-Folk",
 "Eurodance", "Dream", "Southern Rock", "Comedy", "Cult", "Gangsta",
 "Top 40", "Christian Rap", "Pop/Funk", "Jungle", "Native American", "Cabaret",
 "New Wave", "Psychadelic", "Rave", "Showtunes", "Trailer", "Lo-Fi",
 "Tribal", "Acid Punk", "Acid Jazz", "Polka", "Retro", "Musical",
 "Rock & Roll", "Hard Rock", "Folk", "Folk/Rock", "National Folk", "Swing",
 "Fast-Fusion", "Bebob", "Latin", "Revival", "Celtic", "Bluegrass", "Avantgarde",
 "Gothic Rock", "Progressive Rock", "Psychedelic Rock", "Symphonic Rock", "Slow Rock", "Big Band",
 "Chorus", "Easy Listening", "Acoustic", "Humour", "Speech", "Chanson",
 "Opera", "Chamber Music", "Sonata", "Symphony", "Booty Bass", "Primus",
 "Porn Groove", "Satire", "Slow Jam", "Club", "Tango", "Samba",
 "Folklore", "Ballad", "Power Ballad", "Rhythmic Soul", "Freestyle", "Duet",
 "Punk Rock", "Drum Solo", "A capella", "Euro-House", "Dance Hall",
 "Goa", "Drum & Bass", "Club House", "Hardcore", "Terror",
 "Indie", "BritPop", "NegerPunk", "Polsk Punk", "Beat",
 "Christian Gangsta", "Heavy Metal", "Black Metal", "Crossover", "Contemporary C",
 "Christian Rock", "Merengue", "Salsa", "Thrash Metal", "Anime", "JPop",
 "SynthPop",
};


TagProcessor::TagProcessor()
{
	processImage = false;
	imageScaleHeight = 327;
	imageScaleWidth = 327;
}

TagProcessor::~TagProcessor()
{
}

void TagProcessor::CleanSweep()
{
	/*
	** Just make sure everything is properly initialised
	**
	** Note that we do not clear down the value of processImage - the value 
	** of this is set by the calling agent and is valid for the life of the
	** TagProcessor object
	*/

	hasValidTags = false;
	hasAPEv2 = false;
	hasID3v1 = false;
	hasVorbis = false;
	hasQT = false;
	hasImage = false;

	testFirstForID3 = false;
	testFirstForAPE = false;
	testFirstForOgg = false;
	testFirstForFLAC = false;
	testFirstForQT = false;

	GenreNo=0;
	TrackNo=0;
	Genre[0]='\0';
	Year[0]='\0';
	Track[0]='\0';
	Title[0]='\0';
	Artist[0]='\0';
	Album[0]='\0';
	Comment[0]='\0';

	VorbisFilePos = 0;
	stream_size = 0;

	m_nDurationInMilliseconds = 0;

	imageType = 0;

}

/*
** GetProcessImageEnabled()
** Returns the current value of processImage. A value of
** true means embedded art is being processed rather than skipped
*/

bool TagProcessor::GetProcessImageEnabled()
{
	return processImage;
}

/*
** SetProcessImage()
** Sets whether embedded art (if it exists) should be processed or skipped
*/
void TagProcessor::SetProcessImage(bool active)
{
	processImage = active;
}

/*
** GetImageType()
** Returns 0 if unknown
** 1 if PNG
** 2 if JPG
*/

u32 TagProcessor::GetImageType()
{
    return imageType;
}

/*
** GetHasImage()
** Returns whether the file has embedded art
*/

bool TagProcessor::GetHasImage()
{
	if(!hasValidTags)
	{
		return(false);
	}
	return(hasImage);
}

bool TagProcessor::GetArtwork(CascadeBitmap * bm)
{
	if(!hasValidTags)
	{
		return(false);
	}

	if(!hasImage)
	{
		return(false);
	}

    /* The only type of tagging that supports embedded art that this code
    ** currently supports are QT tags.
    ** Therefore we will re-call CheckForQTTags with the appropriate parameters
    ** We 'save' the current value of processImage, and we set hasValidTags to false
    ** otherwise it will no parse the file correctly (and it will reset to true when
    ** finished anyway)
    */
    bool currentProcessImageSettings = processImage;
    hasValidTags = false;
    processImage = true;

    CheckForQTTags();

    processImage = currentProcessImageSettings;

    if(imageType==0)
    {
	/* this should not happen, but to be complete */

	return (false);
    }

    if(imageType==1)
    {
	/* Image type is PNG */
	/* /tmp/albumart.png */
	CascadeFile art;
	bool ok = false;

	bool fileAllocate = art.SetName("/tmp/albumart.png");
	if(fileAllocate)
	{
		ok = bm->CreateFromBitmapFile(art, CascadeBitmap::kEither);
		art.Delete();
	}

	return ok;

    }

    if(imageType==2)
    {
	/* Image type is JPG */

        CascadeFile art;
        bool fileAllocate = art.SetName("/tmp/albumart.jpg");

	if(!fileAllocate)
	{
		return false;
	}
	CascadeFileStream tempFS;
	bool ok = false;
	if (!tempFS.Open(art)) 
	{
		ok = false;
	}
	else
	{
		ok = LoadJPGIntoBitmap (&tempFS, bm);	
	}


	art.Delete();
	return ok;

    }

    return false;   /* how did you get here? */
}

bool TagProcessor::LoadJPGIntoBitmap (CascadeStream *jpeg, CascadeBitmap *bm)
{
	CascadeSharedMemZone art_sm;
	CascadeJpegFile jpg;
	CascadeRect clip(0,0,0,0);
	CascadeString name("CoverArt");
	u32 jp_size, bm_size;
	u32 pixel;
	u8 *data;
	u32 *to, r, g, b;
	int l, z;

	/* Open a shared memory zone to decode the image into */
	bm_size = imageScaleWidth * imageScaleHeight * 4;
	bm->Create(CascadeDims(imageScaleWidth, imageScaleHeight),
			CascadeBitmap::kEither);
	if (!art_sm.Open("album_art", bm_size, true)) {
		return false;
	}

	/* Start reading JPG file */
	if (!jpg.Init(jpeg, name, CascadeJpegFile::eRGB, false)) {
		return false;
	}
	jp_size = jpg.GetBitmapDataSize(imageScaleWidth, imageScaleHeight);

	data = (u8 *)art_sm.MapLock();
	if (data == NULL) {
		return false;
	}

	/* Clear data to all white */
	memset(data, 255, bm_size);

	/* Get and decode JPEG data */
	clip = CascadeRect(0, 0, imageScaleWidth, imageScaleHeight);
	if (!jpg.GetBitmapData(data, bm_size, clip, imageScaleWidth, imageScaleHeight, 0)) {
	} else {
		/* Convert from 24bpp to 32bpp */
		z = ((imageScaleWidth * imageScaleHeight * 4) - 4);
		l = ((imageScaleWidth * imageScaleHeight * 3) - 3);
		while ( l >= 0) {
			r = *(data + l);
			g = *(data + l + 1);
			b = *(data + l + 2);
			to   = (u32 *)(data + z);
			pixel = (b << 24) | (r << 16) | (g << 8) | 0x000000ff;
			*to = pixel;
			z -= 4;
			l -= 3;
		}
	}

	art_sm.Unlock();
	clip = CascadeRect(0, 0, imageScaleWidth, imageScaleHeight);
	bm->Blit(CascadePoint(0, 0), art_sm, 0, imageScaleWidth, imageScaleHeight, 32, clip);
	return true;
}

bool TagProcessor::SetImageScaleDimensions(CascadeDims dims)
{
	u32 passed_width = dims.w;
	u32 passed_height = dims.h;

	if(passed_width>2000)
	{
		// just wanted some form of sanity checking
		return false;
	}

	if(passed_height>2000)
	{
		// more sanity checking
		return false;
	}

	imageScaleHeight = passed_height;
	imageScaleWidth = passed_width;
	return true;
}


u32 TagProcessor::GetDurationInMilliseconds()
{
	if (!hasValidTags)
	{
		return 0;
	}
	return (m_nDurationInMilliseconds);
}

char * TagProcessor::GetGenre()
{
	if(!hasValidTags)
	{
		return("");
	}
	return(Genre); 
}

char * TagProcessor::GetYear()
{
    if(!hasValidTags)
    {
	return("");
    }

    return(Year);
}

char * TagProcessor::GetTrack()
{
    if(!hasValidTags)
    {
        return("");
    }

    return(Track);
}

char * TagProcessor::GetTitle()
{
    if(!hasValidTags)
    {
        return("");
    }

    return(Title);
}

char * TagProcessor::GetArtist()
{
    if(!hasValidTags)
    {
        return("");
    }

    return(Artist);
}

char * TagProcessor::GetAlbum()
{
    if(!hasValidTags)
    {
        return("");
    }

    return(Album);
}

char * TagProcessor::GetComment()
{
    if(!hasValidTags)
    {
        return("");
    }

    return(Comment);
}


bool TagProcessor::MatchTagIgnoreCase(const u8 * pTag1, const char * pTag2, u8 len) 
{
    if (strncasecmp((char *)pTag1, (char *)pTag2, len) == 0) { 
        return true; 
    } else { 
        return false; 
    }
}

bool TagProcessor::MatchTag(const u8 * pTag1, const char * pTag2, u8 len) 
{
	bool char_match = true;
	int i = 0;
	while(i<len)
	{
		if(*pTag1++!= *pTag2++)
		{
			char_match = false;
		}
		i++;
	}
	return char_match;
}


bool TagProcessor::SetInputStream(CascadeFileStream * pInputStream)
{
	/* 
	** This is the simple case where the file extension has not been provided
	*/

	return(SetInputStream(pInputStream, "noneX"));
}

bool TagProcessor::SetInputStream(CascadeFileStream * pInputStream, char * fileExtension)
{
	bool return_value = false;
	CleanSweep();

	// always discard the old input stream if present
	fp = NULL;

	if (NULL == pInputStream) return false;
	fp = pInputStream;
	stream_size = fp->GetSize();


	/*
	** We check if a file extension has been passed. If it has then we can use this
	** to determine which TAG check to call first. Of course this may not produce the
	** result, some music files e.g. WavPack support multiple TAG types. But it
	** should often/mostly be the case that the first choice is the best one
	*/

	if(MatchTagIgnoreCase((u8 *)fileExtension, "flac", 4))	/* FLAC */
	{
		return_value = CheckForFLACTags();
		testFirstForFLAC=true;
	}
	else if(MatchTagIgnoreCase((u8 *)fileExtension, "ogg", 3))        /* Ogg Vorbis */
	{
		return_value = CheckForOggTags();
		testFirstForOgg=true;
	}
	else if(MatchTagIgnoreCase((u8 *)fileExtension, "mpc", 3) || MatchTagIgnoreCase((u8 *)fileExtension, "wv", 2)) 
	{
		/* Either Musepack/MPC or WavPack */
		testFirstForAPE = true;
		return_value = CheckForAPETags();
	}
	else if(MatchTagIgnoreCase((u8 *)fileExtension, "m4a", 3))
	{
		return_value = CheckForQTTags();
		testFirstForQT = true;
	}
	else if(MatchTagIgnoreCase((u8 *)fileExtension, "tta", 3))
	{
		return_value = CheckForID3V1Tags();
		testFirstForID3 = true;
	}

	if(!return_value)
	{
		/* 
		** In here because either a (valid) file extension wasn't passed
		** or it was passed but there was no match based on the checked TAG
		** type. So we check the other types of TAGS (minus the one already
		** checked of course)
		*/
		return_value = DoInitialDecode();
	}

	if (return_value)
	{
		if(hasAPEv2)
		{
			/*
			** APEv2 tag found, so could be APE, MPC or WavPack file
			** If duration is zero, then we do WavPack duration check
			** If that fails, we do a MPC duration check
			*/
			if(m_nDurationInMilliseconds==0)
			{
				m_nDurationInMilliseconds = WavPackGetDuration();
			}
	
			/* Still zero? May be a MPC/Musepack file */
		
			if(m_nDurationInMilliseconds==0)
			{
				m_nDurationInMilliseconds = MPCGetDuration();
			}

			return(Read_APE_Tags());
		}
		if(hasVorbis)
		{
			return(Read_Vorbis_Tags());
		}
		if(hasQT)
		{
			return true;
		}
		if(hasID3v1)
		{
			/*
			** A file have have both ID3v1 tags and other tags, so
			** we make the check for ID3 tags our last check
			*/

			/*
			** ID3v1 tag found, so may possibly be a WavPack file
			** If duration is zero, then we do WavPack duration check
			*/

			if(m_nDurationInMilliseconds==0)
			{
				m_nDurationInMilliseconds = WavPackGetDuration();
			}


			/* Still zero? May be a MPC/Musepack file */

			if(m_nDurationInMilliseconds==0)
			{
				m_nDurationInMilliseconds = MPCGetDuration();
			}

			return(ReadID3Tag());
		}
	}	

	// initial decode failed, reset stream parameters
	fp = NULL;
	return false;
}

bool TagProcessor::DoInitialDecode()
{
	bool APEV2check = false;
	bool FLACcheck = false;
	bool Oggcheck = false;
	bool ID3V1check = false;
	bool QTcheck = false;
	
	if(!testFirstForAPE)
	{
	    APEV2check = CheckForAPETags();
	}

	if(APEV2check)
	{
		return true;
	}

	/*
	** FLAC files (should) use Vorbis Comments as their tagging
	** mechanism. 
	*/

	if(!testFirstForFLAC)
	{
	    FLACcheck = CheckForFLACTags();
	}

	if(FLACcheck)
	{
		return true;
	}

        /*
        ** Ogg Vorbis files (should) use Vorbis Comments as their tagging
        ** mechanism.
        */

        if(!testFirstForOgg)
        {
            Oggcheck = CheckForOggTags();
        }

        if(Oggcheck)
        {
                return true;
        }


	if(!testFirstForID3)
	{
	    ID3V1check = CheckForID3V1Tags();
	}

	if(ID3V1check)
	{
		return true;
	}

	if(!testFirstForQT)
	{
	    QTcheck = CheckForQTTags();
	}

	if(QTcheck)
	{
		return true;
	}

	return false;
}

bool TagProcessor::CheckForQTTags()
{
        demux_res_t demux_res;
        int myResult = 0;

	bool tempVal =fp->SeekToPosition(0);    //make sure at start of file

	if(!tempVal)
	{
		return false;
	}

        myResult = qtmovie_read(&demux_res);
        if(myResult==1)
        {
		return true;
        }
	return false;
}


bool TagProcessor::CheckForID3V1Tags()
{
	u8 buff[256];
        u64 desiredPos;
        bool tempVal;

        // does not have APEv2 tags, lets see if it has
        // ID3v1 tags

        // ID3 v1 should be at end of file. ID3v1 structure
        // is 128 bytes in length

        desiredPos = stream_size - 128;

        tempVal = fp->SeekToPosition(desiredPos);
	if(!tempVal)
	{
		return false;
	}

        if (3 != fp->ReadBytes(3, buff))
        {
		return false;
        }

        if(MatchTag(buff, "TAG", 3))
        {
                hasID3v1= true;
		hasValidTags = true;
		return true;
        }
	// Got this far? Then no valid tags found

	return false;
}

bool TagProcessor::CheckForAPETags()
{
        struct APETagFooterStruct T;
        u8 buff[256];

        /* The first thing we look for is if the file has a APE v2 tag
        ** This should be at the end of the file. If it is there then
        ** the last data at the end of the file should be the APE Tag footer
        ** which is 32 bytes long, with the first 8 bytes being "APETAGEX"
        **
	** A file may have both ID3v1 tags and APEv2 tags however, in this
	** case we need to skip over the ID3v1 tag also
	*/

	bool id3Present = CheckForID3V1Tags();

        u64 desiredPos = stream_size - (sizeof T);

	if(id3Present)
	{
		/*
		** As ID3v1 tags are present, either theres no APE tags
		** or we have to do an additional skip past them
		*/

		desiredPos = desiredPos - 128;
	}

        bool tempVal = fp->SeekToPosition(desiredPos);

        if(!tempVal)
        {
                return false;
        }

        if ((sizeof T) != fp->ReadBytes((sizeof T), buff))
        {
                return false;
        }

        memcpy((void *)&T,buff,(sizeof T));

        if(MatchTag(T.ID, "APETAGEX", (sizeof T.ID)))
        {
                hasAPEv2 = true;
                hasValidTags = true;
                return true;
        }
    return false;
}

bool TagProcessor::CheckForOggTags()
{
	/* 
	** Ogg Vorbis files are 'wrapped' in an Ogg container
	** We need to parse the Ogg container, looking for
	** a Vorbis comment header.
	** Vorbis has 3 types of header, identification (type 1)
	** comment (type 3) and setup (type 5)
	** They must appear in the order identification, comment, setup.
	*/

	/*
	** Ogg Header
	** Byte order: Little-endian 
	** Offset   Length   Contents
	** [ 0      4 bytes  "OggS"
	**   4      1 byte   Stream structure version (0x00)
	**   5      1 byte   Packet flag:
        **            bit 0:  true if page continued
        **            bit 1:  true if first page
        **            bit 2:  true if last page
        **            bit 3..7: reserved
	**   6      8 bytes  The end pcm sample position (64bit integer)
	**  14      4 bytes  Stream serial number
	**  18      4 bytes  Page number
	**  22      4 bytes  Check sum
	**  26      1 byte   Number of segments(s)
	**  27     (s)bytes  Segment table
	**  27+(s) (b)bytes  Body (b := header[27] + header[27+1] + ... + header[27+s-1])
	** ]*
	*/


        u8 buff[256];
        bool tempVal =fp->SeekToPosition(0);    //make sure at start of file

        if(!tempVal)
        {
                return(false);
        }

        if (4 != fp->ReadBytes(4, buff))
        {
                return(false);
        }
        if (! MatchTag(buff, "OggS", 4))
        {
                return false;
        }

	tempVal =fp->SeekToPosition(26);
        if(!tempVal)
        {
                return(false);
        }

	/* 
	** We read the vaule of the byte contained here
	** We are looking for it to have only one segment
	*/

        if (1 != fp->ReadBytes(1, buff))
        {
                return(false);
        }

	if(buff[0]!=1)
	{
		return false;
	}

	/*
	** The next value we read is the size of the segment
	** We are going to skip that amount of bytes
	*/

        if (1 != fp->ReadBytes(1, buff))
        {
                return(false);
        }

	u64 currPos = fp->GetPosition();

        tempVal =fp->SeekToPosition(currPos + buff[0]);
        if(!tempVal)
        {
                return(false);
        }

	/*
	** We now should be at the next Ogg section
	** This section should contain our Vorbis comments
	** (if there are any)
	*/

        if (4 != fp->ReadBytes(4, buff))
        {
                return(false);
        }
        if (! MatchTag(buff, "OggS", 4))
        {
                return false;
        }

	currPos = fp->GetPosition();

        tempVal =fp->SeekToPosition(currPos + 22);
        if(!tempVal)
        {
                return(false);
        }

	/*
	** Find number of segments
	*/

        if (1 != fp->ReadBytes(1, buff))
        {
                return(false);
        }

	/*
	** Skip the bytes representing those segments
	*/

        currPos = fp->GetPosition();

        tempVal =fp->SeekToPosition(currPos + buff[0]);
        if(!tempVal)
        {
                return(false);
        }

	/*
	** At this stage we (hopefully) are at the Vorbis
	** header
	** Need to check we are!
	** If we are then first byte we read will have value 3
	** (representing Vorbis comment header), then the string
	** vorbis
	*/

        if (1 != fp->ReadBytes(1, buff))
        {
                return(false);
        }

        if (6 != fp->ReadBytes(6, buff))
        {
                return(false);
        }
        if (! MatchTag(buff, "vorbis", 6))
        {
                return false;
        }

	/* If we have got this far, then we can go to our 'generic'
	** Vorbis comment reader. We just need to set our file position
	** to the current position
	*/

	VorbisFilePos = fp->GetPosition();
	hasVorbis = true;
	hasValidTags = true;
	return true;
}

bool TagProcessor::CheckForFLACTags()
{
	/*
        ** The basic structure of a FLAC stream is:
        ** - The four byte string "fLaC"
        ** - The STREAMINFO metadata block (mandatory)
        ** - Zero or more other metadata blocks
        ** - One or more audio frames
        */


        u8 buff[256]; 
	bool tempVal =fp->SeekToPosition(0);	//make sure at start of file

	if(!tempVal)
	{
		return(false);
	}


        if (4 != fp->ReadBytes(4, buff))
        {
		return(false);
        }

        if (! MatchTag(buff, "fLaC", 4))
        {
		return false;
        }

        // read the streaminfo metadata block header
        if (1 != fp->ReadBytes(1, buff))
        {
		return false;
        }

        if(buff[0]==128)
        {
                //no metadata other than streaminfo
		return false;
        }
        else if(buff[0]==0)
        {
		// fine to continue
        }
        else
        {
                // not a valid value, possibly corrupt file
		return false;
        }

        // Next three bytes give the length of the metablock

        if (3 != fp->ReadBytes(3, buff))
        {
		return false;
        }

        u32 blockSize = ReadBE24(&buff[0]);

        /*
        ** The block size for the STREAMINFO metablock is 34
        ** Anything else and we have an error
        */

        if(34!=blockSize)
        {
		return false;
        }

        if (34 != fp->ReadBytes(34, buff))
        {
		return false;
        }

	/* The STREAMINFO metablock contains number of channels
	** number of samples etc. We can determine the length
	** of the song using this information
	** The first 10 bytes give block and frame sizes, we can ignore
	** The next 20 bits give the sample rate in Hz
	*/
	
	u8 Hz[3];
	Hz[0] = buff[10];
	Hz[1] = buff[11];
	Hz[2] = buff[12] & 0xF0;

	/*
	** Now we need to right align (shift right by 4 bits)
	*/

	u32 sampleRate = ReadBE24(&Hz[0]);
	sampleRate = sampleRate >> 4;

	//u8 numC = buff[12] &0x0E;

	/* Total samples takes 36 bits in total */

	u8 sampleSize[6];

	sampleSize[0]=buff[13] & 0x0F;
	sampleSize[1]=buff[14];
	sampleSize[2]=buff[15];
	sampleSize[3]=buff[16];
	sampleSize[4]=buff[17];

	/* 36 bits, but already correctly right aligned */

	u64 sampleCount = ((u64)sampleSize[0] << 32) | ((u64)sampleSize[1] <<  24) | ((u64)sampleSize[2] << 16 ) | ((u64)sampleSize[3] << 8 ) | ((u64)sampleSize[4]);

	u64 nTime = sampleCount*1000/sampleRate;
	u32 nTime32 = nTime;

        bool commentFound = false;
        bool lastBlock = false;

        while(!commentFound && !lastBlock)
        {
                if (1 != fp->ReadBytes(1, buff))
                {
			return false;
                }

                if(127<=buff[0])
                {
                        lastBlock = true;
                }

                if(4==buff[0] || 132==buff[0])
                {
                        // found a vorbis comment block
                        commentFound = true;
                }

                // read the block size

                if (3 != fp->ReadBytes(3, buff))
                {
			return false;
                }

                blockSize = ReadBE24(&buff[0]);

                if(!commentFound)
                {
                        u64 currPos = fp->GetPosition();

                        tempVal =fp->SeekToPosition((u64)currPos + (u64)blockSize);
                }
		else
		{
			VorbisFilePos = fp->GetPosition();
			hasVorbis = true;
			hasValidTags = true;
			m_nDurationInMilliseconds = nTime32;
			return true;
		}
        }
	return false;
}

u32 TagProcessor::Read_LE_Uint32 ( const unsigned char* p )
{
    return ((u32)p[0] <<  0) |
           ((u32)p[1] <<  8) |
           ((u32)p[2] << 16) |
           ((u32)p[3] << 24);
}

u32 TagProcessor::ReadLE24(const u8 * pBuff) 
{ 
   return(((u32)pBuff[2] << 16) | ((u32)pBuff[1] <<  8) | ((u32)pBuff[0])); 
}

u32 TagProcessor::ReadLE32(const u8 * pBuff) 
{ 
	return (((u32)pBuff[3] << 24) | ((u32)pBuff[2] << 16) | ((u32)pBuff[1] <<  8) | ((u32)pBuff[0])); 
}

u32 TagProcessor::ReadBE24(const u8 * pBuff)
{
   return(((u32)pBuff[0] << 16) | ((u32)pBuff[1] <<  8) | ((u32)pBuff[2]));
}

#define TAG_ANALYZE(item,elem)                      \
    if ( 0 == memcmp (p, #item, sizeof #item ) ) {  \
        p += sizeof #item;                          \
        memcpy_crop ( elem, p, len );               \
        p += len;                                   \
    } else


bool TagProcessor::Read_APE_Tags () 
{
    u32                   len;
    u32                   flags;
    u8              buff [8192];
    u8*             p;
    u8*             end;
    struct APETagFooterStruct  T;
    u32                   TagLen;
    u32                   TagCount;
    u32                   tmp;

    u32 addtionalSkip = 0;

    GenreNo = -1;
    TrackNo = -1;

    if(hasID3v1)
    {
	/*
	** If in here then the file has APEv2 tags, but the file may
	** also have ID3v1 tags. If this is the case we need to make
	** an additional skip over the ID3v1 tag data
	*/
	
	addtionalSkip = 128;
    }

    u64 desiredPos = stream_size - (sizeof T) - addtionalSkip;

    bool tempVal = fp->SeekToPosition(desiredPos);

    if(!tempVal)
    {
	goto notag;
    }

    if ((sizeof T) != fp->ReadBytes((sizeof T), buff))
    {
	goto notag;
    }

    memcpy((void *)&T,buff,(sizeof T));

    if(!MatchTag(T.ID, "APETAGEX", (sizeof T.ID)))
    {
	goto notag;
    }

    tmp = Read_LE_Uint32 (T.Version);
    if (  tmp != 1000  &&  tmp != 2000 )
        goto notag;
    TagLen = Read_LE_Uint32 (T.Length);
    if ( TagLen <= sizeof T )
        goto notag;

    desiredPos = stream_size-TagLen-addtionalSkip;
    tempVal = fp->SeekToPosition(desiredPos);

    if(!tempVal)
    {
	goto notag;
    }

    memset ( buff, 0, sizeof(buff) );

    if (TagLen - (sizeof T) != fp->ReadBytes(TagLen - (sizeof T), buff))
    {
        goto notag;
    }

    TagCount = Read_LE_Uint32 (T.TagCount);
    end = buff + TagLen - sizeof T;
    for ( p = buff; p < end  &&  TagCount--; ) {
        len   = Read_LE_Uint32 ( p ); p += 4;
        flags = Read_LE_Uint32 ( p ); p += 4;
        TAG_ANALYZE ( Title  , Title   )
        TAG_ANALYZE ( Album  , Album   )
        TAG_ANALYZE ( Artist , Artist  )
        TAG_ANALYZE ( Album  , Album   )
        TAG_ANALYZE ( Comment, Comment )
        TAG_ANALYZE ( Track  , Track   )
        TAG_ANALYZE ( Year   , Year    )
        TAG_ANALYZE ( Genre  , Genre   )
        {
            p += strlen((const char *)p) + 1 + len;
        }
    }

    if ( Track[0] != '\0' )
        sprintf ( Track, "%02d", TrackNo = atoi (Track) );
    else
        strcpy ( Track, "    " );

    /* genre is not yet entirely decoded */
    return true;

notag:
    return false;
}


bool TagProcessor::Read_Vorbis_Tags()
{
	u8 buff[512];


	// We first need to go to the position in the file where
	// the Vorbis comments are. This should have been set when
	// identifying that Vorbis Comments were being used

	bool tempVal = fp->SeekToPosition(VorbisFilePos);

	if(!tempVal)
	{
		return false;
	}

	if (4 != fp->ReadBytes(4, buff))
	{
		// serious error
		return false;
	}

	u32 vendorLen = ReadLE32(&buff[0]);

	// Advance past data

	tempVal =fp->SeekToPosition(VorbisFilePos + 4 + (u64)vendorLen);

	if (!tempVal)
	{
		// serious error
		return false;
	}

	// Now read in length of user comment list

	if (4 != fp->ReadBytes(4, buff))
	{
		// serious error
		return false;
	}

	u32 commentsListLen = ReadLE32(&buff[0]);

	u64 currPos;
	u32 i = 0;
	u32 commentLength = 0;
	u32 extraPart;
	u32 chunkToRead=0;
	bool foundComment = false;

	while(i<commentsListLen)
	{
		if (4 != fp->ReadBytes(4, buff))
		{
			// serious error
			return false;
		}

		commentLength = ReadLE32(&buff[0]);

		extraPart=0;
		if(commentLength>512)
		{
			extraPart = commentLength-512;
			chunkToRead = 512;
		}
		else
		{
			chunkToRead = commentLength;
		}

		if (chunkToRead!=fp->ReadBytes(chunkToRead, buff))
		{
			// serious error
			return false;
		}

		if(extraPart>0)
		{	
			// meta data larger than we have space for, we skip

			currPos = fp->GetPosition();
			tempVal =fp->SeekToPosition((u64)currPos + (u64)extraPart);
			if(!tempVal)
			{
				return false;
			}
		}

		// Ok, now we try to work out what the data we have read actually means!

		if(MatchTagIgnoreCase(buff, "ALBUM=", 6))  
		{
			u32 transferSize = 0;
			if((chunkToRead-6)>ALBUM_LENGTH)
			{
				transferSize = ALBUM_LENGTH;
			}
			else
			{
				transferSize=chunkToRead-6;
			}
			memcpy_crop( Album  , buff + 6, transferSize );
			foundComment = true;
		}
                if(MatchTagIgnoreCase(buff, "GENRE=", 6))
                {
                        u32 transferSize = 0;
                        if((chunkToRead-6)>GENRE_LENGTH)
                        {
                                transferSize = GENRE_LENGTH;
                        }
                        else
                        {
                                transferSize=chunkToRead-6;
                        }
                        memcpy_crop( Genre  , buff + 6, transferSize );
                        foundComment = true;
                }
                if(MatchTagIgnoreCase(buff, "DATE=", 5))
                {
                        u32 transferSize = 0;
                        if((chunkToRead-5)>YEAR_LENGTH)
                        {
                                transferSize = YEAR_LENGTH;
                        }
                        else
                        {
                                transferSize=chunkToRead-5;
                        }
                        memcpy_crop( Year  , buff + 5, transferSize );
                        foundComment = true;
                }
                if(MatchTagIgnoreCase(buff, "TRACKNUMBER=", 12))
                {
                        u32 transferSize = 0;
                        if((chunkToRead-12)>TRACKNUM_LENGTH)
                        {
                                transferSize = TRACKNUM_LENGTH;
                        }
                        else
                        {
                                transferSize=chunkToRead-12;
                        }
                        memcpy_crop( Track  , buff + 12, transferSize );
                        foundComment = true;
                }
                if(MatchTagIgnoreCase(buff, "TITLE=", 6))
                {
                        u32 transferSize = 0;
                        if((chunkToRead-6)>TITLE_LENGTH)
                        {
                                transferSize = TITLE_LENGTH;
                        }
                        else
                        {
                                transferSize=chunkToRead-6;
                        }
                        memcpy_crop( Title  , buff + 6, transferSize );
                        foundComment = true;
                }
                if(MatchTagIgnoreCase(buff, "ARTIST=", 7))
                {
                        u32 transferSize = 0;
                        if((chunkToRead-7)>ARTIST_LENGTH)
                        {
                                transferSize = ARTIST_LENGTH;
                        }
                        else
                        {
                                transferSize=chunkToRead-7;
                        }
                        memcpy_crop( Artist  , buff + 7, transferSize );
                        foundComment = true;
                }
		/*
		** Two possible fields may map to Comment
		** DESCRIPTION or Comment
		*/

                if(MatchTagIgnoreCase(buff, "DESCRIPTION=", 13))
                {
                        u32 transferSize = 0;
                        if((chunkToRead-13)>COMMENT_LENGTH)
                        {
                                transferSize = COMMENT_LENGTH;
                        }
                        else
                        {
                                transferSize=chunkToRead-13;
                        }
                        memcpy_crop( Comment  , buff + 13, transferSize );
                        foundComment = true;
                }
                if(MatchTagIgnoreCase(buff, "Comment=", 8))
                {
                        u32 transferSize = 0;
                        if((chunkToRead-8)>COMMENT_LENGTH)
                        {
                                transferSize = COMMENT_LENGTH;
                        }
                        else
                        {
                                transferSize=chunkToRead-8;
                        }
                        memcpy_crop( Comment  , buff + 8, transferSize );
                        foundComment = true;
                }


		i++;
	}

	if(foundComment)
	{
		return true;
	}

	return false;
}


void TagProcessor::memcpy_crop ( char* dst, u8* src, size_t len )
{
    size_t  i;

    for ( i = 0; i < len; i++ )
        if  ( src[i] != '\0' )
            dst[i] = src[i];
        else
            break;

    // dst[i] points behind the string contents

    while ( i > 0  &&  dst [i-1] == ' ' )
        i--;
    dst [i] = '\0';
}

bool TagProcessor::ReadID3Tag()
{
    u8  tmp [128];
    GenreNo = -1;
    TrackNo = -1;


    u64 desiredPos = stream_size - 128;

    bool tempVal = fp->SeekToPosition(desiredPos);

    if(!tempVal)
    {
	return false;
    }

    if (128 != fp->ReadBytes(128, tmp))
    {
	return false;
    }

    if(!MatchTag(tmp, "TAG", 3))	// first 3 chars should be TAG
    {
	return false;
    }
    if ( !tmp[3]  &&  !tmp[33]  &&  !tmp[63]  &&  !tmp[93]  &&  !tmp[97] )
        return false;

    memcpy_crop  ( Title  , tmp +  3, 30 );
    memcpy_crop  ( Artist , tmp + 33, 30 );
    memcpy_crop  ( Album  , tmp + 63, 30 );
    memcpy_crop  ( Year   , tmp + 93,  4 );
    memcpy_crop  ( Comment, tmp + 97, 30 );

    strcpy ( Genre, tmp[127] < sizeof(GenreList)/sizeof(*GenreList)  ?
                         GenreList [GenreNo = tmp[127]]  :  "???" );

    // Index 0 may be true if file is very short
    if ( tmp[125] == 0  &&  (tmp[126] != 0  ||  stream_size < 66000 ) )
        sprintf ( Track, "%02d", TrackNo = tmp[126] );
    else
        strcpy ( Track, "    " );

    return true;
}

u32 TagProcessor::MPCGetDuration()
{
    u8 buff[256];

    /*
    ** Go to start of stream i.e. beginning of file
    */

    bool tempVal = fp->SeekToPosition(0);

    if(!tempVal)
    {
        return 0;
    }

    if (4 != fp->ReadBytes(4, buff))
    {
	return 0;
    }

    if (! MatchTag(buff, "MP+", 3))
    {
	return 0;
    }

    /*
    ** Looking good so far, lets determine the version of the file
    */

    u8 mpc_version = buff[3] & 0xF;

    /* 
    ** The header changed for version 7 (no version 8 yet)
    */

    u32 mpc_total_frames = 0;
    u32 mpc_samplerate = 0;
    u32 mpc_lastframe_num_samples = 0;

    if(mpc_version>=7)
    {
	u32 samplefreqs[4] = { 44100, 48000, 37800, 32000 };

	if(4 != fp->ReadBytes(4, &mpc_total_frames))
	{
	    return 0;
	}

	u32 tempDataHolder;
	if(4 != fp->ReadBytes(4, &tempDataHolder))
	{
	    return 0;
	}
	mpc_samplerate = samplefreqs[(tempDataHolder >> 16) & 0x0003];

	/* 
	** we skip the next 8 bytes, nothing we can make use of
	*/

	u64 currPos = fp->GetPosition();
	fp->SeekToPosition((u64)currPos + (u64)8);

        if(4 != fp->ReadBytes(4, &tempDataHolder))
        {
            return 0;
        }

	mpc_lastframe_num_samples = ( tempDataHolder >> 20) & 0x07FF;
	
    }
    else
    {
	mpc_samplerate = 44100;

        if(4 != fp->ReadBytes(4, &mpc_total_frames))
        {
            return 0;
        }

	/* the documentation for handling older version files is 
	** rubbish, so this is a best guess interpretation
	*/

	if(mpc_version<5)
	{
	    /* 16 bit not 32 bit */
	    mpc_total_frames = mpc_total_frames >> 16;
	}

	if(mpc_version<6)
	{
	    /* last frame is invalid in versions less than 6 */

	    mpc_total_frames = mpc_total_frames - 1;
	}
    }

    /*
    ** Calculate the song duration as follows
    ** (Number of frames -1) * samples per mpc frame + samples in last frame
    ** All multiplied by number of channels * 1000
    ** Divided by sample rate
    ** Number of channels is always 2
    */

    u64 numSamples = ((mpc_total_frames - 1) * MPC_FRAME_LENGTH + mpc_lastframe_num_samples) * 2;
    numSamples = numSamples * 1000; /* to get milliseconds */
    u64 nTime = (numSamples/2) / mpc_samplerate;
 
    return((u32)nTime);

}

u32 TagProcessor::WavPackGetDuration()
{
    u32 sample_rate;
    WavpackHeader wphdr;
    char buffer [sizeof (wphdr)], *sp = buffer + sizeof (wphdr);


    u32 total_samples = 0;

    /* 
    ** Go to start of stream i.e. beginning of file
    */

    bool tempVal = fp->SeekToPosition(0);

    if(!tempVal)
    {
        return 0;
    }

    if (fp->ReadBytes (sizeof (wphdr), buffer) != (u32) sizeof (wphdr))
    {
	return 0;
    }
    sp = buffer;

    if (*sp++ == 'w' && *sp == 'v' && *++sp == 'p' && *++sp == 'k' &&
    !(*++sp & 1) && sp [2] < 16 && !sp [3] && sp [5] == 4 && sp [4] >= 2 && sp [4] <= 0xf)
    {
        memcpy (&wphdr, buffer, sizeof (wphdr));
        little_endian_to_native (&wphdr, WavpackHeaderFormat);
    }
    else
    {
        return 0;
    }


    if (wphdr.block_samples && wphdr.total_samples != (u32) -1)
    {
            total_samples = wphdr.total_samples;
    }
    else
    {
	return 0;
    }

    if (wphdr.block_samples) 
    {
	sample_rate = (wphdr.flags & WAVPACK_SRATE_MASK) >> WAVPACK_SRATE_LSB;

	if (sample_rate == 15)
	{
	    // unknown sample rate
	    return 0;
	}
	else
	{
	    sample_rate = wavpack_sample_rates [sample_rate];
	    u64 nNumSamples = total_samples;
	    u32 timeofsong  = nNumSamples*1000/sample_rate;
	    return (timeofsong);
	}
    }

    return 0;
}

void TagProcessor::little_endian_to_native (void *data, char *format)
{
    u8 *cp = (u8 *) data;
    long temp;

    while (*format) {
        switch (*format) {
            case 'L':
               temp = cp [0] + ((long) cp [1] << 8) + ((long) cp [2] << 16) + ((long) cp [3] << 24);
                * (long *) cp = temp;
                cp += 4;
                break;

            case 'S':
                temp = cp [0] + (cp [1] << 8);
                * (short *) cp = (short) temp;
                cp += 2;
                break;

            default:
                if (isdigit (*format))
                    cp += *format - '0';

                break;
        }

        format++;
    }
}


/*
**
All atoms have a header of either 8 or 16 bytes, consisting of either two or three parts:

- atom size:a 4-byte, unsigned integer. If 0, the atom continues to the end of the file. 
- atom type: a 4-byte value, usually interpreted as an ASCII string like moov, though any value is valid. 
- Optionally, an extended size: if the atom size was 1, then this field is present and interpreted as an 8-byte unsigned integer. This allows an atom to contain more than 4 GB of data. 
*
*/

/* chunk handlers */
bool TagProcessor::read_chunk_ftyp(size_t chunk_len)
{
    fourcc_t type;
    u32 minor_ver;

    size_t size_remaining = chunk_len - 8; /* FIXME: can't hardcode 8, size may be 64bit */

    u32 v;
    fp->ReadBytes(4, &v);
    _Swap32(v);
    type = v;

    size_remaining-=4;
    if (type != MAKEFOURCC('M','4','A',' '))
    {
//        fprintf(stderr, "not M4A file\n");
        return false;
    }

    fp->ReadBytes(4, &v);
    _Swap32(v);
    minor_ver = v;

    size_remaining-=4;

    /* compatible brands */
    while (size_remaining)
    {
        /* unused */
	fp->ReadBytes(4, &v);
        size_remaining-=4;
    }
    return true;
}

void TagProcessor::read_chunk_tkhd(size_t chunk_len)
{
    /* don't need anything from here atm, skip */
    size_t size_remaining = chunk_len - 8; /* FIXME WRONG */

    u64 currPos = fp->GetPosition();

    fp->SeekToPosition((u64)currPos + (u64)size_remaining);
}

void TagProcessor::read_chunk_mdhd(size_t chunk_len)
{
    /* don't need anything from here atm, skip */
    size_t size_remaining = chunk_len - 8; /* FIXME WRONG */
    u64 currPos = fp->GetPosition();

    fp->SeekToPosition((u64)currPos + (u64)size_remaining);
}

/* media handler inside mdia */
void TagProcessor::read_chunk_hdlr(qtmovie_t *qtmovie, size_t chunk_len)
{
    fourcc_t comptype, compsubtype;
    size_t size_remaining = chunk_len - 8; /* FIXME WRONG */

    int strlen;
    char str[256] = {0};

    /* version */
    u8 v8;
    fp->ReadBytes(1, &v8);
    size_remaining -= 1;
    /* flags */
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);
    size_remaining -= 3;

    /* component type */
    u32 v;
    fp->ReadBytes(4, &v);
    _Swap32(v);
    comptype = v;

    fp->ReadBytes(4, &v);
    _Swap32(v);
    compsubtype = v;

    size_remaining -= 8;

    /* component manufacturer */
    fp->ReadBytes(4, &v);
    size_remaining -= 4;

    /* flags */
    fp->ReadBytes(4, &v);
    fp->ReadBytes(4, &v);
    size_remaining -= 8;

    /* name */
    fp->ReadBytes(1, &v8);
    strlen = v8;
    fp->ReadBytes(strlen, &str);
    size_remaining -= 1 + strlen;

    if (size_remaining)
    {
	u64 currPos = fp->GetPosition();
        fp->SeekToPosition((u64)currPos + (u64)size_remaining);
    }

}

void TagProcessor::read_chunk_stsd(qtmovie_t *qtmovie, size_t chunk_len)
{
    unsigned int i;
    u32 numentries;
    size_t size_remaining = chunk_len - 8; /* FIXME WRONG */

    /* version */
    u8 v8;
    fp->ReadBytes(1, &v8);
    size_remaining -= 1;
    /* flags */
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);
    size_remaining -= 3;

    u32 v;
    fp->ReadBytes(4, &v);
    _Swap32(v);
    numentries = v;

    size_remaining -= 4;

    if (numentries != 1)
    {
        fprintf(stderr, "only expecting one entry in sample description atom!\n");
        return;
    }
    
    for (i = 0; i < numentries; i++)
    {
        u32 entry_size;
        u16 version;

        u32 entry_remaining;

	fp->ReadBytes(4, &v);
	_Swap32(v);
	entry_size = v;

	fp->ReadBytes(4, &v);
	_Swap32(v);
	qtmovie->res->format = v;

        entry_remaining = entry_size;
        entry_remaining -= 8;

        /* sound info: */

	u64 currPos = fp->GetPosition();
	fp->SeekToPosition((u64)currPos + (u64)6); /* reserved */

        entry_remaining -= 6;

	u16 v16;
	fp->ReadBytes(2, &v16);
	_Swap16(v16);
	version = v16;

        if (version != 1)
            fprintf(stderr, "unknown version??\n");
        entry_remaining -= 2;

        /* revision level */
        fp->ReadBytes(2, &v16);
        /* vendor */
	fp->ReadBytes(4, &v);
        entry_remaining -= 6;

        /* EH?? spec doesn't say theres an extra 16 bits here.. but there is! */
        fp->ReadBytes(2, &v16);
        entry_remaining -= 2;

        fp->ReadBytes(2, &v16);
        _Swap16(v16);
        qtmovie->res->num_channels = v16;

        fp->ReadBytes(2, &v16);
        _Swap16(v16);
        qtmovie->res->sample_size = v16;

        entry_remaining -= 4;

        /* compression id */
        fp->ReadBytes(2, &v16);
        /* packet size */
        fp->ReadBytes(2, &v16);
        entry_remaining -= 4;

        /* sample rate - 32bit fixed point = 16bit?? */
        fp->ReadBytes(2, &v16);
        _Swap16(v16);
        qtmovie->res->sample_rate = v16;

        entry_remaining -= 2;

        /* remaining is codec data - we skip */

        currPos = fp->GetPosition();
        fp->SeekToPosition((u64)currPos + (u64)entry_remaining);

        entry_remaining -= entry_remaining;

        if (entry_remaining)
	{
	    currPos = fp->GetPosition();
	    fp->SeekToPosition((u64)currPos + (u64)entry_remaining);
	}

        if (qtmovie->res->format != MAKEFOURCC('a','l','a','c'))
        {
//            fprintf(stderr, "expecting 'alac' data format, got %c%c%c%c\n",
//                    SPLITFOURCC(qtmovie->res->format));
//            return;
        }
    }
}

void TagProcessor::read_chunk_stts(qtmovie_t *qtmovie, size_t chunk_len)
{
    u32 numentries;
    size_t size_remaining = chunk_len - 8; /* FIXME WRONG */

    /* version */
    u8 v8;
    fp->ReadBytes(1, &v8);
    size_remaining -= 1;
    /* flags */
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);
    size_remaining -= 3;

    u32 v;
    fp->ReadBytes(4, &v);
    _Swap32(v);
    numentries = v;

    size_remaining -= 4;

    qtmovie->res->num_time_to_samples = numentries;
    qtmovie->res->time_to_sample = (time_to_sample_struct *)malloc(numentries * sizeof(*qtmovie->res->time_to_sample));
    qtmovie->res->timesCalculated = true;

    u32 i;
    u32 tempSampleCount;
    u32 tempSampleDuration;

    for (i = 0; i < numentries; i++)
    {
	fp->ReadBytes(4, &v);
	_Swap32(v);
	tempSampleCount = v;

	fp->ReadBytes(4, &v);
	_Swap32(v);
	tempSampleDuration = v;

        qtmovie->res->time_to_sample[i].sample_count = tempSampleCount;
        qtmovie->res->time_to_sample[i].sample_duration = tempSampleDuration;
        size_remaining -= 8;
    }


    if (size_remaining)
    {
        fprintf(stderr, "ehm, size remaining?\n");
	u64 currPos = fp->GetPosition();
	fp->SeekToPosition((u64)currPos + (u64)size_remaining);
    }
}

void TagProcessor::read_chunk_stsz(qtmovie_t *qtmovie, size_t chunk_len)
{
    unsigned int i;
    u32 v;
    uint32_t numentries;
    size_t size_remaining = chunk_len - 8; /* FIXME WRONG */

    /* version */
    u8 v8;
    fp->ReadBytes(1, &v8);
    size_remaining -= 1;
    /* flags */
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);

    size_remaining -= 3;

    /* default sample size */
    fp->ReadBytes(4, &v);
    _Swap32(v);
    u32 sampleSizes = v;

    if (sampleSizes != 0)
    {
        fprintf(stderr, "i was expecting variable samples sizes\n");
	fp->ReadBytes(4, &v);
        size_remaining -= 4;
        return;
    }
    size_remaining -= 4;

    fp->ReadBytes(4, &v);
    _Swap32(v);
    numentries = v;

    size_remaining -= 4;

    qtmovie->res->num_sample_byte_sizes = numentries;
    qtmovie->res->sample_byte_size = (u32 *)malloc(numentries * sizeof(*qtmovie->res->sample_byte_size));
    qtmovie->res->sizesCalculated = true;

    u32 tempSampleByteSize;

    /*
    ** Previously we were going through a loop reading 4 bytes at
    ** a time, tens of thousands of times. Obviously this was too
    ** slow, so we read the data into a CascadeMemoryStream and then
    ** parse it - much (much) faster
    */

    u8 *tempHolder = new u8[(numentries*4)+1];
    fp->ReadBytes(numentries*4, tempHolder);

    CascadeMemoryStream *cms = new CascadeMemoryStream(tempHolder,numentries*4);

    for (i = 0; i < numentries; i++)
    {
	cms->ReadBytes(4, &v);
	_Swap32(v);
	tempSampleByteSize = v;

        qtmovie->res->sample_byte_size[i] = tempSampleByteSize;
        size_remaining -= 4;
    }
    delete cms;
    delete tempHolder;

    if (size_remaining)
    {
        u64 currPos = fp->GetPosition();
        fp->SeekToPosition((u64)currPos + (u64)size_remaining);
    }
}

void TagProcessor::read_chunk_stbl(qtmovie_t *qtmovie, size_t chunk_len)
{
    size_t size_remaining = chunk_len - 8; /* FIXME WRONG */
    u32 v;
    u64 currPos;

    while (size_remaining)
    {
        size_t sub_chunk_len;
        fourcc_t sub_chunk_id;

	fp->ReadBytes(4, &v);
	_Swap32(v);
	sub_chunk_len = v;

        if (sub_chunk_len <= 1 || sub_chunk_len > size_remaining)
        {
            fprintf(stderr, "strange size for chunk inside stbl\n");
            return;
        }

        fp->ReadBytes(4, &v);
        _Swap32(v);
        sub_chunk_id = v;

        switch (sub_chunk_id)
        {
        case MAKEFOURCC('s','t','s','d'):
            read_chunk_stsd(qtmovie, sub_chunk_len);
            break;
        case MAKEFOURCC('s','t','t','s'):
            read_chunk_stts(qtmovie, sub_chunk_len);
            break;
        case MAKEFOURCC('s','t','s','z'):
	    read_chunk_stsz(qtmovie, sub_chunk_len);
            break;
        case MAKEFOURCC('s','t','s','c'):
        case MAKEFOURCC('s','t','c','o'):
            /* skip these, no indexing for us! */
		currPos = fp->GetPosition();
		fp->SeekToPosition((u64)currPos + (u64)sub_chunk_len-8);
            break;
        default:
            fprintf(stderr, "(stbl) unknown chunk id: %c%c%c%c\n",
                    SPLITFOURCC(sub_chunk_id));
            return;
        }

        size_remaining -= sub_chunk_len;
    }
}

void TagProcessor::read_chunk_minf(qtmovie_t *qtmovie, size_t chunk_len)
{
    size_t dinf_size, stbl_size;
    size_t size_remaining = chunk_len - 8; /* FIXME WRONG */

  /**** SOUND HEADER CHUNK ****/
    u32 v;
    fp->ReadBytes(4, &v);
    _Swap32(v);

    if (v != 16)
    {
        fprintf(stderr, "unexpected size in media info\n");
        return;
    }
    fp->ReadBytes(4, &v);
    _Swap32(v);

    if (v != MAKEFOURCC('s','m','h','d'))
    {
        fprintf(stderr, "not a sound header! can't handle this.\n");
        return;
    }
    /* now skip the rest */
    u64 currPos = fp->GetPosition();
    fp->SeekToPosition((u64)currPos + (u64)16-8);
    size_remaining -= 16;
  /****/

  /**** DINF CHUNK ****/
    fp->ReadBytes(4, &v);
    _Swap32(v);
    dinf_size = v;
    
    fp->ReadBytes(4, &v);
    _Swap32(v);

    if (v != MAKEFOURCC('d','i','n','f'))
    {
        fprintf(stderr, "expected dinf, didn't get it.\n");
        return;
    }
    /* skip it */
    currPos = fp->GetPosition();
    fp->SeekToPosition((u64)currPos + (u64)dinf_size-8);

    size_remaining -= dinf_size;
  /****/


  /**** SAMPLE TABLE ****/
    fp->ReadBytes(4, &v);
    _Swap32(v);
    stbl_size = v;

    fp->ReadBytes(4, &v);
    _Swap32(v);

    if (v != MAKEFOURCC('s','t','b','l'))
    {
        fprintf(stderr, "expected stbl, didn't get it.\n");
        return;
    }
    read_chunk_stbl(qtmovie, stbl_size);
    size_remaining -= stbl_size;

    if (size_remaining)
    {
        fprintf(stderr, "oops\n");
	u64 currPos = fp->GetPosition();
	fp->SeekToPosition((u64)currPos + (u64)size_remaining);
    }
}

void TagProcessor::read_chunk_mdia(qtmovie_t *qtmovie, size_t chunk_len)
{
    size_t size_remaining = chunk_len - 8; /* FIXME WRONG */
    u32 v;

    while (size_remaining)
    {
        size_t sub_chunk_len;
        fourcc_t sub_chunk_id;

	fp->ReadBytes(4, &v);
	_Swap32(v);
	sub_chunk_len = v;

        if (sub_chunk_len <= 1 || sub_chunk_len > size_remaining)
        {
            fprintf(stderr, "strange size for chunk inside mdia\n");
            return;
        }

	fp->ReadBytes(4, &v);
	_Swap32(v);
	sub_chunk_id = v;


        switch (sub_chunk_id)
        {
        case MAKEFOURCC('m','d','h','d'):
            read_chunk_mdhd(sub_chunk_len);
            break;
        case MAKEFOURCC('h','d','l','r'):
            read_chunk_hdlr(qtmovie, sub_chunk_len);
            break;
        case MAKEFOURCC('m','i','n','f'):
            read_chunk_minf(qtmovie, sub_chunk_len);
            break;
        default:
            fprintf(stderr, "(mdia) unknown chunk id: %c%c%c%c\n",
                    SPLITFOURCC(sub_chunk_id));
            return;
        }

        size_remaining -= sub_chunk_len;
    }
}

/* 'trak' - a movie track - contains other atoms */
void TagProcessor::read_chunk_trak(qtmovie_t *qtmovie, size_t chunk_len)
{
    size_t size_remaining = chunk_len - 8; /* FIXME WRONG */
    u32 v;

    while (size_remaining)
    {
        size_t sub_chunk_len;
        fourcc_t sub_chunk_id;

	fp->ReadBytes(4, &v);
	_Swap32(v);
	sub_chunk_len = v;

        if (sub_chunk_len <= 1 || sub_chunk_len > size_remaining)
        {
            fprintf(stderr, "strange size for chunk inside trak\n");
            return;
        }

	fp->ReadBytes(4, &v);
	_Swap32(v);
	sub_chunk_id = v;

        switch (sub_chunk_id)
        {
        case MAKEFOURCC('t','k','h','d'):
            read_chunk_tkhd(sub_chunk_len);
            break;
        case MAKEFOURCC('m','d','i','a'):
            read_chunk_mdia(qtmovie, sub_chunk_len);
            break;
        default:
            fprintf(stderr, "(trak) unknown chunk id: %c%c%c%c\n",
                    SPLITFOURCC(sub_chunk_id));
            return;
        }

        size_remaining -= sub_chunk_len;
    }
}

/* 'mvhd' movie header atom */
void TagProcessor::read_chunk_mvhd(qtmovie_t *qtmovie, size_t chunk_len)
{
    /* don't need anything from here atm, skip */
    size_t size_remaining = chunk_len - 8; /* FIXME WRONG */
    u64 currPos = fp->GetPosition();
    fp->SeekToPosition((u64)currPos + (u64)size_remaining);
}

/*
** Tags work as follows, there is a moov atom. Inside the moov atom there
** should be a udta atom (user data). Inside the udta atom will be a meta
** atom (assuming theres tags of course). Inside this meta atom is our
** various pieces of tag information
** so moov->udta->meta
*/

void TagProcessor::skip_chunk_generic(qtmovie_t *qtmovie, size_t chunk_len)
{
    size_t size_remaining = chunk_len - 8; /* FIXME WRONG */
    u64 currPos = fp->GetPosition();
    fp->SeekToPosition((u64)currPos + (u64)size_remaining);
}

void TagProcessor::read_chunk_generic_content(size_t chunk_len)
{
    size_t size_remaining = chunk_len - 8; /* FIXME WRONG */

//    u8 v8;
//    while (size_remaining)
//    {
//    	fp->ReadBytes(1, &v8);
//	fprintf(stderr,"[%c]",(char)v8);
//        size_remaining -= 1;
//    }
//    fprintf(stderr,"\n");
    u64 currPos = fp->GetPosition();
    fp->SeekToPosition((u64)currPos + (u64)size_remaining);
}

/*
** COPYnam - @nam
** Contains the name of the track/song
** This should contain one atom, data
*/
void TagProcessor::read_chunk_COPYnam(size_t chunk_len)
{
    size_t size_remaining = chunk_len - 8; /* FIXME WRONG */
    u32 v;
    fourcc_t sub_chunk_id;


    u8 v8;
    fp->ReadBytes(1, &v8);         /* version */
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);
    size_remaining -= 4;

    fp->ReadBytes(4, &v);
    _Swap32(v);
    sub_chunk_id = v;

    if (sub_chunk_id != MAKEFOURCC('d','a','t','a'))
    {
	fprintf(stderr,"in @nam - should have read data - this is bad\n");
	u64 currPos = fp->GetPosition();
	fp->SeekToPosition((u64)currPos + (u64)size_remaining-4);
	return;
    }

    fp->ReadBytes(4, &v);
    fp->ReadBytes(4, &v);
    size_remaining -=12;

    u8 trackName[size_remaining+1];
    if(size_remaining>0)
    {
        u8 sizeToCopy=size_remaining;

        if(size_remaining>TITLE_LENGTH)
        {
                sizeToCopy = TITLE_LENGTH;
        }
        fp->ReadBytes(size_remaining, &trackName);
        memcpy_crop( Title, trackName, sizeToCopy );
    }

    hasQT=true;
    hasValidTags=true;

}

/*
** gnre
** Contains the genre of the track/song
** This data will be an octet based on the ID3v1 index + 1
** This should contain one atom, data
*/
void TagProcessor::read_chunk_gnre(size_t chunk_len)
{
    size_t size_remaining = chunk_len - 8; /* FIXME WRONG */
    u32 v;
    fourcc_t sub_chunk_id;
    GenreNo = -1;


    u8 v8;
    fp->ReadBytes(1, &v8);         /* version */
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);
    size_remaining -= 4;

    fp->ReadBytes(4, &v);
    _Swap32(v);
    sub_chunk_id = v;

    if (sub_chunk_id != MAKEFOURCC('d','a','t','a'))
    {
        fprintf(stderr,"in gnre - should have read data - this is bad\n");
        u64 currPos = fp->GetPosition();
	fp->SeekToPosition((u64)currPos + (u64)size_remaining-4);
	return;
    }

    fp->ReadBytes(4, &v);
    fp->ReadBytes(4, &v);
    size_remaining -=12;

    /*
    ** The actual data here is (should be) 2 octets in length
    */

    fp->ReadBytes(1, &v8);
    size_remaining -=1;

    fp->ReadBytes(1, &v8);
    if(v8 < sizeof(GenreList)/sizeof(*GenreList)+1 )
    {
	strcpy(Genre, GenreList[v8-1]);
    }

    size_remaining -=1;


    while (size_remaining)
    {
        fp->ReadBytes(1, &v8);
        size_remaining -= 1;
    }

    hasQT=true;
    hasValidTags=true;

}

/*
** COPYgen - @gen
** Alternative atom for getting Genre data
** This should contain one atom, data
*/
void TagProcessor::read_chunk_COPYgen(size_t chunk_len)
{
    size_t size_remaining = chunk_len - 8; /* FIXME WRONG */
    u32 v;
    fourcc_t sub_chunk_id;


    u8 v8;
    fp->ReadBytes(1, &v8);         /* version */
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);
    size_remaining -= 4;

    fp->ReadBytes(4, &v);
    _Swap32(v);
    sub_chunk_id = v;

    if (sub_chunk_id != MAKEFOURCC('d','a','t','a'))
    {
        fprintf(stderr,"in @gen - should have read data - this is bad\n");
        u64 currPos = fp->GetPosition();
        fp->SeekToPosition((u64)currPos + (u64)size_remaining-4);
        return;
    }

    fp->ReadBytes(4, &v);
    fp->ReadBytes(4, &v);

    size_remaining -=12;

    u8 genre[size_remaining+1];
    if(size_remaining>0)
    {
        u8 sizeToCopy=size_remaining;

        if(size_remaining>GENRE_LENGTH)
        {
                sizeToCopy = GENRE_LENGTH;
        }
        fp->ReadBytes(size_remaining, &genre);
        memcpy_crop( Genre, genre, sizeToCopy );
    }

    hasQT=true;
    hasValidTags=true;

}


/*
** COPYcmt - @cmt
** Contains comments about the song
** This should contain one atom, data
*/
void TagProcessor::read_chunk_COPYcmt(size_t chunk_len)
{
    size_t size_remaining = chunk_len - 8; /* FIXME WRONG */
    u32 v;
    fourcc_t sub_chunk_id;


    u8 v8;
    fp->ReadBytes(1, &v8);         /* version */
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);
    size_remaining -= 4;

    fp->ReadBytes(4, &v);
    _Swap32(v);
    sub_chunk_id = v;

    if (sub_chunk_id != MAKEFOURCC('d','a','t','a'))
    {
        fprintf(stderr,"in @cmt - should have read data - this is bad\n");
        u64 currPos = fp->GetPosition();
	fp->SeekToPosition((u64)currPos + (u64)size_remaining-4);
	return;
    }

    fp->ReadBytes(4, &v);
    fp->ReadBytes(4, &v);

    size_remaining -=12;

    u8 comment[size_remaining+1];
    if(size_remaining>0)
    {
        u8 sizeToCopy=size_remaining;

        if(size_remaining>COMMENT_LENGTH)
        {
                sizeToCopy = COMMENT_LENGTH;
        }
        fp->ReadBytes(size_remaining, &comment);
        memcpy_crop( Comment, comment, sizeToCopy );
    }


    hasQT=true;
    hasValidTags=true;

}

/*
** COPYart - @ART
** Contains the Artist name
** This should contain one atom, data
*/
void TagProcessor::read_chunk_COPYart(size_t chunk_len)
{
    size_t size_remaining = chunk_len - 8; /* FIXME WRONG */
    u32 v;
    fourcc_t sub_chunk_id;


    u8 v8;
    fp->ReadBytes(1, &v8);         /* version */
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);
    size_remaining -= 4;

    fp->ReadBytes(4, &v);
    _Swap32(v);
    sub_chunk_id = v;

    if (sub_chunk_id != MAKEFOURCC('d','a','t','a'))
    {
        fprintf(stderr,"in @ART - should have read data - this is bad\n");
        u64 currPos = fp->GetPosition();
	fp->SeekToPosition((u64)currPos + (u64)size_remaining-4);
	return;
    }

    fp->ReadBytes(4, &v);
    fp->ReadBytes(4, &v);
    size_remaining -=12;

    u8 artist[size_remaining+1];
    if(size_remaining>0)
    {
	u8 sizeToCopy=size_remaining;

	if(size_remaining>ARTIST_LENGTH)
	{
		sizeToCopy = ARTIST_LENGTH;
	}
        fp->ReadBytes(size_remaining, &artist);
        memcpy_crop( Artist, artist, sizeToCopy );
    }

    hasQT=true;
    hasValidTags=true;

}

/*
** COPYalb - @alb
** Contains the name of the Album
** This should contain one atom, data
*/
void TagProcessor::read_chunk_COPYalb(size_t chunk_len)
{
    size_t size_remaining = chunk_len - 8; /* FIXME WRONG */
    u32 v;
    fourcc_t sub_chunk_id;


    u8 v8;
    fp->ReadBytes(1, &v8);         /* version */
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);
    size_remaining -= 4;

    fp->ReadBytes(4, &v);
    _Swap32(v);
    sub_chunk_id = v;

    if (sub_chunk_id != MAKEFOURCC('d','a','t','a'))
    {
        fprintf(stderr,"in @alb - should have read data - this is bad\n");
        u64 currPos = fp->GetPosition();
	fp->SeekToPosition((u64)currPos + (u64)size_remaining-4);
	return;
    }

    fp->ReadBytes(4, &v);
    fp->ReadBytes(4, &v);
    size_remaining -=12;

    u8 album[size_remaining+1];
    if(size_remaining>0)
    {
        u8 sizeToCopy=size_remaining;

        if(size_remaining>ALBUM_LENGTH)
        {
                sizeToCopy = ALBUM_LENGTH;
        }
        fp->ReadBytes(size_remaining, &album);
        memcpy_crop( Album, album, sizeToCopy );
    }

    hasQT=true;
    hasValidTags=true;
}

/*
** trkn
** Contains the track number in the following way
** Tracknumber (8 byte string)
** 16 bit: empty
** 16 bit: tracknumber
** 16 bit: total tracks on album
** 16 bit: empty
*/
 
void TagProcessor::read_chunk_trkn(size_t chunk_len)
{
    size_t size_remaining = chunk_len - 8; /* FIXME WRONG */
    u32 v;
    u16 v16;
    fourcc_t sub_chunk_id;
    u16 tracknumber;

    u8 v8;
    fp->ReadBytes(1, &v8);         /* version */
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);
    size_remaining -= 4;

    fp->ReadBytes(4, &v);
    _Swap32(v);
    sub_chunk_id = v;

    if (sub_chunk_id != MAKEFOURCC('d','a','t','a'))
    {
        fprintf(stderr,"in trkn - should have read data - this is bad\n");
        u64 currPos = fp->GetPosition();
	fp->SeekToPosition((u64)currPos + (u64)size_remaining-4);
	return;
    }

    fp->ReadBytes(4, &v);
    fp->ReadBytes(4, &v);
    size_remaining -=12;

    /*
    ** The actual data here is (should be) 8 octets in length
    */

    /*
    ** 2 bytes of nothing much happening
    */
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);

    /*
    ** 2 bytes containing the actual track number
    */

    fp->ReadBytes(2, &v16);
    _Swap16(v16);
    tracknumber = v16;

    sprintf ( Track, "%02d", tracknumber );

    /*
    ** Two bytes containing total number tracks, we ignore
    */
    fp->ReadBytes(2, &v16);

    /*
    ** Another 2 bytes of nothing much happening
    */

    fp->ReadBytes(2, &v16);


    size_remaining -=8;


    while (size_remaining)
    {
        fp->ReadBytes(1, &v8);
        size_remaining -= 1;
    }
    hasQT=true;
    hasValidTags=true;

}

/*
** COPYday - @day
** Contains the year of the track. This is 4 bytes in length (string)
** This should contain one atom, data
*/
void TagProcessor::read_chunk_COPYday(size_t chunk_len)
{
    size_t size_remaining = chunk_len - 8; /* FIXME WRONG */
    u32 v;
    fourcc_t sub_chunk_id;


    u8 v8;
    fp->ReadBytes(1, &v8);         /* version */
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);
    size_remaining -= 4;

    fp->ReadBytes(4, &v);
    _Swap32(v);
    sub_chunk_id = v;

    if (sub_chunk_id != MAKEFOURCC('d','a','t','a'))
    {
        fprintf(stderr,"in @day - should have read data - this is bad\n");
        u64 currPos = fp->GetPosition();
	fp->SeekToPosition((u64)currPos + (u64)size_remaining-4);
	return;
    }

    fp->ReadBytes(4, &v);
    fp->ReadBytes(4, &v);
    size_remaining -=12;

    u8 year[size_remaining+1];
    if(size_remaining>0)
    {
        u8 sizeToCopy=size_remaining;

        if(size_remaining>YEAR_LENGTH)
        {
                sizeToCopy = YEAR_LENGTH;
        }

        fp->ReadBytes(size_remaining, &year);
	memcpy_crop( Year  , year, sizeToCopy );
    }
    hasQT=true;
    hasValidTags=true;

}

/*
** covr
** Contains the cover art
**
** This is a (fully working) placeholder piece of code at the moment
*/
void TagProcessor::read_chunk_covr(size_t chunk_len)
{
    size_t size_remaining = chunk_len - 8; /* FIXME WRONG */
    u32 v;
    fourcc_t sub_chunk_id;

    u8 v8;
    fp->ReadBytes(1, &v8);         /* version */
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);
    size_remaining -= 4;

    fp->ReadBytes(4, &v);
    _Swap32(v);
    sub_chunk_id = v;

    fprintf(stderr,"in covr checking for album art.\n");
    if (sub_chunk_id != MAKEFOURCC('d','a','t','a'))
    {
        fprintf(stderr,"in covr - should have read data - this is bad\n");
        u64 currPos = fp->GetPosition();
	fp->SeekToPosition((u64)currPos + (u64)size_remaining-4);
	return;
    }

    fp->ReadBytes(4, &v);
    fp->ReadBytes(4, &v);

    size_remaining -=12;
    /*
    ** iTunes apparently generally saves in PNG format, but lets check
    **
    ** File header for PNG starts with the following values:
    ** 137  80  78  71  13  10  26  10
    **
    ** In case its not a PNG, then it could be a JPG
    ** File header for JPG starts with the following HEX values
    ** FF D8
    **
    ** We'll check the first 4 bytes
    **
    */
    
    FILE * pOutputFile;    
    bool validFormat = false;
    u8 o1[5];

    fp->ReadBytes(4, &o1); 
    size_remaining -= 4;

    if((o1[0]==137) && (o1[1]==80) && (o1[2]==78) && (o1[3]==71))  /* PNG */
    {
	//fprintf(stderr,"embedded cover art is in PNG format\n");
	imageType = 1;

	if(processImage)
	{
	    pOutputFile = fopen("/tmp/albumart.png", "wb");
	}
	validFormat = true;
    }
    else if(o1[0]==255 && o1[1]==216)		/* JPG */
    {
	//fprintf(stderr,"embedded cover art is in JPG format\n");

	imageType = 2;

	if(processImage)
	{
            pOutputFile = fopen("/tmp/albumart.jpg", "wb");
	}
        validFormat = true;
    }

    if(validFormat)
    {
	hasImage = true;
    }

    if(validFormat && processImage)
    {
	/* Lets first write those 4 bytes we read */
	fwrite((void *)o1, 4, 1, pOutputFile);
        u8 artBuffer[8192];
        while (size_remaining>8192)
        {
              fp->ReadBytes(8192, &artBuffer);
              fwrite(artBuffer, 8192, 1, pOutputFile);
              size_remaining -= 8192;
        }
        fp->ReadBytes(size_remaining, &artBuffer);
        fwrite(artBuffer, size_remaining, 1, pOutputFile);

	fclose(pOutputFile);
    }
    else
    {
	/* Neither PNG nor JPG. We skip therefore */
        u64 currPos = fp->GetPosition();
	fp->SeekToPosition((u64)currPos + (u64)size_remaining);
    }

}


/*
** Some mappings to know
** ©nam - name of song/track
** ©alb - album name
** ©ART - artist name
** trkn - track number
** ©cmt - comment
** gnre - genre (ID3v1 index + 1)
** ©gen - genre (Free Form)
** covr - cover art
** ©day - date (year)
** ---- - free form
**
*/
void TagProcessor::read_chunk_ilst(size_t chunk_len)
{
    size_t size_remaining = chunk_len - 8; /* FIXME WRONG */
    u32 v;

    while (size_remaining)
    {
        size_t sub_chunk_len;
        fourcc_t sub_chunk_id;

        fp->ReadBytes(4, &v);
        _Swap32(v);
        sub_chunk_len = v;

        if (sub_chunk_len <= 1 || sub_chunk_len > size_remaining)
        {
            fprintf(stderr, "strange size for chunk inside ilst\n");
            fprintf(stderr, "have the value %d\n", sub_chunk_len);
            return;
        }

        fp->ReadBytes(4, &v);
        _Swap32(v);
        sub_chunk_id = v;

//        fprintf(stderr, "in ilst, parsing atoms - got data format, got %c%c%c%c\n",
 //                   SPLITFOURCC(sub_chunk_id));

        switch (sub_chunk_id)
        {
        case MAKEFOURCC('©','n','a','m'):
            read_chunk_COPYnam(sub_chunk_len);
            break;
        case MAKEFOURCC('©','c','m','t'):
            read_chunk_COPYcmt(sub_chunk_len);
            break;
        case MAKEFOURCC('©','A','R','T'):
            read_chunk_COPYart(sub_chunk_len);
            break;
        case MAKEFOURCC('©','d','a','y'):
            read_chunk_COPYday(sub_chunk_len);
            break;
        case MAKEFOURCC('©','a','l','b'):
            read_chunk_COPYalb(sub_chunk_len);
            break;
        case MAKEFOURCC('t','r','k','n'):
            read_chunk_trkn(sub_chunk_len);
            break;
	case MAKEFOURCC('g','n','r','e'):
            read_chunk_gnre(sub_chunk_len);
            break;
        case MAKEFOURCC('c','o','v','r'):
            read_chunk_covr(sub_chunk_len);
            break;
        case MAKEFOURCC('©','g','e','n'):
            read_chunk_COPYgen(sub_chunk_len);
            break;
        default:
            read_chunk_generic_content(sub_chunk_len);
            break;
        }

        size_remaining -= sub_chunk_len;
    }
}


/*
** If there is a meta atom, then it should the following atoms
** - hdlr (required) - one only
** - ilst (required) - one only
*/

void TagProcessor::read_chunk_meta(qtmovie_t *qtmovie, size_t chunk_len)
{
    size_t size_remaining = chunk_len - 8; /* FIXME WRONG */
    u32 v;

    /* I think that the meta atom starts with these four octets */

    u8 v8;
    fp->ReadBytes(1, &v8);		/* version */
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);
    fp->ReadBytes(1, &v8);
    size_remaining -= 4;


    while (size_remaining)
    {
        size_t sub_chunk_len;
        fourcc_t sub_chunk_id;

        fp->ReadBytes(4, &v);
        _Swap32(v);
        sub_chunk_len = v;

        if (sub_chunk_len <= 1 || sub_chunk_len > size_remaining)
        {
            fprintf(stderr, "strange size for chunk inside meta\n");
	    fprintf(stderr, "have the value %d\n", sub_chunk_len);
            return;
        }

        fp->ReadBytes(4, &v);
        _Swap32(v);
        sub_chunk_id = v;

//        fprintf(stderr, "in meta, parsing atoms - got data format, got %c%c%c%c\n",
 //                   SPLITFOURCC(sub_chunk_id));

        switch (sub_chunk_id)
        {
        case MAKEFOURCC('i','l','s','t'):
            read_chunk_ilst(sub_chunk_len);
            break;
        default:
            skip_chunk_generic(qtmovie, sub_chunk_len);
            break;
        }

        size_remaining -= sub_chunk_len;
    }
}


void TagProcessor::read_chunk_udta(qtmovie_t *qtmovie, size_t chunk_len)
{
    size_t size_remaining = chunk_len - 8; /* FIXME WRONG */
    u32 v;

    while (size_remaining)
    {
        size_t sub_chunk_len;
        fourcc_t sub_chunk_id;

        fp->ReadBytes(4, &v);
        _Swap32(v);
        sub_chunk_len = v;

        if (sub_chunk_len <= 1 || sub_chunk_len > size_remaining)
        {
            fprintf(stderr, "strange size for chunk inside udta\n");
            return;
        }

        fp->ReadBytes(4, &v);
        _Swap32(v);
        sub_chunk_id = v;

        switch (sub_chunk_id)
        {
        case MAKEFOURCC('m','e','t','a'):
            read_chunk_meta(qtmovie, sub_chunk_len);
            break;
        default:
	    skip_chunk_generic(qtmovie, sub_chunk_len);
            break;
        }

        size_remaining -= sub_chunk_len;
    }
}



/* 'moov' movie atom - contains other atoms */
void TagProcessor::read_chunk_moov(qtmovie_t *qtmovie, size_t chunk_len)
{
    size_t size_remaining = chunk_len - 8; /* FIXME WRONG */
    u32 v;

    while (size_remaining)
    {
        size_t sub_chunk_len;
        fourcc_t sub_chunk_id;

	fp->ReadBytes(4, &v);
	_Swap32(v);
	sub_chunk_len = v;

        if (sub_chunk_len <= 1 || sub_chunk_len > size_remaining)
        {
            fprintf(stderr, "strange size for chunk inside moov\n");
            return;
        }

	fp->ReadBytes(4, &v);
	_Swap32(v);
	sub_chunk_id = v;

        switch (sub_chunk_id)
        {
        case MAKEFOURCC('m','v','h','d'):
            read_chunk_mvhd(qtmovie, sub_chunk_len);
            break;
        case MAKEFOURCC('t','r','a','k'):
            read_chunk_trak(qtmovie, sub_chunk_len);
            break;
        case MAKEFOURCC('u','d','t','a'):
            read_chunk_udta(qtmovie, sub_chunk_len);
            break;
        default:
            fprintf(stderr, "(moov) unknown chunk id: %c%c%c%c\n",
                    SPLITFOURCC(sub_chunk_id));
            return;
        }

        size_remaining -= sub_chunk_len;
    }
}

int TagProcessor::get_sample_info(demux_res_t *demux_res, u32 samplenum,
                           u32 *sample_duration,
                           u32 *sample_byte_size)
{
    unsigned int duration_index_accum = 0;
    unsigned int duration_cur_index = 0;

    if (samplenum >= demux_res->num_sample_byte_sizes)
    {
        //fprintf(stderr, "sample %i does not exist\n", samplenum);
        return 0;
    }

    if (!demux_res->num_time_to_samples)
    {
        fprintf(stderr, "no time to samples\n");
        return 0;
    }
    while ((demux_res->time_to_sample[duration_cur_index].sample_count + duration_index_accum)
            <= samplenum)
    {
        duration_index_accum += demux_res->time_to_sample[duration_cur_index].sample_count;
        duration_cur_index++;
        if (duration_cur_index >= demux_res->num_time_to_samples)
        {
            //fprintf(stderr, "sample %i does not have a duration\n", samplenum);
            return 0;
        }
    }

    *sample_duration = demux_res->time_to_sample[duration_cur_index].sample_duration;
    *sample_byte_size = demux_res->sample_byte_size[samplenum];

    return 1;
}

void TagProcessor::CalculateQTDuration(qtmovie_t *qtmovie)
{
	u32 i;
	u64 nTime;
	u64 nNumSamples;
	u32 output_size = 0;
	u32 thissample_duration;
	u32 thissample_bytesize;

	for (i = 0; i < qtmovie->res->num_sample_byte_sizes; i++)
	{
		thissample_duration = 0;
		thissample_bytesize = 0;

		get_sample_info(qtmovie->res, i, &thissample_duration, &thissample_bytesize);

		output_size += thissample_duration * (qtmovie->res->sample_size / 8)
                      * qtmovie->res->num_channels;
	}

	if(qtmovie->res->sizesCalculated)
	{
		free(qtmovie->res->sample_byte_size);
	}

	if(qtmovie->res->timesCalculated)
	{
		free(qtmovie->res->time_to_sample);
	}

	nNumSamples = ((output_size * 8)/qtmovie->res->sample_size) / qtmovie->res->num_channels;
	nTime = nNumSamples*1000/qtmovie->res->sample_rate;

	m_nDurationInMilliseconds = (u32)nTime;
}


int TagProcessor::qtmovie_read(demux_res_t *demux_res)
{
	qtmovie_t *qtmovie;
	u32 v;
	u64 currPos;
	bool tempVal;
	int return_value;
	bool ftypRetVal;

	qtmovie = (qtmovie_t*)malloc(sizeof(qtmovie_t));

	qtmovie->res = demux_res;
	qtmovie->res->sizesCalculated = false;
	qtmovie->res->timesCalculated = false;

	/* read the chunks */
	while (1)
	{
		if(hasValidTags)
		{
		    /* 
		    ** If we have already found tags then there is no need to process any further
		    */
		    CalculateQTDuration(qtmovie);
		    free(qtmovie);

		    return(1);
		}

   		size_t chunk_len;
		fourcc_t chunk_id;
    
		return_value = fp->ReadBytes(4, &v);
		_Swap32(v);
		chunk_len = v;

		if (return_value==0)	/* end of stream */
		{
	   		if(qtmovie->res->sizesCalculated)
			{
				free(qtmovie->res->sample_byte_size);
	   		}

			if(qtmovie->res->timesCalculated)
			{
				free(qtmovie->res->time_to_sample);
	   		}

			free(qtmovie);
			return 0;
		}

		if (chunk_len == 1)
		{
			fprintf(stderr, "need 64bit support\n");
			if(qtmovie->res->sizesCalculated)
			{
				free(qtmovie->res->sample_byte_size);
			}

			if(qtmovie->res->timesCalculated)
			{
				free(qtmovie->res->time_to_sample);
			}

			free(qtmovie);
			return 0;
		}
		fp->ReadBytes(4, &v);
		_Swap32(v);
		chunk_id = v;

		switch (chunk_id)
		{
			case MAKEFOURCC('f','t','y','p'):
				ftypRetVal = read_chunk_ftyp(chunk_len);
	    			if(!ftypRetVal)
	    			{
					/*
					** If ftypRetVal is false, it means this file does not
					** have M4A in its ftyp atom. So its some sort of QT/MPEG4
					** file, but not one we're interested in
					*/
					if(qtmovie->res->sizesCalculated)
					{
						free(qtmovie->res->sample_byte_size);
					}

					if(qtmovie->res->timesCalculated)
					{
						free(qtmovie->res->time_to_sample);
					}

					free(qtmovie);
					return 0;
				}
				break;
			case MAKEFOURCC('m','o','o','v'):
				read_chunk_moov(qtmovie, chunk_len);
				break;
			/* once we hit mdat we stop reading and return.
			* this is on the assumption that there is no further interesting
			* stuff in the stream. 
			*/
			case MAKEFOURCC('m','d','a','t'):
				CalculateQTDuration(qtmovie);
				free(qtmovie);
				return 1;

			/*  these following atoms can be skipped !!!! */
			case MAKEFOURCC('f','r','e','e'):
				currPos = fp->GetPosition();
				tempVal= fp->SeekToPosition((u64)currPos + (u64)chunk_len-8);
				/* FIXME not necessarily 8 */
				break;
			default:
	    		/* If its not one of the above, its probably not a file that
	    		** we can process, so we exit
	    		*/

//				fprintf(stderr, "(top) unknown chunk id: %c%c%c%c\n",
//				SPLITFOURCC(chunk_id));


				if(qtmovie->res->sizesCalculated)
				{
					free(qtmovie->res->sample_byte_size);
				}

				if(qtmovie->res->timesCalculated)
				{
					free(qtmovie->res->time_to_sample);
				}

				free(qtmovie);
				return 0;
		}
	}
	return 0;
}
