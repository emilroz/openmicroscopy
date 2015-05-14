//
//  read_attachment.h
//  
//
//  Created by Emil Rozbicki on 14/05/2015.
//
//

#ifndef _read_attachment_h
#define _read_attachment_h

#include <fstream>
#include <iostream>

#include "connect_to_omero.h"
#include <omero/api/IContainer.h>
#include <omero/sys/ParametersI.h>
#include <omero/api/RawFileStore.h>
#include <omero/model/OriginalFileI.h>
#include <omero/model/FileAnnotationI.h>
#include <omero/api/IMetadata.h>

// Read attached file.
void ReadAttachment(int argc, char* argv[])
{
    // Connect to the server.
    if (!connected) ConnectToOmero(argc, argv);
    
    // Find File Annotation for the current user,
    std::string name_space = "new_name_space"; 
    int user_id = admin->getEventContext()->userId;
    omero::api::StringSet include;
    include.push_back(name_space.c_str());
    omero::api::StringSet exclude;
    // Populate params
    omero::api::IMetadataPrx proxy = session->getMetadataService();
    omero::sys::ParametersIPtr param = new omero::sys::ParametersI();

    // Get annotations
    omero::api::AnnotationList annotations = proxy->loadSpecifiedAnnotations(
            "ome.model.annotations.FileAnnotation", include, exclude, param
    );
    
    if (annotations.size() == 0) {
        std::cout << "No files found\n";
        exit(-2);
    }
    std::cout << "Found " << annotations.size() << " files.\n";
    
    // Read the file.
    omero::api::RawFileStorePrx store = session->createRawFileStore();
    omero::model::FileAnnotationPtr fa =
        omero::model::FileAnnotationIPtr::dynamicCast(annotations.at(0));
    omero::model::OriginalFilePtr original_file = fa->getFile();
    store->setFileId(original_file->getId()->getValue());
    long file_size = original_file->getSize()->getValue();
    char* buffer = (char*) malloc(file_size);
    std::ifstream file;
    std::vector<Ice::Byte> bytes;
    bytes.resize(file_size);
    bytes = store->read(0, file_size);
    memcpy(buffer,
        reinterpret_cast<unsigned char *>(&bytes[0]),
        file_size
    );
    std::istringstream iss(std::string(buffer, file_size));
    std::string line;
    std::istringstream ss(line);
    std::istream_iterator<std::string> begin(ss), end;
    std::cout << "\nPrinting out "
              << original_file->getName()->getValue() << "\n";
    while(getline(iss, line)){
        std::cout << "\t" << line << "\n";
    }
    std::cout << "End of File\n";
}

#endif
