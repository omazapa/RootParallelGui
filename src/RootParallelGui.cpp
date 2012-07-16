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
static void InitResources();

using namespace ROOT;
ClassImp(ParallelGui)

ParallelGui::ParallelGui(QMainWindow *parent): QMainWindow(parent)
{
   setupUi(this);
//    Q_INIT_RESOURCE(ParallelGui);
   InitResources();
   connect(MpiLauncherPushButton, SIGNAL(clicked()), this, SLOT(showMpiLauncher()));
}

void ParallelGui::showMpiLauncher()
{
   MpiLauncher = new ParallelGuiMpiLauncher;
   MpiLauncher->SetEmitOutput(true);
   ModulesTabWidget->addTab(MpiLauncher, "Mpi Launcher");
   connect(MpiLauncher, SIGNAL(closeme(ParallelGuiMpiLauncher*)), this, SLOT(closeMpiLauncher(ParallelGuiMpiLauncher*)));
   connect(MpiLauncher, SIGNAL(sendOutput(QString)), this, SLOT(prepend(QString)));
   ModulesTabWidget->setCurrentWidget(MpiLauncher);
}

void ParallelGui::prepend(QString msg)
{
   msg += OutputTextEdit->toHtml();
   OutputTextEdit->setHtml(msg);;
   OutputTextEdit->setFocus();
}

void ParallelGui::closeMpiLauncher(ParallelGuiMpiLauncher* mpiLauncher)
{
   int index = ModulesTabWidget->indexOf(mpiLauncher);
   ModulesTabWidget->removeTab(index);
   delete mpiLauncher;
}

static void  InitResources()
{
   Q_INIT_RESOURCE(RootParallelGui);
}
