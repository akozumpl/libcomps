set (libcomps_SOURCES comps.c comps_list.c comps_hslist.c comps_dict.c
     comps_elem.c comps_radix.c comps_mradix.c comps_bradix.c comps_set.c
     comps_parse.c comps_logger.c comps_utils.c)
set (libcomps_HEADERS comps.h comps_list.h comps_hslist.h comps_dict.h
     comps_elem.h comps_radix.h comps_mradix.h comps_bradix.h comps_set.h
     comps_parse.h comps_logger.h comps_utils.h comps_types.h)

configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/libcomps_config.h.in"
                "${PROJECT_SOURCE_DIR}/libcomps_config.h")

include_directories(${LIBXML2_INCLUDE_DIR})
file(GLOB files "${PROJECT_SOURCE_DIR}/src/*.h")
file(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/src/libcomps")

foreach(file ${files})
    file(COPY ${file} DESTINATION "${PROJECT_SOURCE_DIR}/src/libcomps/")
endforeach()


add_library(libcomps SHARED ${libcomps_SOURCES})
target_link_libraries(libcomps ${EXPAT_LIBRARY})
target_link_libraries(libcomps ${LIBXML2_LIBRARIES})
set_target_properties(libcomps PROPERTIES OUTPUT_NAME "comps")
set_target_properties(libcomps PROPERTIES SOVERSION ${VERSION})

IF (CMAKE_SIZEOF_VOID_P MATCHES "8")
    SET (LIB_SUFFIX "64")
ENDIF (CMAKE_SIZEOF_VOID_P MATCHES "8")
set (LIB_INST_DIR ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})

install (FILES ${libcomps_HEADERS} DESTINATION include/libcomps)
install (TARGETS libcomps LIBRARY DESTINATION ${LIB_INST_DIR})

add_subdirectory(python)

