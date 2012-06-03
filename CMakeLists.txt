project(root-parallel-gui)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)
SET(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/cmake")
FIND_PACKAGE(Qt4 REQUIRED  QtCore QtGui QtNetwork QtMain)
INCLUDE(${QT_USE_FILE})
FIND_PACKAGE(MPI)

##################
#ROOT ENVIRONMENT#
##################
IF(${CMAKE_HOST_SYSTEM} MATCHES "Linux")
     INCLUDE(CMakeForceCompiler)
     CMAKE_FORCE_CXX_COMPILER(mpic++ "MPI C++ Compiler")
     MESSAGE("-- SYSTEM = UNIX")
     SET(ROOTSYS "/usr")
     ADD_DEFINITIONS( -Wall -g )
     SET(ROOT_LIBS Core Matrix MathCore Cint Net RIO Hist MathMore GenVector Physics)
ELSEIF(${CMAKE_HOST_SYSTEM} MATCHES "Windows")
     MESSAGE("-- SYSTEM = Windows")
        IF(MSVC10)
                SET(CMAKE_BUILD_TYPE Release)
                SET(ROOTSYS "C:\\root")
                ADD_DEFINITIONS(/TP /EHsc /D "OMPI_IMPORTS" /D "OPAL_IMPORTS" /D "ORTE_IMPORTS" /D "_CRT_SECURE_NO_WARNINGS")
                SET(MPI_LIBRARY_DIRS "C:\\Program Files (x86)\\OpenMPI_v1.4.5-win32\\lib" "C:\\Program Files (x86)\\OpenMPI_v1.4.5-win32\\bin")
                SET(MPI_LIBRARIES libmpi_cxx.lib libmpi.lib libopen-pal.lib libopen-rte.lib advapi32.lib Ws2_32.lib shlwapi.lib )
                SET(ROOT_LIBS libCore libMatrix libMathCore libCint libNet libRIO libHist libMathMore libGenVector libPhysics)
                SET(MPI_INCLUDE_DIRS "C:\\Program Files (x86)\\OpenMPI_v1.4.5-win32\\include")
        ENDIF()
ELSEIF(${CMAKE_HOST_APPLE} )
        #Not supported yet
ENDIF()
SET(ROOTVERSION "5.32")
SET(PLUGIN_NAME "root-parallel-gui")

INCLUDE("${CMAKE_SOURCE_DIR}/cmake/RootMpiInstaller.cmake")


INCLUDE_DIRECTORIES( ${MPI_INCLUDE_DIRS} "${ROOTSYS}/include" "${ROOTSYS}/include/root" "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/src/" "${CMAKE_SOURCE_DIR}" )

LINK_DIRECTORIES( ${MPI_LIBRARY_DIRS} "${ROOTSYS}/lib/root" "${ROOTSYS}/lib")

#ROOTSYS should be loaded from environment variable
SET(CMAKE_INSTALL_PREFIX ${ROOTSYS})



SET(ROOT_PARALLEL_GUI_MPI True)

IF(ROOT_PARALLEL_GUI_MPI)
INCLUDE(${CMAKE_SOURCE_DIR}/cmake/RootParallelGuiMpi.cmake)
ENDIF()


SET(ROOT_PARALLEL_GUI_SRC
${CMAKE_SOURCE_DIR}/src/RootParallelGui.cpp
${CMAKE_SOURCE_DIR}/src/RootParallelGuiCint.cpp
)

SET(ROOT_PARALLEL_GUI_MOC_HDRS
${CMAKE_SOURCE_DIR}/include/RootParallelGui.h
)

SET(ROOT_PARALLEL_GUI_MPI_RCS
   ${CMAKE_SOURCE_DIR}/resources/RootParallelGui.qrc
)

SET(ROOT_PARALLEL_GUI_UIS
   ${CMAKE_SOURCE_DIR}/uis/RootParallelGui/RootParallelGui.ui
)


QT4_ADD_RESOURCES(ROOT_PARALLEL_GUI_RCS_SRCS 
                    ${ROOT_PARALLEL_GUI_MPI_RCS}
)


QT4_WRAP_UI(ROOT_PARALLEL_GUI_UIS_HDRS
   ${ROOT_PARALLEL_GUI_UIS}
)

QT4_WRAP_CPP( ROOT_PARALLEL_GUI_MOC_SRCS
   ${ROOT_PARALLEL_GUI_MOC_HDRS}
)

ADD_LIBRARY(ParallelGui SHARED
${ROOT_PARALLEL_GUI_SRC}
${ROOT_PARALLEL_GUI_RCS_SRCS}
${ROOT_PARALLEL_GUI_UIS_HDRS}
${ROOT_PARALLEL_GUI_MOC_SRCS}
)

ADD_LIBRARY(ParallelGuiStatic STATIC
${ROOT_PARALLEL_GUI_SRC}
${ROOT_PARALLEL_GUI_RCS_SRCS}
${ROOT_PARALLEL_GUI_UIS_HDRS}
${ROOT_PARALLEL_GUI_MOC_SRCS}
)

TARGET_LINK_LIBRARIES(ParallelGui ${ROOT_LIBS} ${QT_LIBRARIES} )
ADD_DEPENDENCIES(ParallelGui ParallelGuiMpi)

SET(ROOT_PARALLEL_GUI_MAIN_SRCS ${CMAKE_SOURCE_DIR}/src/RootParallelGuiMain.cpp)

ADD_EXECUTABLE(RootParallelGui
${ROOT_PARALLEL_GUI_MAIN_SRCS}
)

TARGET_LINK_LIBRARIES(RootParallelGui ParallelGuiStatic ParallelGuiMpiStatic ${ROOT_LIBS} ${QT_LIBRARIES})

MINSTALLTARGET("LIB" ParallelGui)
MINSTALLTARGET("BIN" RootParallelGui)
MINSTALLFILE("DOC" ${ROOTMPITUTORIALSDIR})

INCLUDE(CPack)
