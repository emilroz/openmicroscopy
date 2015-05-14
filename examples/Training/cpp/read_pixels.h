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

#ifndef _read_pixels_h
#define _read_pixels_h

#include "connect_to_omero.h"
#include <omero/api/IContainer.h>
#include <omero/sys/ParametersI.h>
#include <omero/model/PixelsI.h>
#include <omero/model/ImageI.h>
#include <omero/api/RawPixelsStore.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void ReadPixels(int argc, char* argv[])
{
    // Connect to the server.
    if (!connected) ConnectToOmero(argc, argv);

    // We'll get the Image using Container Service.
    omero::api::IContainerPrx container_service =
        session->getContainerService();
    omero::sys::LongList id_list;
    if (argc >= 7) {
        image_id = atoi(argv[6]);
    }
    if (argc >= 8) {
        plane_to_read = atoi(argv[7]);
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
    
    int image_size_z = image->getPrimaryPixels()->getSizeZ()->getValue();
    
    if (plane_to_read + 1 > image_size_z) {
        std::cout << "Error: plane_to_read > image_size_z\n";
        exit(-2);
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
    image_ice_container = pixel_store->getPlane(plane_to_read, 0, 0);
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
    
    // Use OpenCV to show the image.
    int size_x = image->getPrimaryPixels()->getSizeX()->getValue();
    int size_y = image->getPrimaryPixels()->getSizeY()->getValue();
    cv::Mat opencv_image = cv::Mat(
        size_y, size_x, CV_8U, image_cast
    );
    
    cv::Mat display_image = cv::Mat::zeros(
        int(size_y * 0.5), int(size_x * 0.5), CV_8U
    );
    cv::resize(
        opencv_image, display_image, display_image.size(),
        0, 0, CV_INTER_LINEAR
    );
    cv::namedWindow( "OMERO Image", CV_WINDOW_NORMAL);
    cv::imshow( "OMERO Image", display_image );
    cv::waitKey(0);
}

#endif
