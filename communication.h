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
    Q_INVOKABLE void startHeartbeat();
    Q_INVOKABLE void stopHeartbeat();
    char *showBufferInByte(void *unk_buf, unsigned long byte_cnt, QString mes);

signals:
    void signalSequenceReceivedFromHeartBeat(int sequence);

private slots:
    void sendData();
    void slotReadyRead();

private:
    QUdpSocket udpSocket;
    QTimer timer;
};

#endif // COMMUNICATION_H
