/*****************************************************************************
 * modules_builtin.h: built-in modules list
 *****************************************************************************
 * Copyright (C) 2001 VideoLAN
 *
 * Authors: Samuel Hocevar <sam@zoy.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111, USA.
 *****************************************************************************/

#define ALLOCATE_BUILTIN( NAME ) \
    AllocateBuiltinModule( p_this, vlc_entry__ ## NAME );

/* We also consider the main program as a module (useful for config stuff) */
int vlc_entry__main( module_t* );

/* Add stuff here */
int vlc_entry__dummy( module_t* );
int vlc_entry__rc( module_t* );
int vlc_entry__telnet( module_t* );
int vlc_entry__logger( module_t* );
int vlc_entry__gestures( module_t* );
int vlc_entry__memcpy( module_t* );
int vlc_entry__hotkeys( module_t* );
int vlc_entry__netsync( module_t* );
int vlc_entry__showintf( module_t* );
int vlc_entry__mpgv( module_t* );
int vlc_entry__mpga( module_t* );
int vlc_entry__m4v( module_t* );
int vlc_entry__m4a( module_t* );
int vlc_entry__h264( module_t* );
int vlc_entry__ps( module_t* );
int vlc_entry__pva( module_t* );
int vlc_entry__avi( module_t* );
int vlc_entry__asf( module_t* );
int vlc_entry__aac( module_t* );
int vlc_entry__mp4( module_t* );
int vlc_entry__rawdv( module_t* );
int vlc_entry__nsv( module_t* );
int vlc_entry__real( module_t* );
int vlc_entry__aiff( module_t* );
int vlc_entry__mjpeg( module_t* );
int vlc_entry__demuxdump( module_t* );
int vlc_entry__cvdsub( module_t* );
int vlc_entry__svcdsub( module_t* );
int vlc_entry__spudec( module_t* );
int vlc_entry__subsdec( module_t* );
int vlc_entry__dvbsub( module_t* );
int vlc_entry__mpeg_audio( module_t* );
int vlc_entry__lpcm( module_t* );
int vlc_entry__a52( module_t* );
int vlc_entry__dts( module_t* );
int vlc_entry__cinepak( module_t* );
int vlc_entry__deinterlace( module_t* );
int vlc_entry__invert( module_t* );
int vlc_entry__adjust( module_t* );
int vlc_entry__wall( module_t* );
int vlc_entry__transform( module_t* );
int vlc_entry__distort( module_t* );
int vlc_entry__clone( module_t* );
int vlc_entry__crop( module_t* );
int vlc_entry__motionblur( module_t* );
int vlc_entry__float32tos16( module_t* );
int vlc_entry__float32tos8( module_t* );
int vlc_entry__float32tou16( module_t* );
int vlc_entry__float32tou8( module_t* );
int vlc_entry__a52tospdif( module_t* );
int vlc_entry__dtstospdif( module_t* );
int vlc_entry__fixed32tofloat32( module_t* );
int vlc_entry__fixed32tos16( module_t* );
int vlc_entry__s16tofixed32( module_t* );
int vlc_entry__s16tofloat32( module_t* );
int vlc_entry__s16tofloat32swab( module_t* );
int vlc_entry__s8tofloat32( module_t* );
int vlc_entry__u8tofixed32( module_t* );
int vlc_entry__u8tofloat32( module_t* );
int vlc_entry__audio_format( module_t* );
int vlc_entry__trivial_resampler( module_t* );
int vlc_entry__ugly_resampler( module_t* );
int vlc_entry__linear_resampler( module_t* );
int vlc_entry__bandlimited_resampler( module_t* );
int vlc_entry__trivial_channel_mixer( module_t* );
int vlc_entry__simple_channel_mixer( module_t* );
int vlc_entry__headphone_channel_mixer( module_t* );
int vlc_entry__normvol( module_t* );
int vlc_entry__trivial_mixer( module_t* );
int vlc_entry__spdif_mixer( module_t* );
int vlc_entry__float32_mixer( module_t* );
int vlc_entry__aout_file( module_t* );
int vlc_entry__equalizer( module_t* );
int vlc_entry__i420_rgb( module_t* );
int vlc_entry__i420_yuy2( module_t* );
int vlc_entry__i422_yuy2( module_t* );
int vlc_entry__i420_ymga( module_t* );
int vlc_entry__m3u( module_t* );
int vlc_entry__id3( module_t* );
int vlc_entry__playlist( module_t* );
int vlc_entry__export( module_t* );
int vlc_entry__sgimb( module_t* );
int vlc_entry__rawvideo( module_t* );
int vlc_entry__blend( module_t* );
int vlc_entry__scale( module_t* );
int vlc_entry__time( module_t* );
int vlc_entry__marq( module_t* );
int vlc_entry__wav( module_t* );
int vlc_entry__araw( module_t* );
int vlc_entry__subtitle( module_t* );
int vlc_entry__vobsub( module_t* );
int vlc_entry__adpcm( module_t* );
int vlc_entry__a52sys( module_t* );
int vlc_entry__dtssys( module_t* );
int vlc_entry__au( module_t* );
int vlc_entry__access_file( module_t* );
int vlc_entry__access_udp( module_t* );
int vlc_entry__access_tcp( module_t* );
int vlc_entry__access_http( module_t* );
int vlc_entry__ipv4( module_t* );
int vlc_entry__access_mms( module_t* );
int vlc_entry__access_ftp( module_t* );
int vlc_entry__access_directory( module_t* );
int vlc_entry__sap( module_t* );
int vlc_entry__http( module_t* );
int vlc_entry__screensaver( module_t* );
int vlc_entry__ipv6( module_t* );
int vlc_entry__mpgatofixed32( module_t* );
int vlc_entry__libmpeg2( module_t* );
int vlc_entry__hd1000a( module_t* );

#define ALLOCATE_ALL_BUILTINS() \
    do \
    { \
        ALLOCATE_BUILTIN(dummy); \
        ALLOCATE_BUILTIN(rc); \
        ALLOCATE_BUILTIN(telnet); \
        ALLOCATE_BUILTIN(logger); \
        ALLOCATE_BUILTIN(gestures); \
        ALLOCATE_BUILTIN(memcpy); \
        ALLOCATE_BUILTIN(hotkeys); \
        ALLOCATE_BUILTIN(netsync); \
        ALLOCATE_BUILTIN(showintf); \
        ALLOCATE_BUILTIN(mpgv); \
        ALLOCATE_BUILTIN(mpga); \
        ALLOCATE_BUILTIN(m4v); \
        ALLOCATE_BUILTIN(m4a); \
        ALLOCATE_BUILTIN(h264); \
        ALLOCATE_BUILTIN(ps); \
        ALLOCATE_BUILTIN(pva); \
        ALLOCATE_BUILTIN(avi); \
        ALLOCATE_BUILTIN(asf); \
        ALLOCATE_BUILTIN(aac); \
        ALLOCATE_BUILTIN(mp4); \
        ALLOCATE_BUILTIN(rawdv); \
        ALLOCATE_BUILTIN(nsv); \
        ALLOCATE_BUILTIN(real); \
        ALLOCATE_BUILTIN(aiff); \
        ALLOCATE_BUILTIN(mjpeg); \
        ALLOCATE_BUILTIN(demuxdump); \
        ALLOCATE_BUILTIN(cvdsub); \
        ALLOCATE_BUILTIN(svcdsub); \
        ALLOCATE_BUILTIN(spudec); \
        ALLOCATE_BUILTIN(subsdec); \
        ALLOCATE_BUILTIN(dvbsub); \
        ALLOCATE_BUILTIN(mpeg_audio); \
        ALLOCATE_BUILTIN(lpcm); \
        ALLOCATE_BUILTIN(a52); \
        ALLOCATE_BUILTIN(dts); \
        ALLOCATE_BUILTIN(cinepak); \
        ALLOCATE_BUILTIN(deinterlace); \
        ALLOCATE_BUILTIN(invert); \
        ALLOCATE_BUILTIN(adjust); \
        ALLOCATE_BUILTIN(wall); \
        ALLOCATE_BUILTIN(transform); \
        ALLOCATE_BUILTIN(distort); \
        ALLOCATE_BUILTIN(clone); \
        ALLOCATE_BUILTIN(crop); \
        ALLOCATE_BUILTIN(motionblur); \
        ALLOCATE_BUILTIN(float32tos16); \
        ALLOCATE_BUILTIN(float32tos8); \
        ALLOCATE_BUILTIN(float32tou16); \
        ALLOCATE_BUILTIN(float32tou8); \
        ALLOCATE_BUILTIN(a52tospdif); \
        ALLOCATE_BUILTIN(dtstospdif); \
        ALLOCATE_BUILTIN(fixed32tofloat32); \
        ALLOCATE_BUILTIN(fixed32tos16); \
        ALLOCATE_BUILTIN(s16tofixed32); \
        ALLOCATE_BUILTIN(s16tofloat32); \
        ALLOCATE_BUILTIN(s16tofloat32swab); \
        ALLOCATE_BUILTIN(s8tofloat32); \
        ALLOCATE_BUILTIN(u8tofixed32); \
        ALLOCATE_BUILTIN(u8tofloat32); \
        ALLOCATE_BUILTIN(audio_format); \
        ALLOCATE_BUILTIN(trivial_resampler); \
        ALLOCATE_BUILTIN(ugly_resampler); \
        ALLOCATE_BUILTIN(linear_resampler); \
        ALLOCATE_BUILTIN(bandlimited_resampler); \
        ALLOCATE_BUILTIN(trivial_channel_mixer); \
        ALLOCATE_BUILTIN(simple_channel_mixer); \
        ALLOCATE_BUILTIN(headphone_channel_mixer); \
        ALLOCATE_BUILTIN(normvol); \
        ALLOCATE_BUILTIN(trivial_mixer); \
        ALLOCATE_BUILTIN(spdif_mixer); \
        ALLOCATE_BUILTIN(float32_mixer); \
        ALLOCATE_BUILTIN(aout_file); \
        ALLOCATE_BUILTIN(equalizer); \
        ALLOCATE_BUILTIN(i420_rgb); \
        ALLOCATE_BUILTIN(i420_yuy2); \
        ALLOCATE_BUILTIN(i422_yuy2); \
        ALLOCATE_BUILTIN(i420_ymga); \
        ALLOCATE_BUILTIN(m3u); \
        ALLOCATE_BUILTIN(id3); \
        ALLOCATE_BUILTIN(playlist); \
        ALLOCATE_BUILTIN(export); \
        ALLOCATE_BUILTIN(sgimb); \
        ALLOCATE_BUILTIN(rawvideo); \
        ALLOCATE_BUILTIN(blend); \
        ALLOCATE_BUILTIN(scale); \
        ALLOCATE_BUILTIN(time); \
        ALLOCATE_BUILTIN(marq); \
        ALLOCATE_BUILTIN(wav); \
        ALLOCATE_BUILTIN(araw); \
        ALLOCATE_BUILTIN(subtitle); \
        ALLOCATE_BUILTIN(vobsub); \
        ALLOCATE_BUILTIN(adpcm); \
        ALLOCATE_BUILTIN(a52sys); \
        ALLOCATE_BUILTIN(dtssys); \
        ALLOCATE_BUILTIN(au); \
        ALLOCATE_BUILTIN(access_file); \
        ALLOCATE_BUILTIN(access_udp); \
        ALLOCATE_BUILTIN(access_tcp); \
        ALLOCATE_BUILTIN(access_http); \
        ALLOCATE_BUILTIN(ipv4); \
        ALLOCATE_BUILTIN(access_mms); \
        ALLOCATE_BUILTIN(access_ftp); \
        ALLOCATE_BUILTIN(access_directory); \
        ALLOCATE_BUILTIN(sap); \
        ALLOCATE_BUILTIN(http); \
        ALLOCATE_BUILTIN(screensaver); \
        ALLOCATE_BUILTIN(ipv6); \
        ALLOCATE_BUILTIN(mpgatofixed32); \
        ALLOCATE_BUILTIN(libmpeg2); \
        ALLOCATE_BUILTIN(hd1000a); \
    } while( 0 );

