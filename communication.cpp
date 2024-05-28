#include "communication.h"
#include <QDebug>
#include <QRegularExpression>

Communication::Communication(QObject *parent) : QObject(parent)
{
    QObject::connect(&timer, &QTimer::timeout, this, &Communication::sendData);
}

void Communication::startHeartbeat(const QString &_ipAddress)
{
    qDebug()<<"Communication::startHeartbeat() << IP: "<<_ipAddress;
    this->ipAddress = _ipAddress;
    if(this->ipAddress == "")
    {
        this->host.setAddress(QHostAddress::LocalHost);
        QObject::connect(&udpSocket, &QUdpSocket::readyRead, this, &Communication::slotReadyRead);
        timer.start(1000);
    }
    else
    {
        if(this->isValidIPv4(this->ipAddress) == true)
        {
            this->host.setAddress(this->ipAddress);
            QObject::connect(&udpSocket, &QUdpSocket::readyRead, this, &Communication::slotReadyRead);
            timer.start(1000);
        }
        else
        {
            qDebug()<<"Communication::startHeartbeat() << IP Address is invalid";
            // emit colorChanged("red");
            emit signalIP_AddressIsInvalid();
        }
    }
}

bool Communication::isValidIPv4(const QString &ip) {
    // Biểu thức chính quy cho địa chỉ IPv4
    QRegularExpression ipRegex("^(25[0-5]|2[0-4][0-9]|[0-1]?[0-9]{1,2})\\."
                               "(25[0-5]|2[0-4][0-9]|[0-1]?[0-9]{1,2})\\."
                               "(25[0-5]|2[0-4][0-9]|[0-1]?[0-9]{1,2})\\."
                               "(25[0-5]|2[0-4][0-9]|[0-1]?[0-9]{1,2})$");

    QRegularExpressionMatch match = ipRegex.match(ip);
    return match.hasMatch();
}

void Communication::stopHeartbeat()
{
    qDebug()<<"Communication::stopHeartbeat()";
    QObject::disconnect(&udpSocket, &QUdpSocket::readyRead, this, &Communication::slotReadyRead);
    timer.stop();
}

void Communication::sendData()
{
    qDebug()<<"Communication::sendData()";
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    mavlink_msg_heartbeat_pack(1, 1, &msg, MAV_TYPE_QUADROTOR, MAV_AUTOPILOT_ARDUPILOTMEGA, MAV_MODE_PREFLIGHT, 0, MAV_STATE_UNINIT);
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);

    // this->showBufferInByte((uint8_t *)buf,len, "Data send: ");

    //Test from other devices same local network
    // QHostAddress host;
    // host.setAddress("192.168.0.173");
    // udpSocket.writeDatagram((char *)buf, len, /*QHostAddress::LocalHost*/host, 14550);
    //End test

    udpSocket.writeDatagram((char *)buf, len, QHostAddress::LocalHost, 14550);
}

void Communication::slotReadyRead()
{
    qDebug()<<"Communication::slotReadyRead()";
    while(udpSocket.hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket.pendingDatagramSize());
        udpSocket.readDatagram(datagram.data(), datagram.size());
        mavlink_message_t msg;
        mavlink_status_t status;
        // this->showBufferInByte((uint8_t *)&datagram,datagram.length(), "Data received: ");
        qDebug() << "Size of datagram: " << datagram.size();
        for(int i = 0; i < datagram.size(); i++)
        {
            if(mavlink_parse_char(MAVLINK_COMM_0, (uint8_t)datagram[i], &msg, &status))
            {
                // this->showBufferInByte((uint8_t *)&msg, msg.len + 12, "msg received: ");
                qDebug() << "Received msgid: " << msg.msgid;
                qDebug() << "Received len: " << msg.len;
                if(msg.msgid == MAVLINK_MSG_ID_HEARTBEAT)
                {
                    qDebug() << "Received seq: " << msg.seq;
                    emit signalSequenceReceivedFromHeartBeat(msg.seq);
                }
            }
            else
            {
                qDebug() << "Failed to parse char at " << i;
            }
        }
    }
}

char* Communication::showBufferInByte(void* unk_buf, unsigned long byte_cnt, QString mes)
{
    printf("%s - size = %ld\n", mes.toLatin1().data(), byte_cnt);

    unsigned char* buf = (unsigned char*) unk_buf;

    printf( "\n--------------------------------------------------------------\n");
    printf( " Offset |");
    for(unsigned long i = 0x00000000; i <= 0x0000000F; i++)
    {
        if((i % 8 == 0) && (i != 0))
        {
            printf( " ");
        }
        printf( "%2lX ", i);
    }
    printf( "\n--------------------------------------------------------------");
    unsigned long off = 0x00000000;
    printf( "\n%08lX |", off++);
    unsigned long i = 0;
    unsigned long sec_cnt = 0;
    for(; i < byte_cnt; i++)
    {
        if(i % 8 == 0)
        {
            if(i != 0)
            {
                printf( " ");
            }
        }
        if(i % 16 == 0)
        {
            if(i != 0)
            {
                printf( "| ");
                unsigned long idx = 16;
                for(unsigned long j = 1; j <= 16; j++)
                {
                    unsigned char c = buf[i - idx--];
                    if(c >= 33 && c <= 126)
                    {
                        printf( "%c", c);
                    }
                    else
                    {
                        printf( ".");
                    }
                }
                if(i % 512 == 0)
                {
                    printf( "\nSector %ld\n", ++sec_cnt);
                }
                printf( "\n%08lX |", off++);
            }
        }
        printf( "%02X ", buf[i]);
    }
    printf( " | ");
    unsigned long idx = 16;
    for(unsigned long j = 1; j <= 16; j++)
    {
        unsigned char c = buf[i - idx--];
        if(c >= 33 && c <= 126)
        {
            printf( "%c", c);
        }
        else
        {
            printf( ".");
        }
    }

    printf( "\n\n");
    return (char*)QString("").toLatin1().data();
}
