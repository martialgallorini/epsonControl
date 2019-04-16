#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>
#include "pjwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void checkOuts(qint8 input);    

private slots:

    void on_select_all_toggled(bool checked);
    void on_selection_connect_clicked();
    void on_selection_shutter_on_clicked();
    void on_selection_shutter_off_clicked();
    void on_selection_power_on_clicked();
    void on_selection_power_off_clicked();
    void on_selection_pattern_on_clicked();
    void on_selection_pattern_off_clicked();
    void on_selection_patternNext_clicked();
    void on_selection_patternPrev_clicked();

    //Actions Slots
    void on_actionConnect_triggered();
    void on_actionDisconnect_triggered();

private:
    Ui::MainWindow *ui;
    QVector<pjWidget *> pjPanel; // set of projectors UI (custom widgets)
};

#endif // MAINWINDOW_H
