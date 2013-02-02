/*****************************************************************************
 * roku.c: VLM interface plugin
 *****************************************************************************
 * Copyright (C) 2000, 2001 VideoLAN
 * $Id$
 *
 * Authors: Sven Delmas
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

/*****************************************************************************
 * Preamble
 *****************************************************************************/
#include <stdlib.h>                                      /* malloc(), free() */

#include <vlc/vlc.h>
#include <vlc/intf.h>

#include <vlc/input.h>

#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>

#ifdef HAVE_SYS_TIME_H
#    include <sys/time.h>
#endif

#ifdef HAVE_UNISTD_H
#   include <unistd.h>
#endif

#if defined( UNDER_CE )
#   include <winsock.h>
#elif defined( WIN32 )
#   include <winsock2.h>
#else
#   include <sys/socket.h>
#endif

#include "network.h"

#include "vlc_vlm.h"

#define WORMHOLE_URL "wormholw://localhost/com.vlc.wormhole"

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
static int  Open ( vlc_object_t * );
static void Close( vlc_object_t * );

vlc_module_begin();
    set_description( _("Roku remote control interface") );
    add_category_hint( "VLM", NULL, VLC_FALSE );
    set_capability( "interface", 0 );
    set_callbacks( Open , Close );
vlc_module_end();

/*****************************************************************************
 * Local prototypes.
 *****************************************************************************/
static void Run( intf_thread_t * );

struct intf_sys_t
{
   vlm_t           *mediatheque;
};

/*****************************************************************************
 * Open: initialize dummy interface
 *****************************************************************************/
static int Open( vlc_object_t *p_this )
{
    intf_thread_t *p_intf = (intf_thread_t*) p_this;
    vlm_t *mediatheque;
    FILE *fd;

    if( !(mediatheque = vlm_New( p_intf )) )
    {
        msg_Err( p_intf, "cannot start VLM" );
        return VLC_EGENERIC;
    }

    msg_Info( p_intf, "Using the VLM interface plugin..." );
    
    p_intf->p_sys = malloc( sizeof( intf_sys_t ) );
    p_intf->p_sys->mediatheque = mediatheque;
    p_intf->pf_run = Run;

    // Create the run file.
    if ((fd = fopen("/tmp/vlc.runs", "w")) != NULL) {
        fclose(fd);
    }
    unlink("/tmp/stop.vlc");
    return VLC_SUCCESS;
}

/*****************************************************************************
 * Close:
 *****************************************************************************/
static void Close( vlc_object_t *p_this )
{
    intf_thread_t *p_intf = (intf_thread_t*)p_this;
    intf_sys_t    *p_sys  = p_intf->p_sys;

    unlink("/tmp/vlc.runs");

    vlm_Delete( p_sys->mediatheque );

    free( p_sys );
}

/*****************************************************************************
 * Run: main loop
 *****************************************************************************/
static void Run( intf_thread_t *p_intf )
{
    struct stat filestat;

    msg_Info( p_intf, "Using the VLM interface plugin... starting" );
    while( !p_intf->b_die )
    {
        if (stat("/tmp/stop.vlc", &filestat) == 0) {
            // The only command we support is quit.
            unlink("/tmp/stop.vlc");
            p_intf->b_die = 1;
        } else {
            // Sleep for three seconds.
            sleep(1);
        }
    }
    msg_Info( p_intf, "Using the VLM interface plugin... done" );
}

