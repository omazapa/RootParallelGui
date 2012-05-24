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
#include<ui_RootParallelGuiMpiLauncher.h>
#include<QFutureWatcher>
#include<QProcess>
#include<QTimer>

class RootParallelGuiMpiLauncher: public QWidget, Ui::RootParallelGuiMpiLauncher {
   Q_OBJECT
public:
   RootParallelGuiMpiLauncher(QWidget *parent = 0);
   ~RootParallelGuiMpiLauncher();
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
protected:
   void runProcess();
   void ParseOutput(QByteArray &output);
private:
   QFutureWatcher<void> *futureRunner;
   QProcess *process;
   QString RootMpiPath;
   QTimer *timer;
   Q_PID pid;
   QStringList args;
signals:
   void closeme(RootParallelGuiMpiLauncher*);
   void sendOutput(QString);

};

#endif
