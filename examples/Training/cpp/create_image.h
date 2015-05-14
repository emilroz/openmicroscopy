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

#ifndef _create_image_h
#define _create_image_h

#include "connect_to_omero.h"
#include <omero/api/IContainer.h>
#include <omero/sys/ParametersI.h>
#include <omero/model/PixelsI.h>
#include <omero/model/ImageI.h>
#include <omero/model/LengthI.h>
#include <omero/api/RawPixelsStore.h>
#include <omero/model/DatasetI.h>
#include <omero/api/IUpdate.h>
#include <omero/model/DatasetImageLinkI.h>
#include <omero/model/PixelsTypeI.h>
#include <omero/api/IPixels.h>

// Create an image.
void CreateImage(int argc, char* argv[])
{
    // Connect to the server.
    if (!connected) ConnectToOmero(argc, argv);

    // Read existing image and copy a single plane to the new image.
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
    // Create Raw Pixel Store.
    omero::api::RawPixelsStorePrx pixel_store =
        session->createRawPixelsStore();
    pixel_store->setPixelsId(
        image->getPrimaryPixels()->getId()->getValue(),
        false
    );
    // Read a Plane to Ice::Byte buffer.
    std::vector<Ice::Byte> image_ice_container;
    image_ice_container = pixel_store->getPlane(0, 0, 0);
    // Copy to unsigned char* buffer
    // that will be passed to OpenCV image for display.
    unsigned char *image_cast =
        (unsigned char*) malloc (pixel_store->getPlaneSize());
    memcpy(
        image_cast,
        reinterpret_cast<unsigned char *>(&image_ice_container[0]),
        pixel_store->getPlaneSize()
    );
    pixel_store->save();
    pixel_store->close();
    image_ice_container.clear();
    
    
    // Create new image.
    int width = image->getPrimaryPixels()->getSizeX()->getValue();
    int height = image->getPrimaryPixels()->getSizeY()->getValue();
    int depth = 1;
    int number_of_timepoints = 1;
    int number_of_channels = 1;
    std::string name = "New Image";
    std::string description = "New Image description";
    omero::model::PixelsTypePtr pixel_type =
        image->getPrimaryPixels()->getPixelsType();
    
    omero::sys::IntList list;
    for (int i = 0; i < number_of_channels; i++) {
        list.push_back(i);
    }
    
    omero::api::IPixelsPrx pixel_service = session->getPixelsService();
    omero::RLongPtr new_id = pixel_service->createImage(
        width, height, depth, number_of_timepoints,
        list, pixel_type, name, description
    );

    std::cout << "New Image ID: " << new_id->getValue() << "\n";
    
    // Link image to dataset.
    omero::sys::LongList dataset_list;
    if (argc >= 8) {
        dataset_id = atoi(argv[7]);
    }
    dataset_list.push_back(dataset_id);
    omero::api::IObjectList datasets =
        container_service->loadContainerHierarchy(
            "Dataset", dataset_list, new omero::sys::ParametersI()
    );
    std::vector<omero::model::DatasetPtr> dataset =
        omero::cast<omero::model::DatasetPtr>(datasets);
    id_list.clear();
    id_list.push_back(new_id->getValue());
    omero::api::ImageList new_image =
        container_service->getImages(
            "Image", id_list, new omero::sys::ParametersI()
    );
    omero::model::DatasetImageLinkIPtr link =
        new omero::model::DatasetImageLinkI();
    omero::model::ImagePtr new_image_pointer = new_image.at(0);
    if (dataset.size() == 0) {
        std::cout << "Daset ID: " << dataset_id
                  << " does not exists creating orphan\n";
    } else {
        link->setParent(dataset.at(0));
        link->setChild(new_image_pointer);
        session->getUpdateService()->saveAndReturnObject(link);
    }
    // Populate metadata
    omero::model::LengthPtr length_size_x =
        new omero::model::LengthI(
            image->getPrimaryPixels()->getPhysicalSizeX()->getValue(),
            omero::model::enums::MICROMETER
        );
    omero::model::LengthPtr length_size_y =
        new omero::model::LengthI(
            image->getPrimaryPixels()->getPhysicalSizeY()->getValue(),
            omero::model::enums::MICROMETER
        );
    omero::model::LengthPtr length_size_z =
        new omero::model::LengthI(
            1.0, omero::model::enums::MICROMETER
        );

    new_image_pointer->getPrimaryPixels()->setPhysicalSizeX(length_size_x);
    new_image_pointer->getPrimaryPixels()->setPhysicalSizeY(length_size_y);
    new_image_pointer->getPrimaryPixels()->setPhysicalSizeZ(length_size_z);
    new_image_pointer = omero::model::ImageIPtr::dynamicCast(
        session->getUpdateService()->saveAndReturnObject(new_image_pointer)
    );
    std::cout << "Created new image: "
              << new_image_pointer->getName()->getValue()
              << " ID: "
              << new_image_pointer->getId()->getValue()
              << "\n";

    // Write plane to the created image.
    omero::api::RawPixelsStorePrx new_pixel_store =
        session->createRawPixelsStore();
    new_pixel_store->setPixelsId(
        new_image_pointer->getPrimaryPixels()->getId()->getValue(),
        false
    );
    
    int size = width * height;
    std::vector<Ice::Byte> bytes;
    bytes.resize(size);
    std::copy(
        reinterpret_cast<Ice::Byte *>(image_cast),
        reinterpret_cast<Ice::Byte *>(image_cast) + size,
        bytes.begin()
    );
    std::cout << "Uploading plane\n";
    new_pixel_store->setPlane(bytes, 0, 0, 0);
    new_pixel_store->save();
    new_pixel_store->close();
}
#endif
