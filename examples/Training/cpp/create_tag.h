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

#ifndef _create_tag_h
#define _create_tag_h

#include "connect_to_omero.h"
#include <omero/api/IContainer.h>
#include <omero/sys/ParametersI.h>
#include <omero/api/IUpdate.h>
#include <omero/model/ImageI.h>
#include <omero/model/TagAnnotationI.h>
#include <omero/model/ImageAnnotationLinkI.h>
#include <omero/RTypesI.h>

// Create a new tag and link it to the image.
void CreateTag(int argc, char* argv[])
{
    // Connect to the server.
    if (!connected) ConnectToOmero(argc, argv);
    if (argc >= 7) {
        image_id = atoi(argv[6]);
    }
    if (argc >= 8) {
        tag_name = std::string(argv[7]);
    }
    if (argc >= 9) {
        tag_description = std::string(argv[8]);
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
    // Create new tag.
    omero::model::TagAnnotationIPtr tag = new omero::model::TagAnnotationI();
    tag->setTextValue(omero::rtypes::rstring(tag_name));
    tag->setDescription(omero::rtypes::rstring(tag_description));
    
    omero::model::ImageAnnotationLinkPtr link =
        new omero::model::ImageAnnotationLinkI();
    link->setChild(tag);
    link->setParent(new omero::model::ImageI(image_id, false));
    omero::model::IObjectPtr r =
        session->getUpdateService()->saveAndReturnObject(link);
    std::cout << tag_name << " created and attached\n";
}

#endif
