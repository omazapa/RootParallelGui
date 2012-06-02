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

#ifndef ROOT_Rtypes
#include "Rtypes.h"
#endif


#ifdef __CINT__
namespace Ui {
   class RootParallelGui;
}
class QMainWindow;
#else
#include<ui_RootParallelGui.h>
#endif
#include<RootParallelGuiMpiLauncher.h>
namespace ROOT {
   class ParallelGui: public QMainWindow, Ui::RootParallelGui {
#ifndef __CINT__
      Q_OBJECT
#endif
   private:
      ParallelGui(const ParallelGui &);
      ParallelGui& operator=(const ParallelGui &);
   public:
      ParallelGui(QMainWindow *parent = 0);
   public slots:
      void showMpiLauncher();
      void closeMpiLauncher(ParallelGuiMpiLauncher* mpiLauncher);
      void prepend(QString msg);
   private:
      ParallelGuiMpiLauncher *MpiLauncher;
      ClassDef(ParallelGui, 1) //
   };

}

#endif
