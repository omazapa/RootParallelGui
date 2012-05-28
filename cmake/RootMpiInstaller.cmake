
#Basic Information
set(CPACK_PACKAGE_NAME "root-plugin-mpi")
set(CPACK_PACKAGE_VENDOR "UdeA At Colombia")
set(CPACK_PACKAGE_CONTACT "andresete.chaos@gmail.com" "sigifredo89@gmail.com" "muzgash@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Nowadays it is necessary to paralellize code most importantly within science areas and given the advanages that ROOT brings to the scientific calculations, mpi appears as an essential extension just because with this support it will be much easier for the user to write efficient and distributed routines without the need to resort to complicated syntaxes which we face when writing code with this separated dependencies. ROOT lacks this capability and we are giving this advantage to it to expand his usage and demand.")

IF(${CMAKE_HOST_SYSTEM} MATCHES "Linux")
     SET(CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS},libqt4-core  (>= 4.7),libqt4-gui (>= 4.7),openmpi-bin")
ENDIF()

IF (WIN32)
#Not implemented yet
ELSEIF(UNIX)
	EXECUTE_PROCESS(COMMAND "date" "+%d/%m/%Y" OUTPUT_VARIABLE DATE)
	STRING(REPLACE "/" ";"  RESULT_LIST ${DATE})
	LIST(GET RESULT_LIST 0 DAY)
	LIST(GET RESULT_LIST 1 MONTH)
	LIST(GET RESULT_LIST 2 _YEAR)
	STRING(REPLACE "\n" ""  YEAR  ${_YEAR} )
ENDIF()

#VERSION INFORMATION
set(MAJOR_VERSION  ${YEAR})
set(MINOR_VERSION  ${MONTH})
set(PATCH_VERSION  ${DAY})
set(PACKAGE_VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})
set(CPACK_PACKAGE_VERSION "${PACKAGE_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR "${MAJOR_VERSION}")
set(CPACK_PACKAGE_VERSION_MINOR "${MINOR_VERSION}")
set(CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "ROOT")

#READING FILES FOR INSTALLER
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/doc/CopyRight.txt")
# set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/doc/README.txt")
# SET(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_CURRENT_SOURCE_DIR}/doc/Wellcome.txt")

set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}")
    


IF(${CMAKE_HOST_SYSTEM} MATCHES "Linux")
    SET(CPACK_GENERATOR "TGZ;STGZ;DEB")

    SET(CPACK_SET_DESTDIR "on")

    SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "andresete.chaos@gmail.com" "sigifredo89@gmail.com" "muzgash@gmail.com")

    SET(CPACK_DEBIAN_PACKAGE_DEPENDS "root-system  (>= 5.32.01-1)")

    SET(CPACK_DEBIAN_PACKAGE_PRIORITY "extra")
    SET(CPACK_DEBIAN_PACKAGE_SECTION "science")
    SET(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
ELSEIF(${CMAKE_HOST_SYSTEM} MATCHES "Windows")
#not implemented yet
ENDIF()

MACRO(MINSTALLTARGET MTYPE )
  FOREACH(MTARGET ${ARGN})
    IF(${MTYPE} MATCHES "LIB")
      IF(${CMAKE_HOST_SYSTEM} MATCHES "Linux")
        INSTALL(TARGETS  ${MTARGET} LIBRARY DESTINATION ${ROOTSYS}/lib/root/${ROOTVERSION} )
      ELSEIF(${CMAKE_HOST_SYSTEM} MATCHES "Windows")
        #not implemented yet
      ENDIF()
    ELSEIF(${MTYPE} MATCHES "MACRO")
      IF(${CMAKE_HOST_SYSTEM} MATCHES "Linux" )
        INSTALL(TARGETS  ${MTARGET} LIBRARY DESTINATION ${ROOTSYS}/share/root/macros/)
      ELSEIF(${CMAKE_HOST_SYSTEM} MATCHES "Windows")
        #not implemented yet
      ENDIF()
    ELSEIF(${MTYPE} MATCHES "BIN")
      IF(${CMAKE_HOST_SYSTEM} MATCHES "Linux" )
        INSTALL(TARGETS  ${MTARGET} RUNTIME DESTINATION ${ROOTSYS}/bin )
      ELSEIF(${CMAKE_HOST_SYSTEM} MATCHES "Windows")
	#not implemented yet
      ENDIF()
    ELSEIF(${MTYPE} MATCHES "DOC")
      IF(${CMAKE_HOST_SYSTEM} MATCHES "Linux")
	      STRING(REPLACE "/" ";" SPLIT_PATH ${MTARGET} )
	      LIST(REVERSE SPLIT_PATH )
	      LIST(GET SPLIT_PATH 1 PLUGIN_NAME )
	      INSTALL(DIRECTORY  ${MTARGET} DESTINATION ${ROOTSYS}/share/doc/${PLUGIN_NAME} )
      ELSEIF(${CMAKE_HOST_SYSTEM} MATCHES "Windows")
	    #not implemented yet
      ELSEIF(${CMAKE_HOST_APPLE})
	    #not implemented yet
      ENDIF()      
    ELSE()
      MESSAGE(STATUS "Unknow Target ${MTYPE}")
    ENDIF()
  ENDFOREACH(MTARGET)
ENDMACRO(MINSTALLTARGET)

MACRO(MINSTALLFILE MTYPE )
  FOREACH(MTARGET ${ARGN})
    IF(${MTYPE} MATCHES "LIB")
      IF(${CMAKE_HOST_SYSTEM} MATCHES "Linux")
        INSTALL(FILES  ${MTARGET} DESTINATION ${ROOTSYS}/lib/root/ )
      ELSEIF(${CMAKE_HOST_SYSTEM} MATCHES "Windows")
        #not implemented yet
      ENDIF()
    ELSEIF(${MTYPE} MATCHES "MACRO")
      IF(${CMAKE_HOST_SYSTEM} MATCHES "Linux" )
        INSTALL(FILES  ${MTARGET} DESTINATION ${ROOTSYS}/share/root/macros/ )
      ELSEIF(${CMAKE_HOST_SYSTEM} MATCHES "Windows")
        #not implemented yet
      ENDIF()
    ELSEIF(${MTYPE} MATCHES "HEADER")
      IF(${CMAKE_HOST_SYSTEM} MATCHES "Linux" )
        INSTALL(FILES  ${MTARGET} DESTINATION ${ROOTSYS}/include/root/ )
      ELSEIF(${CMAKE_HOST_SYSTEM} MATCHES "Windows")
        #not implemented yet
      ENDIF()
    ELSEIF(${MTYPE} MATCHES "BIN")
      IF(${CMAKE_HOST_SYSTEM} MATCHES "Linux" )
        INSTALL(PROGRAMS  ${MTARGET} DESTINATION ${ROOTSYS}/bin )
      ELSEIF(${CMAKE_HOST_SYSTEM} MATCHES "Windows")
	#not implemented yet
      ENDIF()
    ELSEIF(${MTYPE} MATCHES "DOC")
      IF(${CMAKE_HOST_SYSTEM} MATCHES "Linux")
	      STRING(REPLACE "/" ";" SPLIT_PATH ${MTARGET} )
	      LIST(REVERSE SPLIT_PATH )
	      LIST(GET SPLIT_PATH 1 PLUGIN_NAME )
	      INSTALL(DIRECTORY  ${MTARGET} DESTINATION ${ROOTSYS}/share/doc/${PLUGIN_NAME} )
      ELSEIF(${CMAKE_HOST_SYSTEM} MATCHES "Windows")
	    #not implemented yet
      ELSEIF(${CMAKE_HOST_APPLE})
	    #not implemented yet
      ENDIF()      
    ELSE()
      MESSAGE(STATUS "Unknow Target ${MTYPE}")
    ENDIF()
  ENDFOREACH(MTARGET)
ENDMACRO(MINSTALLFILE)

MACRO(MINSTALL_H DIR)
    IF(${CMAKE_HOST_SYSTEM} MATCHES "Linux")
      INSTALL(DIRECTORY ${DIR} DESTINATION ${ROOTSYS}/include/root/ )
    ELSEIF(${CMAKE_HOST_SYSTEM} MATCHES "Windows")
	#not implemented yet
    ENDIF()
ENDMACRO(MINSTALL_H)


