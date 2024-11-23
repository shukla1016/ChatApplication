#include "server.h"
#include <QDebug>
#include <QByteArray>
#include <QHostAddress>
#include <qglobal.h>
#include <QtEndian>

Server::Server(QObject *parent) : QTcpServer(parent) {}

void Server::startServer(quint16 port) {
    if (!listen(QHostAddress::Any, port)) {
        qDebug() << "Server could not start on port" << port;
    } else {
        qDebug() << "Server started on port" << port;
    }
}

void Server::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket *client = new QTcpSocket(this);
    client->setSocketDescriptor(socketDescriptor);
    clients.append(client);
    connect(client, &QTcpSocket::readyRead, this, &Server::readMessage);
    connect(client, &QTcpSocket::disconnected, this, &Server::clientDisconnected);
    qDebug() << "New client connected";
}

void Server::readMessage() {
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (client) {
        QByteArray message = client->readAll();

        // Debugging: Print the raw bytes of the message
        qDebug() << "Raw Message:" << message.toHex();

        // Check if the message is long enough
        if (message.size() >= 20) { // Minimum TCP header size is 20 bytes
            // Parse the message into a TCP-like packet
            TCPPacket packet = parseTCPPacket(message);

            // Display the TCP packet details
            displayTCPPacket(packet);

            // Broadcast the message to all other connected clients
            for (QTcpSocket *otherClient : clients) {
                if (otherClient != client && otherClient->state() == QTcpSocket::ConnectedState) {
                    otherClient->write(message);
                }
            }
        } else {
            qDebug() << "Received message is too short to parse as a TCP packet.";
        }
    }
}

void Server::clientDisconnected() {
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (client) {
        clients.removeAll(client);
        client->deleteLater();
        qDebug() << "Client disconnected";
    }
}

Server::TCPPacket Server::parseTCPPacket(const QByteArray &message) {
    TCPPacket packet;

        // Extract the TCP header
    // Extract the TCP header
        packet.src_port = *reinterpret_cast<const quint16*>(message.mid(0, 2).data());
        packet.dest_port = *reinterpret_cast<const quint16*>(message.mid(2, 2).data());
        packet.seq_num = *reinterpret_cast<const quint32*>(message.mid(4, 4).data());
        packet.ack_num = *reinterpret_cast<const quint32*>(message.mid(8, 4).data());
        packet.data_offset = (message[12] >> 4) & 0x0F;  // Data offset is the high nibble of byte 13
        packet.flags = message[13];  // Flags are the low nibble of byte 13
        packet.window_size = *reinterpret_cast<const quint16*>(message.mid(14, 2).data());
        packet.checksum = *reinterpret_cast<const quint16*>(message.mid(16, 2).data());
        packet.urgent_pointer = *reinterpret_cast<const quint16*>(message.mid(18, 2).data());

    // Extract TCP options (if any)
    if (packet.data_offset > 5) {
        packet.options = message.mid(20, (packet.data_offset - 5) * 4); // Options start after 20 bytes of the header
    }

    // Extract the actual data (the message payload)
    packet.data = message.mid(5 * 4); // Data starts after the header (data_offset * 4)

    return packet;
}

void Server::displayTCPPacket(const TCPPacket &packet) {
    qDebug() << "TCP Packet Received:";
    qDebug() << "Source Port:" << packet.src_port;
    qDebug() << "Destination Port:" << packet.dest_port;
    qDebug() << "Sequence Number:" << packet.seq_num;
    qDebug() << "Acknowledgment Number:" << packet.ack_num;
    qDebug() << "Data Offset:" << packet.data_offset << "words (header length)";
    qDebug() << "Flags:" << QString::number(packet.flags, 16);
    qDebug() << "Window Size:" << packet.window_size;
    qDebug() << "Checksum:" << packet.checksum;
    qDebug() << "Urgent Pointer:" << packet.urgent_pointer;

    // If options exist, display them
    if (!packet.options.isEmpty()) {
        qDebug() << "Options:" << packet.options.toHex();
    }

    // Display the data (payload)
    if (packet.data.isEmpty()) {
        qDebug() << "No data in the packet.";
    } else {
        qDebug() << "Data:" << packet.data;
    }
}
