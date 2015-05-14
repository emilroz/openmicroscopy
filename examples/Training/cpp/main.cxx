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

#include <stdio.h>
#include <omero/client.h>
#include <omero/api/IAdmin.h>

omero::client_ptr client;
omero::api::ServiceFactoryPrx session;
omero::api::IAdminPrx admin;
bool connected = false;
int image_id = 1;
int dataset_id = 1;
int plane_to_read = 1;
int project_id = 1;
std::string tag_name = "Test Tag Name";
std::string tag_description = "Test Tag Description";
std::string file_path = "";

#include "connect_to_omero.h"
#include "project_tree.h"
#include "screen_tree.h"
#include "image_info.h"
#include "read_pixels.h"
#include "create_dataset.h"
#include "create_tag.h"
#include "attach_file.h"
#include "read_attachment.h"
#include "create_roi.h"
#include "create_image.h"

int main (int argc, char* argv[])
{   
    if (argc < 6) {
        std::cout << "Not enough parameters!!!!\n"
                  << "Pass following details (order is important): "
                  << "server_name port user pass name_of_example(header file)";
    }
    if (std::string(argv[5]) == "connect_to_omero") ConnectToOmero(argc, argv);
    else if (std::string(argv[5]) == "project_tree") ProjectTree(argc, argv);
    else if (std::string(argv[5]) == "screen_tree") ScreenTree(argc, argv);
    else if (std::string(argv[5]) == "image_info") ImageInfo(argc, argv);
    else if (std::string(argv[5]) == "read_pixels") ReadPixels(argc, argv);
    else if (std::string(argv[5]) == "create_dataset") CreateDataset(argc, argv);
    else if (std::string(argv[5]) == "create_tag") CreateTag(argc, argv);
    else if (std::string(argv[5]) == "attach_file") AttachFile(argc, argv);
    else if (std::string(argv[5]) == "read_attachment") ReadAttachment(argc, argv);
    else if (std::string(argv[5]) == "create_roi") CreateRoi(argc, argv);
    else if (std::string(argv[5]) == "create_image") CreateImage(argc, argv);
    else {
        std::cout << "\n" << std::string(argv[5])
                  << " example does not exist, check spelling?\n\n";
        exit(-1);
    }
    std::cout << "\nSuccess\n";
    return 0;
}