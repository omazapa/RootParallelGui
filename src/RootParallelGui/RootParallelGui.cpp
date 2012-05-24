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
#include<RootParallelGui.h>

RootParallelGui::RootParallelGui(QMainWindow *parent): QMainWindow(parent)
{
   setupUi(this);
   Q_INIT_RESOURCE(RootParallelGui);
   connect(RootMpiLauncherPushButton, SIGNAL(clicked()), this, SLOT(showMpiLauncher()));
}

void RootParallelGui::showMpiLauncher()
{
   MpiLauncher = new RootParallelGuiMpiLauncher;
   ModulesTabWidget->addTab(MpiLauncher, "Mpi Launcher");
   connect(MpiLauncher, SIGNAL(closeme(RootParallelGuiMpiLauncher*)), this, SLOT(closeMpiLauncher(RootParallelGuiMpiLauncher*)));
   connect(MpiLauncher, SIGNAL(sendOutput(QString)), this, SLOT(prepend(QString)));
   ModulesTabWidget->setCurrentWidget(MpiLauncher);
}

void RootParallelGui::prepend(QString msg)
{
   msg += OutputTextEdit->toHtml();
   OutputTextEdit->setHtml(msg);;
   OutputTextEdit->setFocus();
}

void RootParallelGui::closeMpiLauncher(RootParallelGuiMpiLauncher* mpiLauncher)
{
   int index = ModulesTabWidget->indexOf(mpiLauncher);
   ModulesTabWidget->removeTab(index);
   delete mpiLauncher;
}

