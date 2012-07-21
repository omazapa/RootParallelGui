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
#include<RootParallelGuiMpiLauncher.h>
#include<QMessageBox>
#include<QFileDialog>
#include<QtCore>
#include <Rtypes.h>
#include<TROOT.h>
#include<iostream>
#include<QFileInfo>
#include<TTabCom.h>

static void InitResources();

using namespace ROOT;
ClassImp(ParallelGuiMpiLauncher)
//tmp utility function
bool isWritable(QString file)
{
   QFileInfo *fileInfo = new QFileInfo(file);
   if (!fileInfo->isWritable()) {
      delete fileInfo;
      return false;
   }
   delete fileInfo;
   return true;
}

ParallelGuiMpiLauncher::ParallelGuiMpiLauncher(QWidget *parent): QWidget(parent)
{
   setupUi(this);
   InitResources();
   fMacro = NULL;
   fSessionMenu = new QMenu("Session");
   fSessionSave = new QAction("Save", this);
   fSessionLoad = new QAction("Load", this);
   fHighLihgter = new ROOT::ParallelGuiMacroHighlighter(MacroTextEdit->document());
   SessionToolButton->setMenu(fSessionMenu);
   SessionToolButton->setPopupMode(QToolButton::MenuButtonPopup);
   fSessionMenu->addAction(fSessionSave);
   fSessionMenu->addAction(fSessionLoad);
   connect(LaunchPushButton, SIGNAL(clicked()), this, SLOT(launch()));
   connect(StopPushButton, SIGNAL(clicked()), this, SLOT(stop()));
   connect(ClosePushButton, SIGNAL(clicked()), this, SLOT(close()));
   connect(GetExecutableToolButton, SIGNAL(clicked()), this, SLOT(getExecutable()));
   connect(GetCartoFileToolButton, SIGNAL(clicked()), this, SLOT(getCartoFile()));

   timer = new QTimer(this);
   connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));

   /*********************/
   /*Parallel Execution */
   /*********************/
   futureRunner = new QFutureWatcher<void>();

   MpiRunPath = "mpirun";
   StopPushButton->setEnabled(false);
   process = NULL;
   /********************************************
   *  To manage files and runtime environment *
   ********************************************/
   connect(PathToolButton, SIGNAL(clicked()), this, SLOT(getPath()));
   connect(PrefixToolButton, SIGNAL(clicked()), this, SLOT(getPrefix()));
   connect(TmpdirToolButton, SIGNAL(clicked()), this, SLOT(getTmpDir()));
   connect(WdirToolButton, SIGNAL(clicked()), this, SLOT(getWdir()));
   connect(AddPreloadFileToolButton, SIGNAL(clicked()), this, SLOT(getPreloadFiles()));
   connect(RemovePreloadFileToolButton, SIGNAL(clicked()), this, SLOT(removePreloadFiles()));
   connect(PreloadFilesDestDirToolButton, SIGNAL(clicked()), this, SLOT(getPreloadFilesDestDir()));
   connect(AddEnvironmentVariableToolButton, SIGNAL(clicked()), this, SLOT(addEnvironmentVariable()));
   connect(RemoveEnvironmentVariableToolButton, SIGNAL(clicked()), this, SLOT(removeEnvironmentVariable()));
   bEmitOuput = false;

   connect(SaveToolButton, SIGNAL(clicked()), this, SLOT(saveMacro()));
   connect(MacroBinaryComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setMacroBinaryMode(int)));

   /***********************************************************
    *To specify which hosts (nodes) of the cluster to run on: *
    ***********************************************************/
   connect(AddNodeToolButton, SIGNAL(clicked()), this, SLOT(addNode()));
   connect(MarchineFileToolButton, SIGNAL(clicked()), this, SLOT(getMachineFile()));
   connect(RemoveNodeToolButton, SIGNAL(clicked()), this, SLOT(removeNode()));
   connect(NodesSourceComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(NodesSourceChanged(int)));
   NodesTableWidget->setEnabled(false);
   AddNodeToolButton->setEnabled(false);
   RemoveNodeToolButton->setEnabled(false);

#ifdef _WIN32
   RootPath = "C:\\root";
#else
   RootPath = "/usr/";
#endif

   env = QProcessEnvironment::systemEnvironment();
   env.insert("PATH", env.value("PATH") + QString(RootPath + "\\bin"));
}

ParallelGuiMpiLauncher::~ParallelGuiMpiLauncher()
{
   delete futureRunner;
   delete fSessionLoad;
   delete fSessionMenu;
   delete fSessionSave;
   delete fHighLihgter;
}

void ParallelGuiMpiLauncher::getMachineFile()
{
   QString sFilePath = QFileDialog::getOpenFileName(this);
   if (!sFilePath.isNull()) MachinesLineEdit->setText(sFilePath);
}

void ParallelGuiMpiLauncher::getCartoFile()
{
   QString sFilePath = QFileDialog::getOpenFileName(this);
   if (!sFilePath.isNull()) CartoFileLineEdit->setText(sFilePath);
}

void ParallelGuiMpiLauncher::NodesSourceChanged(int index)
{
   if (index == 0) {
      MachinesLineEdit->setEnabled(true);
      MarchineFileToolButton->setEnabled(true);
      NodesTableWidget->setEnabled(false);
      AddNodeToolButton->setEnabled(false);
      RemoveNodeToolButton->setEnabled(false);
   } else {
      MachinesLineEdit->setEnabled(false);
      MarchineFileToolButton->setEnabled(false);
      NodesTableWidget->setEnabled(true);
      AddNodeToolButton->setEnabled(true);
      RemoveNodeToolButton->setEnabled(true);
   }
}

void ParallelGuiMpiLauncher::getPath()
{
   PathLineEdit->setText(QFileDialog::getExistingDirectory(this));
}

void ParallelGuiMpiLauncher::getPrefix()
{
   PrefixLineEdit->setText(QFileDialog::getExistingDirectory(this));
}

void ParallelGuiMpiLauncher::getTmpDir()
{
   TmpdirLineEdit->setText(QFileDialog::getExistingDirectory(this));
}

void ParallelGuiMpiLauncher::getWdir()
{
   WdirLineEdit->setText(QFileDialog::getExistingDirectory(this));
}

void ParallelGuiMpiLauncher::getPreloadFiles()
{
   QStringList files = QFileDialog::getOpenFileNames(this);
   PreloadFilesListWidget->addItems(files);
}
void ParallelGuiMpiLauncher::removePreloadFiles()
{
   if (PreloadFilesListWidget->currentItem() != NULL) {
      PreloadFilesListWidget->removeItemWidget(PreloadFilesListWidget->takeItem(PreloadFilesListWidget->currentRow()));
   }
}

void ParallelGuiMpiLauncher::getPreloadFilesDestDir()
{
   PreloadFilesDestDirLineEdit->setText(QFileDialog::getExistingDirectory(this));
}

void ParallelGuiMpiLauncher::addEnvironmentVariable()
{
   EnvironmentVariablesTableWidget->insertRow(EnvironmentVariablesTableWidget->rowCount());
}

void ParallelGuiMpiLauncher::removeEnvironmentVariable()
{
   EnvironmentVariablesTableWidget->removeRow(EnvironmentVariablesTableWidget->currentRow());
}


void ParallelGuiMpiLauncher::launch()
{
   if (ExecutableLineEdit->text().isEmpty()) {
      QMessageBox::critical(this, tr("Error"), tr("Select executable to run first"));
      return;
   }
   args.clear();
   rootArgs.clear();
   /*******************************
    * Processes to launch options *
    * *****************************/
   args << QString("-np") << QString::number(NPSpinBox->value());
   if (NperSocketCheckBox->isChecked()) {
      args << QString("--npersocket") << QString::number(NperSocketSpinBox->value());
   }

   if (NperNodeCheckBox->isChecked()) {
      args << QString("--npernode") << QString::number(NperNodeSpinBox->value());
   }

   if (PerNodeCheckBox->isChecked()) {
      args << "--pernode";
   }

   /***************************
    * Process binding options *
    * *************************/
   if (ByCoreCheckBox->isChecked()) {
      args << "--bynode";
   }

   if (BySocketCheckBox->isChecked()) {
      args << "--bysocket";
   }

   if (CpuPerProcCheckBox->isChecked()) {
      args << QString("--cpus-per-proc") << QString::number(CpuPerProcSpinBox->value());
   }

   args << QString("--%1").arg(BindTypeComboBox->currentText());
//       args<<QString("");

   if (ReportBindingsCheckBox->isChecked()) {
      args << "--report-bindings";
   }

   /***************************
    * map processes to nodes  *
    * *************************/
   if (LoadBalanceCheckBox->isChecked()) {
      args << "--loadbalance";
   }

   if (NotLocalCheckBox->isChecked()) {
      args << "--nolocal";
   }

   if (NoOverSubscribeCheckBox->isChecked()) {
      args << "--nooversubscribe";
   }

   if (ByNodeCheckBox->isChecked()) {
      args << "--bynode";
   }

   /********************************************
    *  To manage files and runtime environment *
    ********************************************/

   if (PathCheckBox->isChecked()) {
      args << "--path" << PathLineEdit->text();
   }

   if (PrefixCheckBox->isChecked()) {
      args << "--prefix" << PrefixLineEdit->text();
   }

   if (PreloadBinaryCheckBox->isChecked()) {
      args << "--preload-binary";
   }

   if (TmpdirCheckBox->isChecked()) {
      args << "--tmpdir" << TmpdirLineEdit->text();
   }

   if (WdirCheckBox->isChecked()) {
      args << "-wdir" << WdirLineEdit->text();
   }

   if (PreloadFilesGroupBox->isChecked()) {
      if (PreloadFilesListWidget->count() > 0) {
         args << "--preload-files";
         QString Files;
         for (Int_t i = 0; i < PreloadFilesListWidget->count(); i++) {
            Files += PreloadFilesListWidget->takeItem(i)->text();
            if (i != PreloadFilesListWidget->count() - 1) {
               Files += ",";
            }
         }
         args << Files;
         if (PreloadFilesDestDirCheckBox->isChecked()) {
            args << "--preload-files-dest-dir";
            args << PreloadFilesDestDirCheckBox->text();
         }
      }
   }
   if (EnvironmentVariablesGroupBox->isChecked()) {
      Int_t rows = EnvironmentVariablesTableWidget->rowCount();
      for (Int_t i = 0; i < rows; i++) {
         QString fVariable = EnvironmentVariablesTableWidget->item(i, 0)->text();
         QString fValue = EnvironmentVariablesTableWidget->item(i, 1)->text();
         if (!fVariable.isEmpty()) {
            if (!fValue.isEmpty())  args << "-x" << fVariable + "=" + fValue;
            else args << "-x" << fVariable;
         }
      }
   }
   /***********************************************************
    *To specify which hosts (nodes) of the cluster to run on: *
    ***********************************************************/
   if (NodesGroupBox->isChecked()) {
      if (NodesSourceComboBox->currentIndex() == 0) {
         QString sMachineFile = MachinesLineEdit->text();
         if (!sMachineFile.isEmpty()) {
            args << "-machinefile" << sMachineFile;
         } else {
            QMessageBox::critical(this, "Error in tab Nodes", "Enabled option machinefile, but file not specified.");
            return;
         }
      } else {
         int rows = NodesTableWidget->rowCount();
         if (rows > 0) args << "-H";
         QString sNodes;
         for (int i = 0; i < rows; i++) {
            QString sNode = NodesTableWidget->item(i, 0)->text();
            int     iSlots = static_cast<QSpinBox*>(NodesTableWidget->cellWidget(i, 1))->value();
            if (!sNode.isEmpty()) {
               for (int j = 0; j < iSlots; j++) {
                  sNodes += sNode;
                  if (j < (iSlots - 1)) sNodes += ",";
               }
            }
         }
         args << sNodes;
      }
   }
   /**************************************
    *Basic ROOT Options (Just for Macros)*
    **************************************/
   if (MacroBinaryComboBox->currentIndex() == 0) {
      if (bCheckBox->isChecked()) {
         rootArgs << "-b";
      }

      if (nCheckBox->isChecked()) {
         rootArgs << "-n";
      }

      if (qCheckBox->isChecked()) {
         rootArgs << "-q";
      }

      if (lCheckBox->isChecked()) {
         rootArgs << "-l";
      }

      if (xCheckBox->isChecked()) {
         rootArgs << "-x";
      }

      if (memstatCheckBox->isChecked()) {
         rootArgs << "-memstat";
      }
   }

   /***************************
    *Others/Debug options     *
    ***************************/
   if (HeteroCheckBox->isChecked()) {
      args << "--hetero";
   }

   if (LeaveSessionAttachedCheckBox->isChecked()) {
      args << "-leave-session-attached";
   }
   if (WaitForServerCheckBox->isChecked()) {
      args << "-wait-for-server";
   }
   if (SeverWaitTimeCheckBox->isChecked()) {
      args << "-server-wait-time" << QVariant::fromValue<int>(SeverWaitTimeSpinBox->value()).toString();
   }
   if (OMpiServerCheckBox->isChecked()) {
      if (!OMpiServerLineEdit->text().isEmpty()) {
         args << "-ompi-server" << OMpiServerLineEdit->text();
      } else {
         QMessageBox::critical(this, "Error in tab Others/Debug", "Enabled option ompi-server, but server not specified.");
         return;
      }
   }
   if (CartoFileCheckBox->isChecked()) {
      if (!CartoFileLineEdit->text().isEmpty()) {
         args << "--cartofile" << CartoFileLineEdit->text();
      } else {
         QMessageBox::critical(this, "Error in tab Others/Debug", "Enabled option cartofile, but file not specified.");
         return;
      }
   }
   if (AbortedCheckBox->isChecked()) {
      args << "-aborted" << QVariant::fromValue<int>(AbortedSpinBox->value()).toString();
   }

   if (DebugCheckBox->isChecked()) {
      args << "-debug";
   }
   if (DebugLevelCheckBox->isChecked()) {
      args << "--debug-devel";
   }
   if (DebugDaemonsCheckBox->isChecked()) {
      args << "--debug-daemons";
   }
   if (DebugDaemosFileCheckBox->isChecked()) {
      args << "--debug-daemons-file";
   }
   if (LaunchAgentCheckBox->isChecked()) {
      args << "-launch-agent";
   }
   if (NoPrefixCheckBox->isChecked()) {
      args << "--noprefix";
   }

   if (VerboseQuietComboBox->currentIndex() == 0) {
      args << "-v";
   } else {
      args << "-q";
   }
#ifndef _WIN32
#warning "Added options to disable Openib options in mpirun --mca btl ^openib "
   args << "--mca" << "btl" << "^openib";
#endif
   if (MacroBinaryComboBox->currentIndex() == 0) {
#ifdef _WIN32
      args << "root.exe" << rootArgs;
#else
      args << "root" << rootArgs;
#endif
   }
   args << ExecutableLineEdit->text();

   futureRunner->setFuture(QtConcurrent::run<void>(this, &ParallelGuiMpiLauncher::runProcess));
}


void ParallelGuiMpiLauncher::runProcess()
{
   process = new QProcess(futureRunner);
   process->setProcessEnvironment(env);
   qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");
   connect(process, SIGNAL(readyReadStandardError()), this, SLOT(readStandardError()));
   connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(readStandardOutput()));
   connect(process, SIGNAL(started()), this, SLOT(started()));
   connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finished(int, QProcess::ExitStatus)));
   qDebug() << args.join(" ");
   process->start(MpiRunPath, args);
   if (!process->waitForStarted()) {
      emit sendOutput(tr("Error Starting") + ExecutableLineEdit->text() + "<br>Msg:" + process->errorString().replace("\n", "<br>"));
      return;
   }
   pid = process->pid();
   PidLcdNumber->display((int)pid);
   if (!process->waitForFinished(-1)) {
      emit sendOutput(tr("Error finishing") + ExecutableLineEdit->text() + "<br>Msg:" + process->errorString().replace("\n", "<br>"));
      return;
   }

}

void ParallelGuiMpiLauncher::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
   timer->stop();
   StopPushButton->setEnabled(false);
   LaunchPushButton->setEnabled(true);
}

void ParallelGuiMpiLauncher::started()
{
   TimeDoubleSpinBox->setValue(0);
   timer->start(1);
   StopPushButton->setEnabled(true);
   LaunchPushButton->setEnabled(false);
}

void ParallelGuiMpiLauncher::updateTime()
{
   double currentTime = TimeDoubleSpinBox->value() + 0.001;
   TimeDoubleSpinBox->setValue(currentTime);
}

void ParallelGuiMpiLauncher::readStandardError()
{
   QByteArray Stderr = process->readAllStandardError();
   if (bEmitOuput) {
      ParseOutput(Stderr);
      Stderr.trimmed();
      if (!Stderr.isEmpty()) {
         QString ErrorMsg = "<br><span style=\"font-weight:600; color:#ff0000;\">Error:</span><br>";
         ErrorMsg += Stderr.constData();
         ErrorMsg.replace("\n", "<br>");
         emit  sendOutput(ErrorMsg);
      }

   } else {
      std::cerr << "\033[01;31m" << "Error: " << "\033[00m" << Stderr.data();
      std::cerr.flush();
   }
}

void ParallelGuiMpiLauncher::readStandardOutput()
{
   QByteArray Stdout = process->readAllStandardOutput();
   if (bEmitOuput) {
      ParseOutput(Stdout);
      Stdout.trimmed();
      if (!Stdout.isEmpty()) {
         QString Msg = "<br><span style=\"font-weight:600; color:#0000ff;\">Output:</span><br>";
         Msg += Stdout.constData();
         Msg.replace("\n", "<br>");
         emit  sendOutput(Msg);
      }
   } else {
      std::cout << "\033[01;34m" << "Output: " << "\033[00m" << Stdout.data();
      std::cout.flush();
   }
}


void ParallelGuiMpiLauncher::stop()
{
   timer->stop();
   process->kill();
   futureRunner->cancel();

}


void ParallelGuiMpiLauncher::close()
{
   //if some process is running finish process before close this widget
   if (process != NULL) {
      process->kill();
      delete process;
   }
   emit closeme(this);
}

void ParallelGuiMpiLauncher::getExecutable()
{
   QString File = QFileDialog::getOpenFileName(this, tr("Open Executable"));
   if (!File.isEmpty()) ExecutableLineEdit->setText(File);
   if (MacroBinaryComboBox->currentIndex() == 0) {
      if (fMacro) delete fMacro;
      fMacro = new QFile(File);
      if (isWritable(File)) {
         if (!fMacro->open(QIODevice::ReadWrite | QIODevice::Text))
            return;
         QByteArray text = fMacro->readAll();
         MacroTextEdit->setText(text.data());
         SaveToolButton->setEnabled(true);
      } else {
         if (!fMacro->open(QIODevice::ReadOnly | QIODevice::Text))
            return;
         QByteArray text = fMacro->readAll();
         MacroTextEdit->setText(text.data());
         MacroTextEdit->setReadOnly(true);
         SaveToolButton->setEnabled(false);
      }
   }
}

void ParallelGuiMpiLauncher::saveMacro()
{
   if (fMacro) {
      fMacro->resize(0);
      fMacro->write(MacroTextEdit->toPlainText().toStdString().c_str());
      fMacro->flush();
   }
}

void ParallelGuiMpiLauncher::setMacroBinaryMode(int mode)
{
   if (mode == 0) {
      tabWidget->setTabEnabled(5, true);
      RootOptionsGroupBox->setEnabled(true);
   } else {
      if (fMacro) {
         delete fMacro;
         fMacro = NULL;
      }
      tabWidget->setTabEnabled(5, false);
      RootOptionsGroupBox->setEnabled(false);
   }
}

void ParallelGuiMpiLauncher::ParseOutput(QByteArray &output)
{
   //parse from stdout/stderr
   //red
   output.replace("\033[01;31m", "<span style=\"color:#ff0000;\">");
   //green
   output.replace("\033[01;32m", "<span style=\"color:#00ff00;\">");
   //yellow
   output.replace("\033[01;33m", "<span style=\"color:#ffff00;\">");
   //blue
   output.replace("\033[01;34m", "<span style=\"color:#0000ff;\">");
   //purple
   output.replace("\033[01;35m", "<span style=\"color:#ff00ff;\">");
   //cyan
   output.replace("\033[01;36m", "<span style=\"color:#00ffff;\">");

   output.replace("\033[00m", "</span>");

}

void ParallelGuiMpiLauncher::addNode()
{
   NodesTableWidget->insertRow(NodesTableWidget->rowCount());
   QSpinBox *fNodes = new QSpinBox;
   fNodes->setMinimum(1);
   fNodes->setMaximum(255);
   NodesTableWidget->setCellWidget(NodesTableWidget->rowCount() - 1, 1, fNodes);
}

void ParallelGuiMpiLauncher::removeNode()
{
   NodesTableWidget->removeRow(NodesTableWidget->currentRow());
}

static void  InitResources()
{
   Q_INIT_RESOURCE(RootParallelGuiMpi);
}
