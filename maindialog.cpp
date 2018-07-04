#include "maindialog.h"
#include "ui_maindialog.h"

MainDialog::MainDialog(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::MainDialog),
   chart (new QChart()),
   s1(new QLineSeries()),
   timer(new QTimer(this)),
   elapsedTimer(new QElapsedTimer()),
   inited(false)
   {
   ui->setupUi(this);

   chart->addSeries(s1);
   chart->legend()->setVisible(false);
   chart->createDefaultAxes();

   ui->wChart->setChart(chart);

   timer->setSingleShot(true);

   connect(timer, SIGNAL(timeout()), SLOT(timerTick()));
   }

MainDialog::~MainDialog()
   {
   delete ui;
   }

void MainDialog::on_pbSave_clicked()
   {
   QString FileName = QFileDialog::getSaveFileName(this, tr("Save File..."), "Data.csv", tr("CSV files (*.csv)"));
   QFile data(FileName);
   if(data.open(QFile::WriteOnly |QFile::Truncate)) {
      QTextStream output(&data);
      output<<"Time(s);Current(A)\n";
      for (int i = 0; i < s1->count(); ++i){
         output << s1->at(i).x()
                << ';'
                << s1->at(i).y()
                <<'\n';
         }
      }
   data.close();

   }

void MainDialog::timerTick()
   {
   if (!inited){
      on_pbStart_clicked(false);
      return;
      }

   ViByte rdBuff[MAX_SCPI_LEN];

   QString read;
   switch (ui->comboBox->currentIndex()) {
      case 1:
         read = "READING = smua.measure.i()\nprint(READING)\n";
         break;
      default:
         read = ":READ?\n";
      }

   ViStatus status = viQueryf(visaSession, read.toLocal8Bit().data(), "%s", rdBuff);
   if (status>VI_SUCCESS){
      on_pbStart_clicked(false);
      return;
      }

   float tmpF = NAN;

   QString str = QString(QByteArray((const char*)rdBuff));
   QStringList sList = str.split(",");

   if (sList.length()>1)
      tmpF = sList[1].toFloat();
   else if (ui->comboBox->currentIndex() == 1)
      tmpF = str.toFloat();

   if (tmpF!=NAN)
      {
      if (miny ==-1) miny=maxy=tmpF;
      else{
         if (miny>tmpF) miny = tmpF;
         if (maxy<tmpF) maxy = tmpF;
         }

      float maxx = elapsedTimer->elapsed()/1000.0;
      chart->axisX()->setRange((maxx>30)? (maxx-30.0) : 0,elapsedTimer->elapsed()/1000.0);

      if (miny == maxy) chart->axisY()->setRange(miny-1,maxy+1);
      else chart->axisY()->setRange(miny,maxy);

      qobject_cast<QLineSeries*>(chart->series()[0])->append(elapsedTimer->elapsed()/1000.0,tmpF);
      }
   timer->start(10);

   }

bool MainDialog::initVisa()
   {
   inited = false;

   ViSession rmSession;

   ViStatus status = viOpenDefaultRM(&rmSession);
   if (status > VI_SUCCESS) {
      qDebug()<<"error at opening default RM";
      return false;
      }

   status = viOpen(rmSession, ui->leName->text().toLocal8Bit().data(), VI_NULL, VI_NULL, &visaSession);
   if (status > VI_SUCCESS) {
      qDebug()<<"error at connecting to instrument";
      return false;
      }

   /* set visa Format */
   status = viSetAttribute( visaSession, VI_ATTR_TMO_VALUE, DEFAULT_TMO);
   status = viSetAttribute( visaSession, VI_ATTR_SUPPRESS_END_EN, VI_FALSE);
   status = viSetAttribute( visaSession, VI_ATTR_SEND_END_EN, VI_FALSE);

   inited = true;
   return inited;
   }

void MainDialog::on_pbStart_clicked(bool checked)
   {
   ui->pbSave->setDisabled(checked);
   ui->leName->setDisabled(checked);
   if (checked){
      if (!initVisa()) {
         on_pbStart_clicked(false);
         return;
         }
      miny=maxy=-1;
      s1->clear();
      elapsedTimer->start();
      timer->start(10);
      }
   else{
      timer->stop();
      }

   }
