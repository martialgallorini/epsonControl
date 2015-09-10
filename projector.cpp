#include "projector.h"

Projector::Projector(QString _ip, qint16 _port, pjProtocol _comMode, QObject *parent) :
    QObject(parent)
{
    ipAddr = _ip;
    portNum = _port;
    comMode = _comMode;

    // Physical inputs with associated commands
    pjInput["DVI"] = "%1INPT 31\r";
    pjInput["SDI"] = "%1INPT 33\r";
    pjInput["HDMI"] = "%1INPT 32\r";
    pjInput["RGB1"] = "%1INPT 11\r";
    pjInput["RGB2"] = "%1INPT 12\r";
    pjInput["VIDEO"] = "%1INPT 21\r";
    pjInput["S-VIDEO"] = "%1INPT 22\r";

    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    // timer to handle connexions timeout
    timer = new QTimer(this);
    timer->setInterval(20000); // 12 seconds
    connect(timer, SIGNAL(timeout()), this, SLOT(keepAlive()));
}

Projector::~Projector()
{
    socket->abort();
    delete timer;
    delete socket;
}

// COMMAND FUCNTIONS ---------------

void Projector::selectInput(const QString input)
{
    sendCommand(pjInput[input]);
}

void Projector::shutterOn()
{
    sendCommand("%1AVMT 31\r");
}

void Projector::shutterOff()
{
    sendCommand("%1AVMT 30\r");
}

void Projector::powerOn()
{
    sendCommand("%1POWR 1\r");
}

void Projector::powerOff()
{
    sendCommand("%1POWR 0\r");
}

// Socket Method --------------------------------

QAbstractSocket::SocketState Projector::socketState()
{
    return socket->state();
}

bool Projector::sendCommand(QByteArray command)
{
    if (socket->state() != QAbstractSocket::ConnectedState)
    {
        qWarning() << "Can't send command : not connected !";
    }
    else
    {
        if(socket->write(command))
        {
            timer->start();
            return true;
        }
        qWarning() << "Problem sending command !";
        return false;
    }
    return false;
}

QByteArray Projector::sendQuery(QByteArray query)
{
    if (socket->state() != QAbstractSocket::ConnectedState)
    {
        qWarning() << "Can't send command : not connected !";
        return QByteArray("ERR_NOT_CONNECTED");
    }
    else
    {
        socket->write(query);
        QByteArray resp;
        if(socket->waitForBytesWritten(2000)) {
            while(socket->bytesAvailable() > 0 || socket->waitForReadyRead(1000))
            {
                resp.append(socket->readLine());
            }
            timer->start();
            return resp;
        }
        qWarning() << "Waiting for data to read timed out. Nothing to read !";
        return QByteArray("ERR_RESPONSE_TIMEOUT");
    }
    return QByteArray("ERR_SEND_QUERY");
}

QByteArray Projector::parsePjlinkResponse(QByteArray msg)
{
    qint16 pos = msg.indexOf("=") + 1;
    return msg.remove(0, pos);
}

bool Projector::connection(QString &ip, qint16 port)
{
    this->socket->connectToHost(ip, port);

    if(!socket->waitForConnected(500))
    {
        qWarning() << "Can't connect to : " << ip << " on port " << port;
        return false;
    }
    qDebug() << "Connected to : " << ip << " on port " << port;
    return true;
}

void Projector::closeConnection()
{
    socket->abort();
}

// QUERY METHODS -----------------------

QByteArray Projector::getInput()
{
    if(sendCommand("%1INPT ?\r"))
    {
        if(socket->waitForReadyRead(1000))
        {
            return parsePjlinkResponse(socket->readAll());
        }
    }
    return QByteArray("ERR_GET_INPUT");
}

bool Projector::getShutterStatus()
{
    if(sendCommand("%1AVMT ?\r"))
    {
        if(socket->waitForReadyRead(1000))
        {
            return parsePjlinkResponse(socket->readAll()).toInt();
        }
    }
    return false;
}

bool Projector::getPowerStatus()
{
    if(sendCommand("%1POWR ?\r"))
    {
        if(socket->waitForReadyRead(1000))
        {
            return parsePjlinkResponse(socket->readAll()).toInt();
        }
    }
    return false;
}

// SLOTS -------------------------------

void Projector::connected()
{
    qDebug() << "Connected to : " << ipAddr << " on port " << portNum;
    if(socket->waitForReadyRead((1000)))
    {
        while(socket->bytesAvailable() > 0) //purge socket buffer
        {
            socket->readLine();
        }
    }
    timer->start();
    emit sigSocketConnected();
}

void Projector::disconnected()
{
    qDebug() << "disconnected() signal emitted !";
    timer->stop();
    emit sigSocketDisconnected();
}

void Projector::keepAlive()
{
    qDebug() << "keepAlive() signal emitted !" << endl;
    sendCommand("\r");
    timer->start();
}
