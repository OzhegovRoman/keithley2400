#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QtCharts>
#include "visa.h"

#define MAX_SCPI_LEN    255
#define DEFAULT_TMO     5000

using namespace QtCharts;

namespace Ui {
class MainDialog;
}

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();

private slots:
    void on_pbSave_clicked();
    void timerTick();

    void on_pbStart_clicked(bool checked);

private:
    Ui::MainDialog *ui;
    QChart *chart;
    QLineSeries *s1;
    QTimer *timer;
    ViSession visaSession;
    float miny,maxy;
    QElapsedTimer *elapsedTimer;
    bool inited;
    bool initVisa();
};

#endif // MAINDIALOG_H
