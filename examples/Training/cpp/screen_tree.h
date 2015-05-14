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
 

#ifndef _screen_tree_h
#define _screen_tree_h

#include "connect_to_omero.h"
#include <omero/api/IContainer.h>
#include <omero/sys/ParametersI.h>
#include <omero/model/ScreenI.h>
#include <omero/model/PlateI.h>
#include <omero/model/WellI.h>
#include <omero/model/WellSampleI.h>
#include <omero/model/ImageI.h>
#include <omero/api/IQuery.h>

// Print Screen Tree.
void ScreenTree(int argc, char* argv[])
{
    // Connect to the server.
    if (!connected) ConnectToOmero(argc, argv);
    
    // We will user Container Service to get a list of the user Screens.
    omero::api::IContainerPrx container_service =
        session->getContainerService();
    omero::sys::ParametersIPtr param = new omero::sys::ParametersI();
    param->exp(admin->getEventContext()->userId);
    param->leaves();
    omero::sys::LongList emptyArray;
    // Get the list.
    omero::api::IObjectList screens =
        container_service->loadContainerHierarchy(
            "omero.model.Screen", emptyArray, param
        );
    // Cast the IObjectList to ScreenPtr.
    std::vector<omero::model::ScreenPtr> screen_list =
        omero::cast<omero::model::ScreenPtr>(screens);
    // To print full Screen tree we will retrive plate and well lists.
    omero::model::ScreenLinkedPlateSeq plate_list;
    omero::model::PlateWellsSeq well_list;
    
    // We need to use Query Service to get the wells.
    omero::model::WellIPtr well;
    std::vector<omero::model::WellIPtr> wells;
    omero::model::WellSamplePtr sample;
    omero::sys::ParametersIPtr params = new omero::sys::ParametersI();
    omero::api::IQueryPrx query_service = session->getQueryService();
    omero::api::IObjectList query_results;
    std::string well_query =
        "select well from Well as well "
        "left outer join fetch well.plate as pt "
        "left outer join fetch well.wellSamples as ws "
        "left outer join fetch ws.plateAcquisition as pa "
        "left outer join fetch ws.image as img "
        "left outer join fetch img.pixels as pix "
        "left outer join fetch pix.pixelsType as pt "
        "where well.plate.id = :plateID";
    
    // Loops through the Screen list and print info.
    std::cout << "Screen Tree\n";
    std::cout << ".\n"; 
    for (int s = 0; s < screen_list.size(); s++) {
        std::cout << "|--- " 
                  << screen_list.at(s)->getName()->getValue()
                  << " (Id: " << screen_list.at(s)->getId()->getValue()
                  << ") \n";
        // Get Plates linked to the Screen.
        plate_list = screen_list.at(s)->linkedPlateList();
        for (int p = 0; p < plate_list.size(); p++) {
            std::cout << "|\t|--- "
                       << plate_list.at(p)->getName()->getValue()
                       << " (Id: " << plate_list.at(p)->getId()->getValue()
                       << ")\n";
            params = new omero::sys::ParametersI();
            params->add("plateID", plate_list.at(p)->getId());
            // Run query to find wells linked to the plate.
            query_results =
                query_service->findAllByQuery(well_query, params);
            // Cast IObjectList to WellIPtr list
            // and loops through the wells.
            wells = omero::cast<omero::model::WellIPtr>(query_results);
            for (int w = 0; w < wells.size(); w++) {
                well = wells[w];
                std::cout << "|\t|\t|--- Well["
                       << well->getColumn()->getValue() << ","
                       << well->getRow()->getValue()
                       << "] (Id: " << well->getId()->getValue()
                       << ")\n";
                // Each well can contain number of fields.
                // Loop though the fields if well contains any.
                if (well->sizeOfWellSamples() == 0) continue;
                sample = well->getWellSample(0);
                for (int f = 0; f < well->sizeOfWellSamples(); f++) {
                    sample = well->getWellSample(f);
                    std::cout << "|\t|\t|\t|--- Field[" << f << "] "
                    << " " << sample->getImage()->getName()->getValue()
                    << " (Id: "
                    << sample->getImage()->getId()->getValue()
                    << ")\n";
                }
            }
        }
    }
}
#endif
