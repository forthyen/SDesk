/*****************************************************************************
 * evt_generic.hpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: evt_generic.hpp 6961 2004-03-05 17:34:23Z sam $
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

#ifndef EVT_GENERIC_HPP
#define EVT_GENERIC_HPP

#include "../src/skin_common.hpp"
#include <string>


/// Base class for OS events
class EvtGeneric: public SkinObject
{
    public:
        virtual ~EvtGeneric() {}

        /// Return the type of the event
        virtual const string getAsString() const = 0;

    protected:
        EvtGeneric( intf_thread_t *pIntf ): SkinObject( pIntf ) {}
};


#endif
