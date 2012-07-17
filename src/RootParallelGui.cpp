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
   InitResources();
   connect(MpiLauncherPushButton, SIGNAL(clicked()), this, SLOT(showMpiLauncher()));
   connect(actionConfiguration,SIGNAL(triggered()),this,SLOT(showConfiguration()));
   fMpiLauncher=NULL;
   fConfigDialog=NULL;
}

ParallelGui::~ParallelGui()
{
  if(fMpiLauncher) delete fMpiLauncher;
  if(fConfigDialog) delete fConfigDialog;
}
void ParallelGui::showMpiLauncher()
{
   fMpiLauncher = new ParallelGuiMpiLauncher();
   fMpiLauncher->SetEmitOutput(true);
   ModulesTabWidget->addTab(fMpiLauncher, "Mpi Launcher");
   connect(fMpiLauncher, SIGNAL(closeme(ParallelGuiMpiLauncher*)), this, SLOT(closeMpiLauncher(ParallelGuiMpiLauncher*)));
   connect(fMpiLauncher, SIGNAL(sendOutput(QString)), this, SLOT(prepend(QString)));
   ModulesTabWidget->setCurrentWidget(fMpiLauncher);
}

void ParallelGui::showConfiguration()
{
  fConfigDialog=new ParallelGuiConfig();
  fConfigDialog->exec();
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
