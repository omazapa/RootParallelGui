#/***************************************************************************
#*   Copyright (C) by Omar Andres Zapata Mesa                               *
#*   email:andresete.chaos@gmail.com                                        *
#*   Orbital Mechanics Group                                                *
#*   Grupo de Fenomenologia de Interacciones Fundamentales                  *
#*   Universidad de Antioquia At Medellin - Colombia                        *
#*                                                                          *
#***************************************************************************/


####################
# RootParallelGui  #
####################
IF(${CMAKE_HOST_SYSTEM} MATCHES "Linux")
     SET(CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS},libqt4-core  (>= 4.7),libqt4-gui (>= 4.7),openmpi-bin")
ENDIF()

INCLUDE_DIRECTORIES ( ${CMAKE_SOURCE_DIR}/include ${CMAKE_BINARY_DIR} ${QT_INCLUDES} )

SET(X_ROOT_PARALLEL_GUI_MOC_HDRS
   ${CMAKE_SOURCE_DIR}/include/RootParallelGui.h
   ${CMAKE_SOURCE_DIR}/include/RootParallelGuiMpiLauncher.h
)

SET(X_ROOT_PARALLEL_GUI_SRCS
   ${CMAKE_SOURCE_DIR}/src/RootParallelGui/RootParallelGui.cpp
   ${CMAKE_SOURCE_DIR}/src/RootParallelGui/RootParallelGuiMpiLauncher.cpp
)

SET(X_ROOT_PARALLEL_GUI_UIS
   ${CMAKE_SOURCE_DIR}/uis/RootParallelGui/RootParallelGui.ui
   ${CMAKE_SOURCE_DIR}/uis/RootParallelGui/RootParallelGuiMpiLauncher.ui
)

SET(X_ROOT_PARALLEL_GUI_RCS
   ${CMAKE_SOURCE_DIR}/resources/RootParallelGui.qrc
)

SET(X_ROOT_PARALLEL_GUI_MAIN_SRCS ${CMAKE_SOURCE_DIR}/src/RootParallelGui/RootParallelGuiMain.cpp)



QT4_ADD_RESOURCES( X_ROOT_PARALLEL_GUI_RCS_SRCS 
		    ${X_ROOT_PARALLEL_GUI_RCS}
)


QT4_WRAP_UI( X_ROOT_PARALLEL_GUI_UIS_HDRS
   ${X_ROOT_PARALLEL_GUI_UIS}
)

QT4_WRAP_CPP( X_ROOT_PARALLEL_GUI_MOC_SRCS
   ${X_ROOT_PARALLEL_GUI_MOC_HDRS}
)

ADD_EXECUTABLE(RootParallelGui 
${X_ROOT_PARALLEL_GUI_UIS_HDRS}
${X_ROOT_PARALLEL_GUI_MOC_SRCS}
${X_ROOT_PARALLEL_GUI_SRCS}
${X_ROOT_PARALLEL_GUI_RCS_SRCS}
${X_ROOT_PARALLEL_GUI_MAIN_SRCS}
)
TARGET_LINK_LIBRARIES(RootParallelGui QtCore QtGui )

MINSTALLTARGET("BIN" RootParallelGui)
