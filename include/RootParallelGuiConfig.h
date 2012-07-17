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

#ifndef ROOT_Parallel_Gui_Config
#define ROOT_Parallel_Gui_Config

#ifndef ROOT_Rtypes
#include "Rtypes.h"
#endif
#ifdef __CINT__
class QDialog;
class QByteArray;
class QString;
class QStringList;
namespace Ui {
   class  RootParallelGuiConfig;
}
namespace ROOT{
   class ParallelGuiMpiConfig; 
}
#else
#include<ui_RootParallelGuiConfig.h>
#include<QDialog>
#include<RootParallelGuiMpiConfig.h>
#endif

namespace ROOT {
   class ParallelGuiConfig: public QDialog, Ui::RootParallelGuiConfig {
#ifndef __CINT__
      Q_OBJECT
#endif
   private:
      ParallelGuiConfig(const ParallelGuiConfig &);
      ParallelGuiConfig& operator=(const ParallelGuiConfig &);
   public:
      ParallelGuiConfig(QWidget *parent = 0);
      ~ParallelGuiConfig();
   protected slots:
     void showRootMpiConfig();
   private:
     ParallelGuiMpiConfig *fMpiConfigWidget;
      ClassDef(ParallelGuiConfig, 0)
   };
}

#endif
