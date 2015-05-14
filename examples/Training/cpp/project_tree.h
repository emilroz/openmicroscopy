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
 
 
#ifndef _project_tree_h
#define _project_tree_h

#include "connect_to_omero.h"
#include <omero/sys/ParametersI.h>
#include <omero/api/IContainer.h>
#include <omero/model/DatasetI.h>
#include <omero/model/ProjectI.h>
#include <omero/model/ImageI.h>

// Print project Tree.
void ProjectTree(int argc, char* argv[])
{
    // Connect to the server.
    if (!connected) ConnectToOmero(argc, argv);
    
    // We'll use container service to get info about user's Projects.
    omero::api::IContainerPrx container_service =
        session->getContainerService();
    int userID = admin->getEventContext()->userId;
    // Add user ID to paramerters list.
    omero::sys::ParametersIPtr param = new omero::sys::ParametersI();
    param->exp(userID);
    param->leaves();
    omero::sys::LongList emptyArray;
    // Get Project list.
    omero::api::IObjectList projects =
        container_service->loadContainerHierarchy(
            "Project", emptyArray, param
        );
    // Container service return IObjectList so we need to cast it
    // to ProjectPtr list.
    std::vector<omero::model::ProjectPtr> project_list =
        omero::cast<omero::model::ProjectPtr>(projects);
    // To print full Project tree we'll add dataset and image lists.
    omero::model::ProjectLinkedDatasetSeq dataset_list;
    omero::model::DatasetLinkedImageSeq image_list;
    
    // Loop through projects in the Project list.
    std::cout << "Project Tree\n";
    std::cout << ".\n"; 
    for (int p = 0; p < project_list.size(); p++) {
        std::cout << "|--- " 
                  << project_list.at(p)->getName()->getValue()
                  << " (Id: " << project_list.at(p)->getId()->getValue()
                  << ") \n";
        // Get Datasets linked to the given Project.
        dataset_list = project_list.at(p)->linkedDatasetList();
        for (int d = 0; d < dataset_list.size(); d++) {
             std::cout << "|\t|--- "
                       << dataset_list.at(d)->getName()->getValue()
                       << " (Id: " << dataset_list.at(d)->getId()->getValue()
                       << ") \n";
            // Get Images linked to the given Dataset.
            image_list = dataset_list.at(d)->linkedImageList();
            for (int i = 0; i < image_list.size(); i++) {
                std::cout << "|\t|\t|--- "
                            << image_list.at(i)->getName()->getValue()
                          << " (Id: " << image_list.at(i)->getId()->getValue()
                          << ") \n";
            }
        }
        
    }
}

#endif
