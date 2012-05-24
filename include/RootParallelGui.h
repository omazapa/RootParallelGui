/*************************************************************************
* Copyright (C) 2012,  Gfif Developers                                   *
* Grupo de Fenomenologia de Interacciones Fundamentales                  *
* http://gfif.udea.edu.co                                                *
* División de ciencias de la computación Gfifdev                         *
* http://gfifdev.udea.edu.co                                             *
* Class Created and Maintained By Omar Andres Zapata Mesa                *
* All rights reserved.                                                   *
*                                                                        *
*                                                                        *
* For the list of contributors see $ROOTSYS/README/CREDITS.              *
*************************************************************************/

#ifndef ROOT_Parallel_Gui
#define ROOT_Parallel_Gui
#include<ui_RootParallelGui.h>

#include<RootParallelGuiMpiLauncher.h>

class RootParallelGui: public QMainWindow, Ui::RootParallelGui {
   Q_OBJECT
public:
   RootParallelGui(QMainWindow *parent = 0);
protected slots:
   void showMpiLauncher();
   void closeMpiLauncher(RootParallelGuiMpiLauncher* mpiLauncher);
   void prepend(QString msg);
private:
   RootParallelGuiMpiLauncher *MpiLauncher;
};

#endif
