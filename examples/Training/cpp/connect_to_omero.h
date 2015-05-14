/*
 * Copyright (C) Copyright 2015 Glencoe Software, Inc. All rights reserved.
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
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _Connect_to_OMERO_h
#define _Connect_to_OMERO_h

#include <iostream>
#include <omero/client.h>
#include <omero/api/IAdmin.h>

// Connect to the server.
void ConnectToOmero(int argc, char* argv[])
{    
    // Connection Data - host, port, username, password.
    Ice::InitializationData data;
    data.properties = Ice::createProperties();
    data.properties->setProperty("omero.host", argv[1]);
    data.properties->setProperty("omero.port", argv[2]);
    data.properties->setProperty("omero.user", argv[3]);
    data.properties->setProperty("omero.pass", argv[4]);
    // Connect to the server ...
    omero::client_ptr secure_client = new omero::client(data);
    secure_client->createSession();
    // ... and create unenrypted connection -> epecially useful when
    // downloading or uploading data.
    client = secure_client->createClient(false);
    session = client->getSession();
    session->closeOnDestroy();
    connected = true;
    // Print session details.
    admin = session->getAdminService();
    std::cout << "--------\n";
    std::cout << "User: " << admin->getEventContext()->userName << "\n"
              << "Id: "   << admin->getEventContext()->userId << "\n"
              << "Current group: " << admin->getEventContext()->groupName
              << "\n";
    std::cout << "---------\n";
}
#endif
