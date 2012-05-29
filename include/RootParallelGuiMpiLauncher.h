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

#ifndef ROOT_Parallel_Gui_Mpi_Launcher
#define ROOT_Parallel_Gui_Mpi_Launcher

#ifndef ROOT_Rtypes
#include "Rtypes.h"
#endif
#ifdef __CINT__
class QWidget;
class QProcess;
class QByteArray;
class QString;
class QFutureWatcher<void>;
class QStringList;
class QTimer;
class QFile;
typedef Long64_t Q_PID;
namespace Ui {
   class  RootParallelGuiMpiLauncher;
}
enum QProcess::ExitStatus;
#else
#include<ui_RootParallelGuiMpiLauncher.h>
#include<QFutureWatcher>
#include<QProcess>
#include<QTimer>
#include<QFile>
#endif

namespace ROOT {
   class ParallelGuiMpiLauncher: public QWidget, Ui::RootParallelGuiMpiLauncher {
#ifndef __CINT__
      Q_OBJECT
#endif
   private:
      ParallelGuiMpiLauncher(const ParallelGuiMpiLauncher &);
      ParallelGuiMpiLauncher& operator=(const ParallelGuiMpiLauncher &);

   public:
      ParallelGuiMpiLauncher(QWidget *parent = 0);
      ~ParallelGuiMpiLauncher();
      inline void Show() {
         this->show();
      }
      inline void SetEmitOutput(Bool_t status = true) {
         bEmitOuput = status;
      }
   protected slots:
      void launch();
      void stop();
      void close();
      void getExecutable();
      void readStandardError();
      void readStandardOutput();
      void started();
      void finished(int exitCode, QProcess::ExitStatus exitStatus);
      void updateTime();
      void getPath();
      void getPrefix();
      void getTmpDir();
      void getWdir();
      void getPreloadFiles();
      void removePreloadFiles();
      void getPreloadFilesDestDir();
      void addEnvironmentVariable();
      void removeEnvironmentVariable();
      void saveMacro();
      //index 0 macro mode and 1 binary mode 
      void setMacroBinaryMode(int);
   protected:
      void runProcess();
      void ParseOutput(QByteArray &output);
   signals:
      void closeme(ParallelGuiMpiLauncher*);
      void sendOutput(QString);
   private:
      QFutureWatcher<void> *futureRunner;
      QProcess *process;
      QString MpiRunPath;
      QString RootPath;
      QTimer *timer;
      Q_PID pid;
      QStringList args;
      QStringList rootArgs;
      Bool_t bEmitOuput;
      QFile *fMacro;
      ClassDef(ParallelGuiMpiLauncher, 1)
   };
}

#endif
