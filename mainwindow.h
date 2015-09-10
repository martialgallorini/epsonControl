#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>
#include "pjwidget.h"
#include "matrix.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QStringList srcList; // list of sources available for a projector
    QStringListModel *srcModel; // model of sources list
    void checkOuts(qint8 input);

private slots:

    void on_select_all_toggled(bool checked);
    void on_selection_connect_clicked();
    void on_selection_shutter_on_clicked();
    void on_selection_shutter_off_clicked();
    void on_selection_power_on_clicked();
    void on_selection_power_off_clicked();
    void on_selection_sources_box_activated(const QString &arg1);
    void on_resetio_clicked();
    void on_in1_clicked(bool checked);
    void on_in2_clicked(bool checked);
    void on_in3_clicked(bool checked);
    void on_in4_clicked(bool checked);
    void on_in5_clicked(bool checked);
    void on_in6_clicked(bool checked);
    void on_in7_clicked(bool checked);
    void on_in8_clicked(bool checked);
    void on_in9_clicked(bool checked);
    void on_in10_clicked(bool checked);
    void on_pattern_clicked(bool checked);
    void on_take_clicked();

    //Actions Slots
    void on_actionConnect_triggered();
    void on_actionDisconnect_triggered();

    void matrixConnected();
    void matrixDisconnected();

    void on_actionConnect_Matrix_triggered();
    void on_actionDisconnect_Matrix_triggered();


private:
    Ui::MainWindow *ui;
    QVector<pjWidget *> pjPanel; // set of projectors UI (custom widgets)
    matrix *kramer;
};

#endif // MAINWINDOW_H
