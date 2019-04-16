#include "projector.h"

Projector::Projector(QString _ip, quint16 _port, pjProtocol _comMode, QObject *parent) :
    QObject(parent)
{
    ipAddr = _ip;
    portNum = _port;
    comMode = _comMode;

    TcpSocket = new QTcpSocket(this);

    connect(TcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(TcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    // timer to handle connexions timeout
    timer = new QTimer(this);
    timer->setInterval(20000); // 12 seconds
    connect(timer, SIGNAL(timeout()), this, SLOT(keepAlive()));
}

Projector::~Projector()
{
    TcpSocket->abort();
    delete timer;
    delete TcpSocket;
}

// COMMAND FUCNTIONS ---------------

// EPSON COMMANDS
//
// open ESC/VP.net session command
// 0x4553432F56502E6E6574100300000000
//
// power OFF : PWR OFF
// 0x505752204f46460D
//
// power ON : PWR ON
// 0x505752204F4E0D
//
// Shutter : KEY 3E
// 0x4B45592033450D
//
// ESC (Key mode) : KEY 05
// 0x4b45592030350D
//
// ESC (IR mode) : KEY 3D
// 0x4b45592033440D
//
// page UP : KEY 68
// 0x4b45592036380D
//
// page Down : KEY 69
// 0x4b45592036390D
//
// Pattern (Key mode) : KEY 1A
// 0x4b45592031410D
//
// Pattern (IR mode) : KEY 4B
// 0x4b45592034420D
//
// A/V Mute ON : MUTE ON
// 0x4d555445204f4e0D
//
// A/V Mute OFF : MUTE OFF
// 0x4d555445204f46464D
//
// ENTER : KEY 16
// 0x4b45592031360D


void Projector::patternOn()
{
    //pattern on command : "KEY 4B"
    QString command = "4b45592034420D";

    //convert string command to hexadecimal
    QByteArray cmd = QByteArray::fromHex(command.toUtf8());

    sendCommand(cmd);
}

void Projector::patternOff()
{
    //pattern off command : "ESC"
    QString command = "4b45592030350D";

    //convert string command to hexadecimal
    QByteArray cmd = QByteArray::fromHex(command.toUtf8());

    sendCommand(cmd);
}

void Projector::shutterOn()
{
    //Shutter ON command : "MUTE ON"
    QString command = "4d555445204f4e0D";

    //convert string command to hexadecimal
    QByteArray cmd = QByteArray::fromHex(command.toUtf8());

    sendCommand(cmd);
}

void Projector::shutterOff()
{
    //Shutter OFF command : "MUTE OFF"
    QString command = "4d555445204f46464D";

    //convert string command to hexadecimal
    QByteArray cmd = QByteArray::fromHex(command.toUtf8());

    sendCommand(cmd);
}

void Projector::powerOn()
{
    //Power ON command : "PWR ON"
    QString command = "505752204F4E0D";

    //convert string command to hexadecimal
    QByteArray cmd = QByteArray::fromHex(command.toUtf8());

    sendCommand(cmd);
}

void Projector::powerOff()
{
    //Power OFF command : "PWR OFF"
    QString command = "505752204f46460D";

    //convert string command to hexadecimal
    QByteArray cmd = QByteArray::fromHex(command.toUtf8());

    sendCommand(cmd);
}

void Projector::pgUpKey()
{
    //Page Up command : KEY 68
    QString command = "4b45592036380D";

    //convert string command to hexadecimal
    QByteArray cmd = QByteArray::fromHex(command.toUtf8());

    sendCommand(cmd);
}

void Projector::pgDownKey()
{
    //Page Down command : KEY 69
    QString command = "4b45592036390D";

    //convert string command to hexadecimal
    QByteArray cmd = QByteArray::fromHex(command.toUtf8());

    sendCommand(cmd);
}


// Socket Method --------------------------------

QAbstractSocket::SocketState Projector::socketState()
{
    return TcpSocket->state();
}

bool Projector::sendCommand(QByteArray command)
{
    if (TcpSocket->state() != QAbstractSocket::ConnectedState)
    {
        qWarning() << "Can't send command : not connected !";
    }
    else
    {
        if(TcpSocket->write(command))
        {
            timer->start();
            return true;
        }
        qWarning() << "Problem sending command !";
        return false;
    }
    return false;
}

bool Projector::connection(QString &ip, quint16 port)
{
    this->TcpSocket->connectToHost(ip, port);

    if(!TcpSocket->waitForConnected(100))
    {
        qWarning() << "Can't connect to : " << ip << " on port " << port;
        return false;
    }

    //open ESC/VP.net session
    QString sessionCommand = "4553432F56502E6E6574100300000000";
    QByteArray cmd = QByteArray::fromHex(sessionCommand.toUtf8());
    sendCommand(cmd);

    return true;
}

void Projector::closeConnection()
{
    TcpSocket->abort();
}

// SLOTS -------------------------------

void Projector::connected()
{
    if(TcpSocket->waitForReadyRead((100)))
    {
        while(TcpSocket->bytesAvailable() > 0) //purge socket buffer
        {
            TcpSocket->readLine();
        }
    }
    timer->start();
    emit sigSocketConnected();
}

void Projector::disconnected()
{
    timer->stop();
    emit sigSocketDisconnected();
}

void Projector::keepAlive()
{
    QString command = QString("00"); // send NULL as epson need this charater as keepalive
    QByteArray cmd;
    cmd.append(QByteArray::fromHex(command.toUtf8()));
    sendCommand(cmd);

    bool connected = (TcpSocket->state() == QAbstractSocket::ConnectedState);
    if(!connected) {
        emit sigSocketDisconnected();
        timer->stop();
    }
    else {
        timer->start();
    }
}
