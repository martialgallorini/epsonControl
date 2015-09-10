#ifndef MATRIX_H
#define MATRIX_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
//#include <QTimer>

#define PATCH_SIZE 10

class matrix : public QObject
{
    Q_OBJECT
public:
    explicit matrix(QString _ip, qint16 _port, QObject *parent = 0);
    ~matrix();

    QString ipAddr;
    qint16 portNum;

//    quint16 timeout;

    // Socket Methods
    bool connection();
    void closeConnection();

    QAbstractSocket::SocketState socketState();

    bool sendCommand(QByteArray command);
    QByteArray sendQuery(QByteArray query);

    void updatePatch();
    const QMap<qint8, qint8>& getPatch() {return patch;}


signals:
    void sigMatrixConnected();
    void sigMatrixDisconnected();

public slots:
    void connected();
    void disconnected();
//    void keepAlive();

private:
    QTcpSocket *socket;
//    QTimer *timer;
    QMap<qint8, qint8> patch; // IN > OUT patch
};

#endif // MATRIX_H
