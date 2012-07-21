#/***************************************************************************
#*   Copyright (C) by Omar Andres Zapata Mesa                               *
#*   email:andresete.chaos@gmail.com                                        *
#*                                                                          *
#*   Grupo de Fenomenologia de Interacciones Fundamentales                  *
#*   Universidad de Antioquia At Medellin - Colombia                        *
#*                                                                          *
#***************************************************************************/


###############################
# RootParallelGuiMacroEditor  #
###############################

INCLUDE_DIRECTORIES ( ${CMAKE_SOURCE_DIR}/include ${CMAKE_BINARY_DIR} ${QT_INCLUDES} )

SET(ROOT_PARALLEL_GUI_MACRO_EDITOR_MOC_HDRS
   ${CMAKE_SOURCE_DIR}/include/RootParallelGuiMacroHighlighter.h
)

SET(ROOT_PARALLEL_GUI_MACRO_EDITOR_SRCS
   ${CMAKE_SOURCE_DIR}/src/RootParallelGuiMacroHighlighter.cpp
)

# SET(ROOT_PARALLEL_GUI_MACRO_EDITOR_UIS
#    ${CMAKE_SOURCE_DIR}/uis/RootParallelGuiMpi/RootParallelGuiMpiLauncher.ui
#    ${CMAKE_SOURCE_DIR}/uis/RootParallelGuiMpi/RootParallelGuiMpiConfig.ui
# )

# SET(ROOT_PARALLEL_GUI_MACRO_EDITOR_RCS
#    ${CMAKE_SOURCE_DIR}/resources/RootParallelGuiMpi.qrc
# )


# QT4_ADD_RESOURCES(ROOT_PARALLEL_GUI_MACRO_EDITOR_RCS_SRCS
#                     ${ROOT_PARALLEL_GUI_MACRO_EDITOR_RCS}
# )
# 
# QT4_WRAP_UI(ROOT_PARALLEL_GUI_MACRO_EDITOR_UIS_HDRS
#    ${ROOT_PARALLEL_GUI_MACRO_EDITOR_UIS}
# )

QT4_WRAP_CPP(ROOT_PARALLEL_GUI_MACRO_EDITOR_MOC_SRCS
   ${ROOT_PARALLEL_GUI_MACRO_EDITOR_MOC_HDRS}
)

ADD_LIBRARY(ParallelGuiMacroEditor SHARED 
${ROOT_PARALLEL_GUI_MACRO_EDITOR_UIS_HDRS}
${ROOT_PARALLEL_GUI_MACRO_EDITOR_MOC_SRCS}
${ROOT_PARALLEL_GUI_MACRO_EDITOR_SRCS}
${ROOT_PARALLEL_GUI_MACRO_EDITOR_RCS_SRCS}
)
ADD_LIBRARY(ParallelGuiMacroEditorStatic STATIC
${ROOT_PARALLEL_GUI_MACRO_EDITOR_UIS_HDRS}
${ROOT_PARALLEL_GUI_MACRO_EDITOR_MOC_SRCS}
${ROOT_PARALLEL_GUI_MACRO_EDITOR_SRCS}
${ROOT_PARALLEL_GUI_MACRO_EDITOR_RCS_SRCS}
)

TARGET_LINK_LIBRARIES(ParallelGuiMacroEditor ${ROOT_LIBS} ${QT_LIBRARIES})

MINSTALLTARGET("LIB" ParallelGuiMacroEditor)
