#ifndef PJWIDGET_H
#define PJWIDGET_H

#include <QWidget>
#include <QtGui>
#include "projector.h"

namespace Ui {
class pjWidget;
}

class pjWidget : public QWidget
{
    Q_OBJECT

public:
    explicit pjWidget(qint16 _pid, QString _ip, quint16 _port, pjProtocol _protocol, QWidget *parent = 0);
    ~pjWidget();

    qint16 pid; // projector ID

    void selectProjector(bool checked);
    bool isSelected();
    QAbstractSocket::SocketState socketState();
    void connection(QString ip, quint16 port);
    void closeConnection();
    void shutterOn();
    void shutterOff();
    void patternOn();
    void patternOff();
    void patternNext();
    void patternPrev();
    void powerOn();
    void powerOff();

    QString getIp();
    quint16 getPort();

    void updateStatus();

    void setProjectorName(QString pjName);
    void setProjectorModel(QString pjModel);

signals:
    void sigPjConnected();
    void sigPjDisconnected();

public slots:
    void pjConnected();
    void pjDisconnected();

private slots:
    void on_mute_clicked();
    void on_unmute_clicked();
    void on_on_clicked();
    void on_off_clicked();
    void on_patternOn_clicked();
    void on_patternOff_clicked();
    void on_patternNext_clicked();
    void on_patternPrev_clicked();

private:
    Ui::pjWidget *ui;
    Projector *proj; // projector core functions
};

#endif // PJWIDGET_H
