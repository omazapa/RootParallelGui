#/***************************************************************************
#*   Copyright (C) by Omar Andres Zapata Mesa                               *
#*   email:andresete.chaos@gmail.com                                        *
#*                                                                          *
#*   Grupo de Fenomenologia de Interacciones Fundamentales                  *
#*   Universidad de Antioquia At Medellin - Colombia                        *
#*                                                                          *
#***************************************************************************/


#######################
# RootParallelGuiMpi  #
#######################

INCLUDE_DIRECTORIES ( ${CMAKE_SOURCE_DIR}/include ${CMAKE_BINARY_DIR} ${QT_INCLUDES} )

IF(${CMAKE_HOST_SYSTEM} MATCHES "Linux")
     MESSAGE("-- SYSTEM = UNIX")
     ADD_DEFINITIONS( -fPIC )
ENDIF()

SET(ROOT_PARALLEL_GUI_MPI_MOC_HDRS
   ${CMAKE_SOURCE_DIR}/include/RootParallelGuiMpiLauncher.h
   ${CMAKE_SOURCE_DIR}/include/RootParallelGuiMpiConfig.h
)

SET(ROOT_PARALLEL_GUI_MPI_SRCS
   ${CMAKE_SOURCE_DIR}/src/RootParallelGuiMpiLauncher.cpp
   ${CMAKE_SOURCE_DIR}/src/RootParallelGuiMpiConfig.cpp
   ${CMAKE_SOURCE_DIR}/src/RootParallelGuiCintMpi.cpp
)

SET(ROOT_PARALLEL_GUI_MPI_UIS
   ${CMAKE_SOURCE_DIR}/uis/RootParallelGuiMpi/RootParallelGuiMpiLauncher.ui
   ${CMAKE_SOURCE_DIR}/uis/RootParallelGuiMpi/RootParallelGuiMpiConfig.ui
)

SET(ROOT_PARALLEL_GUI_MPI_RCS
   ${CMAKE_SOURCE_DIR}/resources/RootParallelGuiMpi.qrc
)


QT4_ADD_RESOURCES(ROOT_PARALLEL_GUI_MPI_RCS_SRCS
                    ${ROOT_PARALLEL_GUI_MPI_RCS}
)

QT4_WRAP_UI(ROOT_PARALLEL_GUI_MPI_UIS_HDRS
   ${ROOT_PARALLEL_GUI_MPI_UIS}
)

QT4_WRAP_CPP(ROOT_PARALLEL_GUI_MPI_MOC_SRCS
   ${ROOT_PARALLEL_GUI_MPI_MOC_HDRS}
)

ADD_LIBRARY(ParallelGuiMpi SHARED 
${ROOT_PARALLEL_GUI_MPI_UIS_HDRS}
${ROOT_PARALLEL_GUI_MPI_MOC_SRCS}
${ROOT_PARALLEL_GUI_MPI_SRCS}
${ROOT_PARALLEL_GUI_MPI_RCS_SRCS}
)
ADD_LIBRARY(ParallelGuiMpiStatic STATIC
${ROOT_PARALLEL_GUI_MPI_UIS_HDRS}
${ROOT_PARALLEL_GUI_MPI_MOC_SRCS}
${ROOT_PARALLEL_GUI_MPI_SRCS}
${ROOT_PARALLEL_GUI_MPI_RCS_SRCS}
)

TARGET_LINK_LIBRARIES(ParallelGuiMpi ParallelGuiMacroEditorStatic ${ROOT_LIBS} ${QT_LIBRARIES})
ADD_DEPENDENCIES(ParallelGuiMpi ParallelGuiMacroEditorStatic)

MINSTALLTARGET("LIB" ParallelGuiMpi)
