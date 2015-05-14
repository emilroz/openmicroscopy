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

#ifndef _create_roi_h
#define _create_roi_h

#include "connect_to_omero.h"
#include <omero/api/IContainer.h>
#include <omero/sys/ParametersI.h>
#include <omero/model/RoiI.h>
#include <omero/model/ImageI.h>
#include <omero/model/RectI.h>
#include <omero/api/IUpdate.h>
#include <omero/RTypesI.h>
#include <omero/model/PixelsI.h>

// Create a ROI.
void CreateRoi(int argc, char* argv[])
{
    // Connect the server.
    if (!connected) ConnectToOmero(argc, argv);
    
    // Get Image
    omero::api::IContainerPrx container_service =
        session->getContainerService();
    omero::sys::LongList id_list;
    if (argc >= 7) {
        image_id = atoi(argv[6]);
    }
    id_list.push_back(image_id);
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
    
    // Create and link ROI
    omero::model::RoiPtr roi = new omero::model::RoiI();
    roi->setImage(image);
    // Add shape
    omero::model::RectPtr rect = new omero::model::RectI();
    rect->setX(
        omero::rtypes::rdouble(
            0.25 * image->getPrimaryPixels()->getSizeX()->getValue()
        )
    );
    rect->setY(
        omero::rtypes::rdouble(
            0.25 * image->getPrimaryPixels()->getSizeY()->getValue()
        )
    );
    rect->setWidth(
        omero::rtypes::rdouble(
            0.5 * image->getPrimaryPixels()->getSizeX()->getValue()
        )
    );
    rect->setHeight(
        omero::rtypes::rdouble(
            0.5 * image->getPrimaryPixels()->getSizeY()->getValue()
        )
    );
    rect->setTheZ(omero::rtypes::rint(0));
    rect->setTheT(omero::rtypes::rint(0));
    rect->setFillColor(omero::rtypes::rint(2113863680));
    rect->setStrokeColor(omero::rtypes::rint(2113863680));
    rect->setTextValue(omero::rtypes::rstring("NEW ROI"));
    roi->addShape(rect);
    
    roi = omero::model::RoiPtr::dynamicCast(
        session->getUpdateService()->saveAndReturnObject(roi)
    );
    std::cout << "ROI Created";
}

#endif
