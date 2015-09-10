#ifndef PROJECTOR_H
#define PROJECTOR_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QTimer>

enum pjProtocol {
    PJLINK = 1,
    CHRISTIE = 2,
    OPTOMA = 3,
    PJDESIGN = 4
};

class Projector : public QObject
{
    Q_OBJECT
public:
    explicit Projector(QString _ip, qint16 _port, pjProtocol _comMode, QObject *parent = 0);
    ~Projector();

    QString ipAddr;
    qint16 portNum;
    pjProtocol comMode; // projector command protocol
    bool isSelected;

    quint16 timeout;

    // Socket Methods
    bool connection(QString &ip, qint16 port);
    void closeConnection();

    QAbstractSocket::SocketState socketState();

    //Projector Control Methods
    void selectInput(const QString input);
    void shutterOn();
    void shutterOff();
    void powerOn();
    void powerOff();

    QByteArray getInput();
    bool getShutterStatus();
    bool getPowerStatus();

    bool sendCommand(QByteArray command);
    QByteArray sendQuery(QByteArray query);
    QByteArray parsePjlinkResponse(QByteArray msg);

    QMap<QString, QByteArray> pjInput;	// list of device inputs with associated commands

signals:
    void sigSocketConnected();
    void sigSocketDisconnected();

public slots:
    void connected();
    void disconnected();
    void keepAlive();

private:
    QTcpSocket *socket;
    QTimer *timer;
};

#endif // PROJECTOR_H
