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

RootParallelGuiMpiLauncher::RootParallelGuiMpiLauncher(QWidget *parent): QWidget(parent)
{
   setupUi(this);
   Q_INIT_RESOURCE(RootParallelGui);
   connect(LaunchPushButton, SIGNAL(clicked()), this, SLOT(launch()));
   connect(StopPushButton, SIGNAL(clicked()), this, SLOT(stop()));
   connect(ClosePushButton, SIGNAL(clicked()), this, SLOT(close()));
   connect(GetExecutableToolButton, SIGNAL(clicked()), this, SLOT(getExecutable()));

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
}

RootParallelGuiMpiLauncher::~RootParallelGuiMpiLauncher()
{
   delete futureRunner;
}

void RootParallelGuiMpiLauncher::getPath()
{
   PathLineEdit->setText(QFileDialog::getExistingDirectory(this));
}

void RootParallelGuiMpiLauncher::getPrefix()
{
   PrefixLineEdit->setText(QFileDialog::getExistingDirectory(this));
}

void RootParallelGuiMpiLauncher::getTmpDir()
{
   TmpdirLineEdit->setText(QFileDialog::getExistingDirectory(this));
}

void RootParallelGuiMpiLauncher::getWdir()
{
   WdirLineEdit->setText(QFileDialog::getExistingDirectory(this));
}

void RootParallelGuiMpiLauncher::getPreloadFiles()
{
   QStringList files = QFileDialog::getOpenFileNames(this);
   PreloadFilesListWidget->addItems(files);
}
void RootParallelGuiMpiLauncher::removePreloadFiles()
{
   if (PreloadFilesListWidget->currentItem() != NULL) {
      PreloadFilesListWidget->removeItemWidget(PreloadFilesListWidget->takeItem(PreloadFilesListWidget->currentRow()));
   }
}

void RootParallelGuiMpiLauncher::getPreloadFilesDestDir()
{
   PreloadFilesDestDirLineEdit->setText(QFileDialog::getExistingDirectory(this));
}

void RootParallelGuiMpiLauncher::addEnvironmentVariable()
{
   EnvironmentVariablesTableWidget->insertRow(0);
}

void RootParallelGuiMpiLauncher::removeEnvironmentVariable()
{
   EnvironmentVariablesTableWidget->removeRow(EnvironmentVariablesTableWidget->currentRow());
}


void RootParallelGuiMpiLauncher::launch()
{
   if (ExecutableLineEdit->text().isEmpty()) {
      QMessageBox::critical(this, tr("Error"), tr("Select executable to run first"));
      return;
   }
   args.clear();
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
      }
   }

//
//     if(HeteroCheckBox->isChecked()){
//       args<<"--hetero";
//     }

   if (VerboseQuietComboBox->currentIndex() == 0) {
      args << "-v";
   } else {
      args << "-q";
   }

   #warning "Added options to disable Openib options in mpirun --mca btl ^openib "
   args<<"--mca"<<"btl"<<"^openib";

   if(MacroBinaryComboBox->currentIndex()==0)
   {
     args <<"root"<<"-q"<<"-l"<<"-x";
   }
   args << ExecutableLineEdit->text();

   futureRunner->setFuture(QtConcurrent::run<void>(this, &RootParallelGuiMpiLauncher::runProcess));
}


void RootParallelGuiMpiLauncher::runProcess()
{
   process = new QProcess(futureRunner);
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

void RootParallelGuiMpiLauncher::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
   timer->stop();
   StopPushButton->setEnabled(false);
   LaunchPushButton->setEnabled(true);
}

void RootParallelGuiMpiLauncher::started()
{
   TimeDoubleSpinBox->setValue(0);
   timer->start(1);
   StopPushButton->setEnabled(true);
   LaunchPushButton->setEnabled(false);
}

void RootParallelGuiMpiLauncher::updateTime()
{
   double currentTime = TimeDoubleSpinBox->value() + 0.001;
   TimeDoubleSpinBox->setValue(currentTime);
}

void RootParallelGuiMpiLauncher::readStandardError()
{
   QByteArray Stderr = process->readAllStandardError();
   ParseOutput(Stderr);
   Stderr.trimmed();
   if (!Stderr.isEmpty()) {
      QString ErrorMsg = "<br><span style=\"font-weight:600; color:#ff0000;\">Error:</span><br>";
      ErrorMsg += Stderr.constData();
      ErrorMsg.replace("\n", "<br>");
      emit  sendOutput(ErrorMsg);
   }
}

void RootParallelGuiMpiLauncher::readStandardOutput()
{
   QByteArray Stdout = process->readAllStandardOutput();
   ParseOutput(Stdout);
   Stdout.trimmed();
   if (!Stdout.isEmpty()) {
      QString Msg = "<br><span style=\"font-weight:600; color:#0000ff;\">Output:</span><br>";
      Msg += Stdout.constData();
      Msg.replace("\n", "<br>");
      emit  sendOutput(Msg);
   }
}


void RootParallelGuiMpiLauncher::stop()
{
   timer->stop();
   process->kill();
   futureRunner->cancel();
//     delete process;
}


void RootParallelGuiMpiLauncher::close()
{
   //if some process is running finish process before close this widget
   if (process != NULL) {
      process->kill();
      delete process;
   }
   emit closeme(this);
}

void RootParallelGuiMpiLauncher::getExecutable()
{
   QString File = QFileDialog::getOpenFileName(this, tr("Open Executable"));
   if (!File.isEmpty()) ExecutableLineEdit->setText(File);
}

void RootParallelGuiMpiLauncher::ParseOutput(QByteArray &output)
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

