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

#ifndef ROOT_Parallel_Gui_Mpi_Config
#define ROOT_Parallel_Gui_Mpi_Config

#ifndef ROOT_Rtypes
#include "Rtypes.h"
#endif
#ifdef __CINT__
class QDialog;
class QByteArray;
class QString;
class QStringList;
class QSettings;
namespace Ui {
   class  RootParallelGuiMpiConfig;
}
#else
#include<ui_RootParallelGuiMpiConfig.h>
#include<QDialog>
#include<QSettings>
#endif

namespace ROOT {
   class ParallelGuiMpiConfig: public QDialog, Ui::RootParallelGuiMpiConfig {
#ifndef __CINT__
      Q_OBJECT
#endif
   private:
      ParallelGuiMpiConfig(const ParallelGuiMpiConfig &);
      ParallelGuiMpiConfig& operator=(const ParallelGuiMpiConfig &);

   public:
      ParallelGuiMpiConfig(QWidget *parent = 0);
      ~ParallelGuiMpiConfig();
   protected slots:
      void GetRootMpiPath();
      void Apply();
   private:
      QSettings *fConfigSettings;
      ClassDef(ParallelGuiMpiConfig, 0)
   };
}

#endif
