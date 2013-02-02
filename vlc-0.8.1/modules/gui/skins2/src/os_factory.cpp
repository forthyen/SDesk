/*****************************************************************************
 * os_factory.cpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: os_factory.cpp 6961 2004-03-05 17:34:23Z sam $
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teuli�re <ipkiss@via.ecp.fr>
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

#include "os_factory.hpp"

#ifdef X11_SKINS
#include "../x11/x11_factory.hpp"
#elif defined WIN32_SKINS
#include "../win32/win32_factory.hpp"
#endif

OSFactory *OSFactory::instance( intf_thread_t *pIntf )
{
    if( ! pIntf->p_sys->p_osFactory )
    {
        OSFactory *pOsFactory;
#ifdef X11_SKINS
        pOsFactory = new X11Factory( pIntf );
#elif defined WIN32_SKINS
        pOsFactory = new Win32Factory( pIntf );
#else
#error "No OSFactory implementation !"
#endif

        if( pOsFactory->init() )
        {
            // Initialization succeeded
            pIntf->p_sys->p_osFactory = pOsFactory;
        }
        else
        {
            // Initialization failed
            delete pOsFactory;
        }
    }
    return pIntf->p_sys->p_osFactory;
}


void OSFactory::destroy( intf_thread_t *pIntf )
{
    if( pIntf->p_sys->p_osFactory )
    {
        delete pIntf->p_sys->p_osFactory;
        pIntf->p_sys->p_osFactory = NULL;
    }
}
