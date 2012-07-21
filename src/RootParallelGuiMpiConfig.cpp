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
#include<RootParallelGuiMpiConfig.h>
#include<QMessageBox>
#include<QFileDialog>
#include<QtCore>
#include <Rtypes.h>
#include<TROOT.h>
#include<iostream>
#include<QFileInfo>
#include<TTabCom.h>

using namespace ROOT;
ClassImp(ParallelGuiMpiConfig)
//tmp utility function
ParallelGuiMpiConfig::ParallelGuiMpiConfig(QWidget *parent): QDialog(parent)
{
   setupUi(this);
   connect(RootMpiPathToolButton, SIGNAL(clicked()), this, SLOT(GetRootMpiPath()));
   fConfigSettings = new QSettings("root", "ParallelGuiMpiConfig");
}

ParallelGuiMpiConfig::~ParallelGuiMpiConfig()
{

}

void ParallelGuiMpiConfig::GetRootMpiPath()
{
   QString sFilePath = QFileDialog::getOpenFileName(this);
   if (!sFilePath.isNull()) RootMpiPathLineEdit->setText(sFilePath);
}

void ParallelGuiMpiConfig::Apply()
{
   fConfigSettings->setValue("RootMpiCommandPath", RootMpiPathLineEdit->text());
   fConfigSettings->sync();
}

