#include "matrix.h"

matrix::matrix(QString _ip, qint16 _port, QObject *parent) : QObject(parent)
{
    ipAddr = _ip;
    portNum = _port;

    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    // timer to handle connexions timeout
//    timer = new QTimer(this);
//    timer->setInterval(1000);
//    connect(timer, SIGNAL(timeout()), this, SLOT(keepAlive()));
}

matrix::~matrix()
{
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->abort();
    }
    delete socket;
//    delete timer;
}

// Socket Method --------------------------------

QAbstractSocket::SocketState matrix::socketState()
{
    return socket->state();
}

// ----------------------------------------------

bool matrix::sendCommand(QByteArray command)
{
    if (socket->state() != QAbstractSocket::ConnectedState)
    {
        qWarning() << "Can't send command : not connected !";
    }
    else
    {
        if(socket->write(command))
        {
            // purge socket
            while(socket->bytesAvailable() > 0 || socket->waitForReadyRead(1000))
            {
                socket->readLine();
            }
            return true;
        }
        qWarning() << "Problem sending command !";
    }
    return false;
}

// ----------------------------------------------

QByteArray matrix::sendQuery(QByteArray query)
{
    if (socket->state() != QAbstractSocket::ConnectedState)
    {
        qWarning() << "Can't send command : not connected !";
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
            return resp;
        }
        qWarning() << "Waiting for data to read timed out. Nothing to read !";
    }
    return QByteArray();
}

// ----------------------------------------------

bool matrix::connection()
{
    this->socket->connectToHost(ipAddr, portNum);

    if(!socket->waitForConnected(500))
    {
        qWarning() << "Can't connect to : " << ipAddr << " on port " << portNum;
        return false;
    }
    return true;
}

// ----------------------------------------------

void matrix::closeConnection()
{
    socket->abort();
}

// ----------------------------------------------

void matrix::updatePatch() {
    QByteArray resp = sendQuery("#VID? *\r");
    resp.remove(0, 8);
    QList<QByteArray> inouts = resp.split(',');
    patch.clear();
    for(int i = 0; i < PATCH_SIZE; i++)
    {
        QList<QByteArray> tie = inouts.at(i).split('>');
        patch.insertMulti(tie.at(0).toInt(), tie.at(1).toInt());
    }
}

// SLOTS -------------------------------

void matrix::connected()
{
    qDebug() << "Connected to : " << ipAddr << " on port " << portNum;
    if(socket->waitForReadyRead((2000)))
    {
        while(socket->bytesAvailable() > 0) //purge socket buffer
        {
            socket->readLine();
        }
    }
//    timer->start();
    this->updatePatch();
    emit sigMatrixConnected();
}

// ----------------------------------------------

void matrix::disconnected()
{
    qDebug() << "Emitting signal Socket Disconnected";
    emit sigMatrixDisconnected();
}

//void matrix::keepAlive()
//{
//    sendCommand("%1ERST ?\r");
//    timer->start();
//}
