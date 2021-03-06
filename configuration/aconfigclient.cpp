/*
    Aeskulap Configuration - persistent configuration interface library
    Copyright (C) 2005  Alexander Pipelka

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

    Alexander Pipelka
    pipelka@teleweb.at

    Last Update:      $Author: braindead $
    Update Date:      $Date: 2006/03/16 13:50:53 $
    Source File:      $Source: /cvsroot/aeskulap/aeskulap/configuration/aconfigclient.cpp,v $
    CVS/RCS Revision: $Revision: 1.2 $
    Status:           $State: Exp $
*/

#include "aconfigclient.h"

namespace Aeskulap {

ConfigClient::ConfigClient() :
m_configuration(Aeskulap::Configuration::get_instance()) {
}

} // namespace Aeskulap
