#ifndef PROJECTOR_H
#define PROJECTOR_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QTimer>

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
// 0x555445204f46460D



enum pjProtocol {
    PJLINK = 1,
    CHRISTIE = 2,
    OPTOMA = 3,
    PJDESIGN = 4,
    EPSON = 5
};

class Projector : public QObject
{
    Q_OBJECT
public:
    explicit Projector(QString _ip, quint16 _port, pjProtocol _comMode, QObject *parent = 0);
    ~Projector();

    QString ipAddr;
    quint16 portNum;
    pjProtocol comMode; // projector command protocol
    bool isSelected;

    quint16 timeout;

    // Socket Methods
    bool connection(QString &ip, quint16 port);
    void closeConnection();

    QAbstractSocket::SocketState socketState();

    //Projector Control Methods
    void selectInput(const QString input);
    void shutterOn();
    void shutterOff();
    void powerOn();
    void powerOff();
    void patternOn();
    void patternOff();
    void pgUpKey();
    void pgDownKey();

    QByteArray getInput();
    bool getShutterStatus();
    bool getPowerStatus();

    bool sendCommand(QByteArray command);

signals:
    void sigSocketConnected();
    void sigSocketDisconnected();

public slots:
    void connected();
    void disconnected();
    void keepAlive();

private:
    QTcpSocket *TcpSocket;
    QTimer *timer;
};

#endif // PROJECTOR_H
