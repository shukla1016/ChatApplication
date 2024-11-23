#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QByteArray>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);
    void startServer(quint16 port);

private slots:
    void incomingConnection(qintptr socketDescriptor);
    void readMessage();
    void clientDisconnected();

private:
    // Nested structure for the TCP-like packet
    struct TCPPacket {
        quint16 src_port;        // Source Port
        quint16 dest_port;       // Destination Port
        quint32 seq_num;         // Sequence number
        quint32 ack_num;         // Acknowledgment number
        quint8 data_offset;      // Data offset (header length)
        quint8 flags;            // TCP Flags (SYN, ACK, FIN, etc.)
        quint16 window_size;     // Window size
        quint16 checksum;        // Checksum (for simplicity, a 16-bit value)
        quint16 urgent_pointer;  // Urgent pointer (if any)
        QByteArray options;      // TCP options (if any)
        QByteArray data;         // Payload (message data)
    };

    // Function to parse the raw message into a TCP packet structure
    TCPPacket parseTCPPacket(const QByteArray &message);

    // Function to display the TCP packet information
    void displayTCPPacket(const TCPPacket &packet);

    QList<QTcpSocket*> clients;
};

#endif // SERVER_H
