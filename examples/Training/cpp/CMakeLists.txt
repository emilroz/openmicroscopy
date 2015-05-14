cmake_minimum_required(VERSION 2.8)

project(examples)

set(CMAKE_PREFIX_PATH "/usr/local/lib/cmake/;${CMAKE_PREFIX_PATH}")
set(CMAKE_MODULE_PATH "/usr/local/lib/cmake/;${CMAKE_MODULE_PATH}")

find_package(Ice 3.4.0 REQUIRED COMPONENTS Ice IceUtil Glacier2)
find_package(omero-ice REQUIRED)
find_package(omero-client REQUIRED)
find_package(OpenCV REQUIRED)

MESSAGE(STATUS "OMERO CLIENT: " ${OMERO_CLIENT_LIBRARY})
MESSAGE(STATUS "OMERO CLIENT: " ${OMERO_ICE_LIBRARY})

include_directories(${OMERO_ICE_INCLUDE_DIR} ${OMERO_CLIENT_INCLUDE_DIR}
                    ${Ice_INCLUDE_DIRS} ${Ice_SLICE_DIRS}
                    ${OpenCV_INCLUDE_DIRS})
link_directories()

add_executable(examples Main.cxx
                        connect_to_omero.h
                        project_tree.h
                        screen_tree.h
                        image_info.h
                        read_pixels.h
                        create_dataset.h
                        create_tag.h
                        attach_file.h
                        read_attachment.h
                        create_roi.h
                        create_image.h)
target_link_libraries(examples ${OMERO_CLIENT_LIBRARY} ${OMERO_ICE_LIBRARY}
                               ${Ice_LIBRARIES} ${OpenCV_LIBS}
                               libssl.dylib libcrypto.dylib)