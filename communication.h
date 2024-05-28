#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>
#include <c_library_v2/all/mavlink.h>

class Communication : public QObject
{   
    Q_OBJECT
public:
    explicit Communication(QObject *parent = nullptr);
    Q_INVOKABLE void startHeartbeat(const QString &_ipAddress);
    Q_INVOKABLE void stopHeartbeat();
    char *showBufferInByte(void *unk_buf, unsigned long byte_cnt, QString mes);
    bool isValidIPv4(const QString &ip);

signals:
    void signalSequenceReceivedFromHeartBeat(int sequence);
    void signalIP_AddressIsInvalid();

private slots:
    void sendData();
    void slotReadyRead();

private:
    QUdpSocket udpSocket;
    QHostAddress host;
    QString ipAddress;
    QTimer timer;
};

#endif // COMMUNICATION_H
