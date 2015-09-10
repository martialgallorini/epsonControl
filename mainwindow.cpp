#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "projector.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // populate sources list with sources available for use in a model/view combobox
    srcList << "DVI" << "SDI" << "HDMI" << "RGB1" << "RGB2" << "VIDEO" << "S-VIDEO";
    srcModel = new QStringListModel(this); // create a model
    srcModel->setStringList(srcList); // set the list to the model
    ui->selection_sources_box->setModel(srcModel); // populate combobox with the sources list

    //create a vertical layout for a set of projectors UI
    QVBoxLayout *pjLayout = new QVBoxLayout();
    pjLayout->setSpacing(0);
    pjLayout->setMargin(0);

    // for each projector
    for(int i = 0; i < 8; i++)
    {
        QString ip = "192.168.5." + QString::number(21+i);
        pjPanel.append(new pjWidget(i, ip, 4352, PJLINK)); // create new projector UI panel
        pjLayout->addWidget(pjPanel[i]); // add panel to the layout
        pjPanel[i]->setVideoInputs(srcList); // set projector video sources list
        pjPanel[i]->setProjectorName("VP #" + QString::number(i+1));
    }
    ui->pjCommands->setLayout(pjLayout); // add the layout to the central widget in mainwindow

    //setup Matrix
    kramer = new matrix("192.168.5.117", 5000);
    connect(kramer, SIGNAL(sigMatrixConnected()), this, SLOT(matrixConnected()));
    connect(kramer, SIGNAL(sigMatrixDisconnected()), this, SLOT(matrixDisconnected()));

    //Set matrix output button IDs
    ui->outputGroup->setId(ui->out1, 1);
    ui->outputGroup->setId(ui->out2, 2);
    ui->outputGroup->setId(ui->out3, 3);
    ui->outputGroup->setId(ui->out4, 4);
    ui->outputGroup->setId(ui->out5, 5);
    ui->outputGroup->setId(ui->out6, 6);
    ui->outputGroup->setId(ui->out7, 7);
    ui->outputGroup->setId(ui->out8, 8);
    ui->outputGroup->setId(ui->out9, 9);
    ui->outputGroup->setId(ui->out10, 10);
    ui->inputGroup->setId(ui->pattern, 17);
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

void MainWindow::on_selection_sources_box_activated(const QString &arg1)
{
    for(int i = 0; i < 8; i++)
    {
        if(pjPanel[i]->isSelected() && pjPanel[i]->socketState() == QAbstractSocket::ConnectedState)
        {
            pjPanel[i]->selectInput(arg1);
        }
    }
}

void MainWindow::matrixConnected() {
    ui->matrixGroup->setEnabled(true);
    ui->online_icon->setPixmap(QPixmap(":/icons/img/ok.png"));
}

void MainWindow::matrixDisconnected() {
    ui->matrixGroup->setEnabled(false);
    ui->online_icon->setPixmap(QPixmap(":/icons/img/ko.png"));
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


void MainWindow::on_resetio_clicked()
{
    for(int i = 1; i < PATCH_SIZE + 1; i++)
    {
        QByteArray cmd = "#VID " + QByteArray::number(i) + ">" + QByteArray::number(i) + "\r";
        kramer->sendCommand(cmd);
    }
    kramer->sendCommand("#VID 17>17\r");
    kramer->updatePatch();
}

void MainWindow::checkOuts(qint8 input) {
    QMap<qint8, qint8> patch = kramer->getPatch();
    QList<qint8> tiedOuts;
    tiedOuts = patch.values(input);
    for(int i = 1; i < PATCH_SIZE + 1; i++)
    {
        if(tiedOuts.contains(i))
        {
            ui->outputGroup->button(i)->setChecked(true);
        }
        else
        {
            ui->outputGroup->button(i)->setChecked(false);
        }
    }
}

void MainWindow::on_in1_clicked(bool checked)
{
    if(checked)
    {
        checkOuts(1);
    }
}

void MainWindow::on_in2_clicked(bool checked)
{
    if(checked)
    {
        checkOuts(2);
    }
}

void MainWindow::on_in3_clicked(bool checked)
{
    if(checked)
    {
        checkOuts(3);
    }
}

void MainWindow::on_in4_clicked(bool checked)
{
    if(checked)
    {
        checkOuts(4);
    }
}

void MainWindow::on_in5_clicked(bool checked)
{
    if(checked)
    {
        checkOuts(5);
    }
}

void MainWindow::on_in6_clicked(bool checked)
{
    if(checked)
    {
        checkOuts(6);
    }
}

void MainWindow::on_in7_clicked(bool checked)
{
    if(checked)
    {
        checkOuts(7);
    }
}

void MainWindow::on_in8_clicked(bool checked)
{
    if(checked)
    {
        checkOuts(8);
    }
}

void MainWindow::on_in9_clicked(bool checked)
{
    if(checked)
    {
        checkOuts(9);
    }
}

void MainWindow::on_in10_clicked(bool checked)
{
    if(checked)
    {
        checkOuts(10);
    }
}

void MainWindow::on_pattern_clicked(bool checked)
{
    if(checked)
    {
        checkOuts(17);
    }
}

void MainWindow::on_take_clicked()
{
    if(ui->inputGroup->checkedButton() != 0 && ui->outputGroup->checkedButton() != 0)        // if an input is selected
    {
        QByteArray command = "#VID ";
        bool isFirst = true;
        for(int i = 1; i < PATCH_SIZE + 1; i++)     // check for selected output
        {
            if(ui->outputGroup->button(i)->isChecked())
            {
                if(!isFirst)
                {
                    command.append(",");
                }
                if(ui->inputGroup->button(17)->isChecked())
                {
                    command.append("17>" + QString::number(i));
                }
                else
                {
                    QByteArray selectedInput;
                    selectedInput.append(ui->inputGroup->checkedButton()->text());
                    command.append(selectedInput + ">" + QString::number(i));
                }
                isFirst = false;
            }
        }
        command.append("\r");
        qDebug() << "commande : " << command << endl;
        kramer->sendCommand(command);
        kramer->updatePatch();
    }
    else
    {
        qDebug() << "Please select an input and an output to patch !" << endl;
    }
}

void MainWindow::on_actionConnect_Matrix_triggered()
{
    kramer->connection();
}

void MainWindow::on_actionDisconnect_Matrix_triggered()
{
    kramer->closeConnection();
}
