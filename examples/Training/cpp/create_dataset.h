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

#ifndef _create_dataset_h
#define _create_dataset_h

#include "connect_to_omero.h"
#include "project_tree.h"
#include <omero/RTypesI.h>
#include <omero/api/IUpdate.h>
#include <omero/model/ProjectDatasetLinkI.h>

void CreateDataset(int argc, char* argv[])
{
    // Connect to the server.
    if (!connected) ConnectToOmero(argc, argv);

    omero::sys::LongList id_list;
    if (argc >= 7) {
        project_id = atoi(argv[6]);
    }
    id_list.push_back(project_id);
    omero::api::IContainerPrx container_service =
        session->getContainerService();
    omero::api::IObjectList object_list =
        container_service->loadContainerHierarchy(
            "Project",  id_list, new omero::sys::ParametersI()
        );
    
    if (object_list.size() == 0) {
        std::cout << "Project id: " << project_id << " not found\n";
        exit(-2);
    }

    omero::model::DatasetPtr dataset = new omero::model::DatasetI();
    dataset->setName(omero::rtypes::rstring("Example Dataset Creation"));
    dataset->setDescription(
        omero::rtypes::rstring("Exmple Dataset Description")
    );
    
    // Create link to a Project.
    omero::model::ProjectDatasetLinkPtr link =
        new omero::model::ProjectDatasetLinkI();
    link->setChild(dataset);
    link->setParent(new omero::model::ProjectI(project_id, false));
    omero::model::IObjectPtr r =
        session->getUpdateService()->saveAndReturnObject(link);
    // Print updated tree;
    std::cout << "\nNew dataset created.\n";
    ProjectTree(argc, argv);
}

#endif
