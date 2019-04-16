#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "projector.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //create a vertical layout for a set of projectors UI
    QVBoxLayout *pjLayout = new QVBoxLayout();
    pjLayout->setSpacing(0);
    pjLayout->setMargin(0);

    // for each projector
    for(int i = 0; i < 8; i++)
    {
        QString ip = "192.168.5." + QString::number(11+i);
        pjPanel.append(new pjWidget(i, ip, 3629, EPSON)); // create new projector UI panel
        pjLayout->addWidget(pjPanel[i]); // add panel to the layout
        pjPanel[i]->setProjectorName("VP #" + QString::number(i+1));
    }
    ui->pjCommands->setLayout(pjLayout); // add the layout to the central widget in mainwindow

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_selection_connect_clicked()
{
    for(int i = 0; i < 8; i++)
    {
        if(pjPanel[i]->isSelected())
        {
            pjPanel[i]->connection(pjPanel[i]->getIp(), pjPanel[i]->getPort());
        }
    }
}

void MainWindow::on_selection_shutter_on_clicked()
{
    for(int i = 0; i < 8; i++)
    {
        for(int i = 0; i < 8; i++)
        {
            if(pjPanel[i]->isSelected())
            {
                if(pjPanel[i]->socketState() == QAbstractSocket::ConnectedState)
                {
                    pjPanel[i]->shutterOn();
                }
                else
                {
                    ui->statusBar->showMessage("Warning : problem sending command to projector #" + QString::number(i) + " !", 5000);
                }
            }
        }
    }
}

void MainWindow::on_selection_shutter_off_clicked()
{
    for(int i = 0; i < 8; i++)
    {
        if(pjPanel[i]->isSelected())
        {
            if(pjPanel[i]->socketState() == QAbstractSocket::ConnectedState)
            {
                pjPanel[i]->shutterOff();
            }
            else
            {
                ui->statusBar->showMessage("Warning : problem sending command to projector #" + QString::number(i) + " !", 5000);
            }
        }
    }
}

void MainWindow::on_selection_power_on_clicked()
{
    for(int i = 0; i < 8; i++)
    {
        if(pjPanel[i]->isSelected())
        {
            if(pjPanel[i]->socketState() == QAbstractSocket::ConnectedState)
            {
                pjPanel[i]->powerOn();
            }
            else
            {
                ui->statusBar->showMessage("Warning : problem sending command to projector #" + QString::number(i) + " !", 5000);
            }
        }
    }
}

void MainWindow::on_selection_power_off_clicked()
{
    for(int i = 0; i < 8; i++)
    {
        if(pjPanel[i]->isSelected())
        {
            if(pjPanel[i]->socketState() == QAbstractSocket::ConnectedState)
            {
                pjPanel[i]->powerOff();
            }
            else
            {
                ui->statusBar->showMessage("Warning : problem sending command to projector #" + QString::number(i) + " !", 5000);
            }
        }
    }
}

void MainWindow::on_select_all_toggled(bool checked)
{
    for(int i = 0; i < 8; i++)
    {
        pjPanel[i]->selectProjector(checked);
    }
}

void MainWindow::on_selection_pattern_on_clicked()
{
    for(int i = 0; i < 8; i++)
    {
        if(pjPanel[i]->isSelected())
        {
            if(pjPanel[i]->socketState() == QAbstractSocket::ConnectedState)
            {
                pjPanel[i]->patternOn();
            }
            else
            {
                ui->statusBar->showMessage("Warning : problem sending command to projector #" + QString::number(i) + " !", 5000);
            }
        }
    }
}

void MainWindow::on_selection_pattern_off_clicked()
{
    for(int i = 0; i < 8; i++)
    {
        if(pjPanel[i]->isSelected())
        {
            if(pjPanel[i]->socketState() == QAbstractSocket::ConnectedState)
            {
                pjPanel[i]->patternOff();
            }
            else
            {
                ui->statusBar->showMessage("Warning : problem sending command to projector #" + QString::number(i) + " !", 5000);
            }
        }
    }
}

void MainWindow::on_selection_patternNext_clicked()
{
    for(int i = 0; i < 8; i++)
    {
        if(pjPanel[i]->isSelected())
        {
            if(pjPanel[i]->socketState() == QAbstractSocket::ConnectedState)
            {
                pjPanel[i]->patternNext();
            }
            else
            {
                ui->statusBar->showMessage("Warning : problem sending command to projector #" + QString::number(i) + " !", 5000);
            }
        }
    }
}

void MainWindow::on_selection_patternPrev_clicked()
{
    for(int i = 0; i < 8; i++)
    {
        if(pjPanel[i]->isSelected())
        {
            if(pjPanel[i]->socketState() == QAbstractSocket::ConnectedState)
            {
                pjPanel[i]->patternPrev();
            }
            else
            {
                ui->statusBar->showMessage("Warning : problem sending command to projector #" + QString::number(i) + " !", 5000);
            }
        }
    }
}

// ACTIONS SLOTS --------------------------

void MainWindow::on_actionConnect_triggered()
{
    for(int i = 0; i < 8; i++)
    {
        pjPanel[i]->connection(pjPanel[i]->getIp(), pjPanel[i]->getPort());
    }
}

void MainWindow::on_actionDisconnect_triggered()
{
    for(int i = 0; i < 8; i++)
    {
        pjPanel[i]->closeConnection();
    }
    ui->statusBar->showMessage("Disconnecting projectors... Done !", 5000);
}
