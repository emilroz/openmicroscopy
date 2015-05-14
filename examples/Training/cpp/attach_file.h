/*
 * Copyright (C) Copyright 2014 Glencoe Software, Inc. All rights reserved.
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

#ifndef _attach_file_h
#define _attach_file_h

#include <iomanip>
#include <fstream>

#include "connect_to_omero.h"
#include <omero/api/IContainer.h>
#include <omero/sys/ParametersI.h>
#include <omero/model/FileAnnotationI.h>
#include <omero/RTypesI.h>
#include <omero/model/ImageAnnotationLinkI.h>
#include <omero/api/IUpdate.h>
#include <omero/model/ImageI.h>
#include <omero/api/RawFileStore.h>
#include <omero/model/OriginalFileI.h>
#include <omero/model/OriginalFileAnnotationLinkI.h>

// Attach a file to an Image.
void AttachFile(int argc, char* argv[])
{
    // Connect to the server.
    if (!connected) ConnectToOmero(argc, argv);
    
    if (argc >= 7) {
        file_path = std::string(argv[6]);
    } else {
        std::cout << "Pass the file path as the last argument\n";
        exit(-2);
    }
    if (argc >= 8) {
        image_id = atoi(argv[7]);
    }
    // Check if image exists.
    omero::api::IContainerPrx container_service =
        session->getContainerService();
    omero::sys::LongList id_list;
    id_list.push_back(image_id);
    omero::api::ImageList image_list = container_service->getImages(
        "Image", id_list, new omero::sys::ParametersI()
    );
    if (image_list.size() == 0) {
        std::cout << "Image id: " << image_id << " not found\n";
        exit(-2);
    }
    // SHA1 the file.
    std::ifstream file(file_path.c_str(), std::ifstream::binary);
    file.seekg(0, file.end);
    int file_length = file.tellg();
    file.seekg(0, file.beg);
    char buffer[file_length];
    file.read(buffer, file_length);

    // Create Original File.
    omero::api::IUpdatePrx iUpdate = session->getUpdateService();
    omero::model::OriginalFilePtr original_file =
        new omero::model::OriginalFileI();
    original_file->setName(omero::rtypes::rstring(file_path));
    original_file->setPath(omero::rtypes::rstring(file_path));
    original_file->setSize(omero::rtypes::rlong(file_length));
    original_file->setMimetype(omero::rtypes::rstring(".txt"));
    original_file = omero::model::OriginalFilePtr::dynamicCast(
        iUpdate->saveAndReturnObject(original_file)
    );

    // Upload the file.
    omero::api::RawFileStorePrx file_store = session->createRawFileStore();
    file_store->setFileId(original_file->getId()->getValue());
    std::vector<Ice::Byte> bytes;
    bytes.resize(file_length);
    std::copy(
        reinterpret_cast<Ice::Byte *>(buffer),
        reinterpret_cast<Ice::Byte *>(buffer) + file_length, bytes.begin()
    );
    file_store->write(bytes, 0, file_length);
    original_file = file_store->save();
    file.close();
    file_store->close();

    // Create file annotation.
    omero::model::FileAnnotationPtr fa = new omero::model::FileAnnotationI();
    fa->setFile(original_file);
    fa->setDescription(omero::rtypes::rstring("test description"));
    fa->setNs(omero::rtypes::rstring("new_name_space"));
    fa = omero::model::FileAnnotationPtr::dynamicCast(
        iUpdate->saveAndReturnObject(fa)
    );

    // Link the file annotation.
    omero::model::ImagePtr image = new omero::model::ImageI(image_id, false);
    omero::model::ImageAnnotationLinkPtr image_link =
        new omero::model::ImageAnnotationLinkI();
    image_link->setChild(fa);
    image_link->setParent(image);
    image_link = omero::model::ImageAnnotationLinkPtr::dynamicCast(
        iUpdate->saveAndReturnObject(image_link)
    );
    std::cout << "File attached";
}

#endif
