/*****************************************************************************
 * callback.c : Callbacks for CD digital audio input module
 *****************************************************************************
 * Copyright (C) 2004 VideoLAN
 * $Id: callback.c 8606 2004-08-31 18:32:54Z rocky $
 *
 * Authors: Rocky Bernstein <rocky@panix.com>
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

#include "callback.h"
#include "cdda.h"

int
E_(CDDADebugCB)   ( vlc_object_t *p_this, const char *psz_name,
		    vlc_value_t oldval, vlc_value_t val, void *p_data )
{
  cdda_data_t *p_cdda;

  if (NULL == p_cdda_input) return VLC_EGENERIC;

  p_cdda = (cdda_data_t *)p_cdda_input->p_sys;

  if (p_cdda->i_debug & (INPUT_DBG_CALL|INPUT_DBG_EXT)) {
    msg_Dbg( p_cdda_input, "Old debug (x%0x) %d, new debug (x%0x) %d",
             p_cdda->i_debug, p_cdda->i_debug, val.i_int, val.i_int);
  }
  p_cdda->i_debug = val.i_int;
  return VLC_SUCCESS;
}

/* FIXME: could probably shorten some of the below boilerplate code...
*/
int
E_(CDDBEnabledCB)   ( vlc_object_t *p_this, const char *psz_name,
                      vlc_value_t oldval, vlc_value_t val, void *p_data )
{
  cdda_data_t *p_cdda;

  if (NULL == p_cdda_input) return VLC_EGENERIC;

  p_cdda = (cdda_data_t *)p_cdda_input->p_sys;

#ifdef HAVE_LIBCDDB
  if (p_cdda->i_debug & (INPUT_DBG_CALL|INPUT_DBG_EXT)) {
    msg_Dbg( p_cdda_input, "Old CDDB Enabled (x%0x) %d, new (x%0x) %d",
             p_cdda->b_cddb_enabled, p_cdda->b_cddb_enabled,
             val.b_bool, val.b_bool);
  }
  p_cdda->b_cddb_enabled = val.b_bool;
#endif
  return VLC_SUCCESS;
}

int
E_(CDTextEnabledCB)   ( vlc_object_t *p_this, const char *psz_name,
			vlc_value_t oldval, vlc_value_t val, void *p_data )
{
  cdda_data_t *p_cdda;

  if (NULL == p_cdda_input) return VLC_EGENERIC;

  p_cdda = (cdda_data_t *)p_cdda_input->p_sys;

  if (p_cdda->i_debug & (INPUT_DBG_CALL|INPUT_DBG_EXT)) {
    msg_Dbg( p_cdda_input, "Old CDText Enabled (x%0x) %d, new (x%0x) %d",
             p_cdda->b_cdtext_enabled, p_cdda->b_cdtext_enabled,
             val.b_bool, val.b_bool);
  }
  p_cdda->b_cdtext_enabled = val.b_bool;
  return VLC_SUCCESS;
}

int
E_(CDTextPreferCB)   ( vlc_object_t *p_this, const char *psz_name,
		       vlc_value_t oldval, vlc_value_t val, void *p_data )
{
  cdda_data_t *p_cdda;

  if (NULL == p_cdda_input) return VLC_EGENERIC;

  p_cdda = (cdda_data_t *)p_cdda_input->p_sys;

#ifdef HAVE_LIBCDDB
  if (p_cdda->i_debug & (INPUT_DBG_CALL|INPUT_DBG_EXT)) {
    msg_Dbg( p_cdda_input, "Old CDText Prefer (x%0x) %d, new (x%0x) %d",
             p_cdda->b_cdtext_prefer, p_cdda->b_cdtext_prefer,
             val.b_bool, val.b_bool);
  }
  p_cdda->b_cdtext_prefer = val.b_bool;
#endif
  return VLC_SUCCESS;
}

int
E_(CDDABlocksPerReadCB) ( vlc_object_t *p_this, const char *psz_name,
			  vlc_value_t oldval, vlc_value_t val, void *p_data )
{
  cdda_data_t *p_cdda;

  if (NULL == p_cdda_input) return VLC_EGENERIC;

  p_cdda = (cdda_data_t *)p_cdda_input->p_sys;

  if (p_cdda->i_debug & (INPUT_DBG_CALL|INPUT_DBG_EXT)) {
    msg_Dbg( p_cdda_input, "Old blocks per read: %d, new %d",
             p_cdda->i_blocks_per_read, val.i_int);
  }

  if (0 == val.i_int) val.i_int = DEFAULT_BLOCKS_PER_READ;
  if ( val.i_int >= MIN_BLOCKS_PER_READ && val.i_int <= MAX_BLOCKS_PER_READ )
    p_cdda->i_blocks_per_read = val.i_int;
  else {
    msg_Warn( p_cdda_input, 
	      "Number of blocks (%d) has to be between %d and %d. No change.", 
	      val.i_int, MIN_BLOCKS_PER_READ, MAX_BLOCKS_PER_READ );
  }
  
  return VLC_SUCCESS;
}