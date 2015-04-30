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

#include <sys/time.h>
#include <iostream>
#include <omero/client.h>
#include <omero/api/IAdmin.h>
#include <omero/sys/ParametersI.h>
#include <omero/api/IContainer.h>
#include <omero/model/DatasetI.h>
#include <omero/model/Dataset.h>
#include <omero/model/DatasetAnnotationLink.h>
#include <omero/model/DatasetAnnotationLinkI.h>
#include <omero/model/ProjectI.h>
#include <omero/model/Project.h>
#include <omero/model/ProjectAnnotationLink.h>
#include <omero/model/ProjectAnnotationLinkI.h>
#include <omero/model/ImageI.h>
#include <omero/model/Image.h>
#include <omero/model/Pixels.h>
#include <omero/model/PixelsI.h>
#include <omero/model/Screen.h>
#include <omero/model/ScreenAnnotationLink.h>
#include <omero/model/ScreenAnnotationLinkI.h>
#include <omero/model/ScreenI.h>
#include <omero/model/ScreenPlateLink.h>
#include <omero/model/ScreenPlateLinkI.h>
#include <omero/model/Well.h>
#include <omero/model/WellI.h>
#include <omero/model/WellSample.h>
#include <omero/model/WellSampleI.h>
#include <omero/model/Plate.h>
#include <omero/model/PlateI.h>
#include <omero/RTypesI.h>
#include <omero/api/IQuery.h>
#include <omero/api/RawPixelsStore.h>
#include <omero/api/RawFileStore.h>
#include <omero/model/ProjectDatasetLink.h>
#include <omero/model/ProjectDatasetLinkI.h>
#include <omero/api/IUpdate.h>
#include <omero/model/TagAnnotation.h>
#include <omero/model/TagAnnotationI.h>
#include <omero/model/ImageAnnotationLink.h>
#include <omero/model/ImageAnnotationLinkI.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <openssl/sha.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sys/types.h>
#include <sys/stat.h>

#include <omero/model/OriginalFile.h>
#include <omero/model/OriginalFileAnnotationLink.h>
#include <omero/model/OriginalFileAnnotationLinkI.h>
#include <omero/model/OriginalFileI.h>
#include <omero/model/FileAnnotation.h>
#include <omero/model/FileAnnotationI.h>
#include <omero/api/IMetadata.h>

#include <omero/model/Roi.h>
#include <omero/model/RoiAnnotationLink.h>
#include <omero/model/RoiAnnotationLinkI.h>
#include <omero/model/RoiI.h>
#include <omero/model/Rect.h>
#include <omero/model/RectI.h>
#include <omero/model/Ellipse.h>
#include <omero/model/EllipseI.h>
#include <omero/model/Point.h>
#include <omero/model/PointI.h>
#include <omero/model/Polygon.h>
#include <omero/model/PolygonI.h>
#include <omero/model/Polyline.h>
#include <omero/model/PolylineI.h>
#include <omero/model/Line.h>
#include <omero/model/LineI.h>

#include <omero/api/IPixels.h>
#include <omero/model/PixelsType.h>
#include <omero/model/PixelsTypeI.h>
#include <omero/model/DatasetImageLink.h>
#include <omero/model/DatasetImageLinkI.h>

omero::client_ptr client;
omero::api::ServiceFactoryPrx session;
omero::api::IAdminPrx admin;
bool connected = false;
int image_id = 61;
int dataset_id = 6;
int plane_to_read = 60;


// Section 1. Retriving things.

// Connect to the server.
void example_1(int argc, char* argv[])
{    
    // Connection Data - host, port, username, password.
    Ice::InitializationData data;
    data.properties = Ice::createProperties();
    data.properties->setProperty("omero.host", argv[1]);
    data.properties->setProperty("omero.port", argv[2]);
    data.properties->setProperty("omero.user", argv[3]);
    data.properties->setProperty("omero.pass", argv[4]);
    // Connect to the server ...
    omero::client_ptr secure_client = new omero::client(data);
    secure_client->createSession();
    // ... and create unenrypted connection -> epecially useful when
    // downloading or uploading data.
    client = secure_client->createClient(false);
    session = client->getSession();
    session->closeOnDestroy();
    connected = true;
    // Print session details.
    admin = session->getAdminService();
    std::cout << "--------\n";
    std::cout << "User: " << admin->getEventContext()->userName << "\n"
              << "Id: "   << admin->getEventContext()->userId << "\n"
              << "Current group: " << admin->getEventContext()->groupName
              << "\n";
    std::cout << "---------\n";
}


// Print project Tree.
void example_2(int argc, char* argv[])
{
    // Connect to the server.
    if (!connected) example_1(argc, argv);
    
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

// Print Screen Tree.
void example_3(int argc, char* argv[])
{
    // Connect to the server.
    if (!connected) example_1(argc, argv);
    
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


// Get Image Info.
void example_4(int argc, char* argv[])
{
    // Connect the server.
    if (!connected) example_1(argc, argv);

    // We'll get the Image using Container Service.
    omero::api::IContainerPrx container_service =
        session->getContainerService();
    omero::sys::LongList id_list;
    id_list.push_back(image_id);
    // Get the image list.
    omero::api::ImageList image_list = container_service->getImages(
        "Image", id_list, new omero::sys::ParametersI()
    );
    omero::model::ImagePtr image = image_list.at(0);
    // Print the details.
    std::cout << "  Title: " << image->getName()->getValue() << "\n";
    std::cout << "  Description: "
              << image->getDescription()->getValue()
              << "\n";
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


// Get Pixels.
void example_5(int argc, char* argv[])
{
    // Connect to the server.
    if (!connected) example_1(argc, argv);

    // We'll get the Image using Container Service.
    omero::api::IContainerPrx container_service =
        session->getContainerService();
    omero::sys::LongList id_list;
    id_list.push_back(image_id);
    // Get the image list.
    omero::api::ImageList image_list = container_service->getImages(
        "Image", id_list, new omero::sys::ParametersI()
    );
    omero::model::ImagePtr image = image_list.at(0);
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
    cv::namedWindow( "Display window", CV_WINDOW_NORMAL);
    cv::imshow( "Display window", display_image );
    cv::waitKey(0);
}


// Section 2. Creating Things.

// Create dataset and link it to an existing project.
void example_6(int argc, char* argv[])
{
    // Connect to the server.
    if (!connected) example_1(argc, argv);
    // Print current Project tree.
    example_2(argc, argv);
    
    omero::model::DatasetPtr dataset = new omero::model::DatasetI();
    dataset->setName(omero::rtypes::rstring("Example Dataset Creation"));
    dataset->setDescription(
        omero::rtypes::rstring("Exmple Dataset Description")
    );
    
    // Create link to a Project.
    omero::model::ProjectDatasetLinkPtr link =
        new omero::model::ProjectDatasetLinkI();
    link->setChild(dataset);
    link->setParent(new omero::model::ProjectI(2, false));
    omero::model::IObjectPtr r =
        session->getUpdateService()->saveAndReturnObject(link);
    // Print update tree;
    std::cout << "\n\nUpdated Tree\n"; 
    example_2(argc, argv);
}


// Create a new tag and link it to the image.
void example_7(int argc, char* argv[])
{
    // Connect to the server.
    if (!connected) example_1(argc, argv);
    
    // Create new tag.
    omero::model::TagAnnotationIPtr tag = new omero::model::TagAnnotationI();
    tag->setTextValue(omero::rtypes::rstring("new tag"));
    tag->setDescription(omero::rtypes::rstring("new tag description"));
    
    omero::model::ImageAnnotationLinkPtr link =
        new omero::model::ImageAnnotationLinkI();
    link->setChild(tag);
    link->setParent(new omero::model::ImageI(image_id, false));
    omero::model::IObjectPtr r =
        session->getUpdateService()->saveAndReturnObject(link);
}


// Attach a file to an Image.
void example_8(int argc, char* argv[])
{
    // Connect to the server.
    if (!connected) example_1(argc, argv);

    // SHA1 the file.
    std::string path = "/Users/emilrozbicki/Desktop/temp_plot.py";
    std::ifstream file(path.c_str(), std::ifstream::binary);
    file.seekg(0, file.end);
    int file_length = file.tellg();
    file.seekg(0, file.beg);
    char buffer[file_length];
    file.read(buffer, file_length);
    unsigned char datSha[20];
    SHA1((unsigned char *) buffer, file_length, datSha);
    std::ostringstream os;
    os.fill('0');
    os << std::hex;
    for (const unsigned char * ptr = datSha; ptr < datSha + 20; ptr++) {
        os << std::setw(2) << (unsigned int) *ptr;
    }
    std::string hash = os.str();

    // Create Original File.
    omero::api::IUpdatePrx iUpdate = session->getUpdateService();
    omero::model::OriginalFilePtr original_file =
        new omero::model::OriginalFileI();
    original_file->setName(omero::rtypes::rstring(path));
    original_file->setPath(omero::rtypes::rstring(path));
    original_file->setSize(omero::rtypes::rlong(file_length));
    original_file->setHash(omero::rtypes::rstring(hash.c_str()));
    original_file->setMimetype(omero::rtypes::rstring(".txt"));
    original_file = omero::model::OriginalFilePtr::dynamicCast(
        iUpdate->saveAndReturnObject(original_file)
    );
    
    
    // Upload the file.
    omero::api::RawFileStorePrx file_store = session->createRawFileStore();
    file_store->setFileId(original_file->getId()->getValue());
    std::vector<Ice::Byte> bytes;
    bytes.resize(file_length);
    std::copy(
        reinterpret_cast<Ice::Byte *>(buffer),
        reinterpret_cast<Ice::Byte *>(buffer) + file_length, bytes.begin()
    );
    file_store->write(bytes, 0, file_length);
    original_file = file_store->save();
    file.close();
    file_store->close();

    // Create file annotation.
    omero::model::FileAnnotationPtr fa = new omero::model::FileAnnotationI();
    fa->setFile(original_file);
    fa->setDescription(omero::rtypes::rstring("test description"));
    fa->setNs(omero::rtypes::rstring("new_name_space"));
    fa = omero::model::FileAnnotationPtr::dynamicCast(
        iUpdate->saveAndReturnObject(fa)
    );

    // Link the file annotation.
    omero::model::ImagePtr image = new omero::model::ImageI(image_id, false);
    omero::model::ImageAnnotationLinkPtr image_link =
        new omero::model::ImageAnnotationLinkI();
    image_link->setChild(fa);
    image_link->setParent(image);
    image_link = omero::model::ImageAnnotationLinkPtr::dynamicCast(
        iUpdate->saveAndReturnObject(image_link)
    );
}


// Read attached file.
void example_9(int argc, char* argv[])
{
    // Connect to the server.
    if (!connected) example_1(argc, argv);
    
    // Find File Annotation for the current user,
    std::string name_space = "new_name_space"; 
    int user_id = admin->getEventContext()->userId;
    omero::api::StringSet include;
    std::ostringstream convert;
    include.push_back(name_space.c_str());
    omero::api::StringSet exclude;
    // Populate params
    omero::api::IMetadataPrx proxy = session->getMetadataService();
    omero::sys::ParametersIPtr param = new omero::sys::ParametersI();
    param->exp(user_id);
    omero::model::ImagePtr image = new omero::model::ImageI(image_id);
    // Get annotations
    omero::api::AnnotationList annotations = proxy->loadSpecifiedAnnotations(
            "ome.model.annotations.FileAnnotation", include, exclude, param
    );
    
    if (annotations.size() == 0) {
        std::cout << "No files found\n";
        return;
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


// Section 3. ROIs

// Create a ROI.
void example_10(int argc, char* argv[])
{
    // Connect the server.
    if (!connected) example_1(argc, argv);
    
    // Get Image
    omero::api::IContainerPrx container_service =
        session->getContainerService();
    omero::sys::LongList id_list;
    id_list.push_back(image_id);
    omero::api::ImageList image_list = container_service->getImages(
        "Image", id_list, new omero::sys::ParametersI()
    );
    omero::model::ImagePtr image = image_list.at(0);
    
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
    rect->setTheZ(omero::rtypes::rint(plane_to_read));
    rect->setTheT(omero::rtypes::rint(0));
    rect->setFillColor(omero::rtypes::rint(2113863680));
    rect->setStrokeColor(omero::rtypes::rint(2113863680));
    rect->setTextValue(omero::rtypes::rstring("NEW ROI"));
    roi->addShape(rect);
    
    roi = omero::model::RoiPtr::dynamicCast(
        session->getUpdateService()->saveAndReturnObject(roi)
    );
}


// Create an image.
void example_11(int argc, char* argv[])
{
    // Connect to the server.
    if (!connected) example_1(argc, argv);

    // Read existing image and copy a single plane to the new image.
    omero::api::IContainerPrx container_service =
        session->getContainerService();
    omero::sys::LongList id_list;
    id_list.push_back(image_id);
    // Get the image list.
    omero::api::ImageList image_list = container_service->getImages(
        "Image", id_list, new omero::sys::ParametersI()
    );
    omero::model::ImagePtr image = image_list.at(0);
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
    link->setParent(dataset.at(0));
    link->setChild(new_image_pointer);
    session->getUpdateService()->saveAndReturnObject(link);
    // Populate metadata
    new_image_pointer->getPrimaryPixels()->setPhysicalSizeX(
        image->getPrimaryPixels()->getPhysicalSizeX());
    new_image_pointer->getPrimaryPixels()->setPhysicalSizeY(
        image->getPrimaryPixels()->getPhysicalSizeY());
    new_image_pointer->getPrimaryPixels()->setPhysicalSizeZ(0);
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
    new_pixel_store->setPlane(bytes, 0, 0, 0);
    new_pixel_store->save();
    new_pixel_store->close();
}

int main (int argc, char* argv[])
{   
    if (std::string(argv[5]) == "example1") example_1(argc, argv);
    else if (std::string(argv[5]) == "example2") example_2(argc, argv);
    else if (std::string(argv[5]) == "example3") example_3(argc, argv);
    else if (std::string(argv[5]) == "example4") example_4(argc, argv);
    else if (std::string(argv[5]) == "example5") example_5(argc, argv);
    else if (std::string(argv[5]) == "example6") example_6(argc, argv);
    else if (std::string(argv[5]) == "example7") example_7(argc, argv);
    else if (std::string(argv[5]) == "example8") example_8(argc, argv);
    else if (std::string(argv[5]) == "example9") example_9(argc, argv);
    else if (std::string(argv[5]) == "example10") example_10(argc, argv);
    else if (std::string(argv[5]) == "example11") example_11(argc, argv);
    std::cout << "Success\n";
    return 0;
}
