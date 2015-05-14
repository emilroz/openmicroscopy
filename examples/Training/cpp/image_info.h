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

#ifndef _image_info_h
#define _image_info_h

#include "connect_to_omero.h"
#include <omero/api/IContainer.h>
#include <omero/sys/ParametersI.h>
#include <omero/model/PixelsI.h>
#include <omero/model/ImageI.h>
#include <omero/model/LengthI.h>

void ImageInfo(int argc, char* argv[])
{
    // Connect the server.
    if (!connected) ConnectToOmero(argc, argv);

    // We'll get the Image using Container Service.
    omero::api::IContainerPrx container_service =
        session->getContainerService();
    omero::sys::LongList id_list;
    if (argc >= 7) {
        image_id = atoi(argv[6]);
    }
    id_list.push_back(image_id);
    // Get the image list.
    omero::api::ImageList image_list = container_service->getImages(
        "Image", id_list, new omero::sys::ParametersI()
    );
    
    omero::model::ImagePtr image;
    if (image_list.size() == 0) {
        std::cout << "Image id: " << image_id << " not found\n";
        exit(-2);
    } else {
        image = image_list.at(0);
    }
    // Print the details.
    std::cout << "  Title: " << image->getName()->getValue() << "\n";
    std::string description = "";
    try {
        description = image->getDescription()->getValue();
    } catch (...) {}
    std::cout << "  Description: " << description << "\n";
    std::cout << "  # of Channels: ";
    std::cout << image->getPrimaryPixels()->getSizeC()->getValue()
              << "\n";
    std::cout << "  # of Timepoints: ";
    std::cout << image->getPrimaryPixels()->getSizeT()->getValue()
              << "\n";
    std::cout << "  Size X: "
              << image->getPrimaryPixels()->getSizeX()->getValue()
              << " pixels; "
              << image->getPrimaryPixels()->getPhysicalSizeX()->getValue()
              << "\n";
    std::cout << "  Size Y: "
              << image->getPrimaryPixels()->getSizeY()->getValue()
              << " pixels; "
              << image->getPrimaryPixels()->getPhysicalSizeY()->getValue()
              << "\n";
    std::cout << "  Size Z: "
              << image->getPrimaryPixels()->getSizeZ()->getValue()
              << " pixels; "
              << image->getPrimaryPixels()->getPhysicalSizeZ()->getValue()
              << "\n";
}

#endif
