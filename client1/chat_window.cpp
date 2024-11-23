#include "chat_window.h"
#include "ui_chat_window.h"
#include <QDebug>
#include <QByteArray>
#include <QTcpSocket>
#include <QHostAddress>

// Define the TCP-like packet structure
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
    QByteArray data;         // Payload (message data)
};

ChatWindow::ChatWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::ChatWindow), socket(new QTcpSocket(this)),
      seq_num(1000), ack_num(0) {  // Initial sequence and acknowledgment numbers
    ui->setupUi(this);

    connect(ui->connectButton, &QPushButton::clicked, this, &ChatWindow::connectToServer);
    connect(ui->sendButton, &QPushButton::clicked, this, &ChatWindow::sendMessage);
    connect(socket, &QTcpSocket::readyRead, this, &ChatWindow::readMessage);
}

ChatWindow::~ChatWindow() {
    delete ui;
}

void ChatWindow::connectToServer() {
    socket->connectToHost(ui->serverIP->text(), ui->serverPort->text().toUShort());

    if (socket->waitForConnected(3000)) {
        ui->chatHistory->append("Connected to server.");
    } else {
        ui->chatHistory->append("Connection failed: " + socket->errorString());
    }
}

void ChatWindow::sendMessage() {
    if (socket->state() == QTcpSocket::ConnectedState) {
        // Create a custom TCP-like packet
        TCPPacket packet;
        packet.src_port = 12345; // Source port (client side)
        packet.dest_port = ui->serverPort->text().toUShort(); // Destination port (server side)
        
        // Sequence number is incremented based on the previous one
        packet.seq_num = seq_num;
        seq_num += ui->messageInput->text().size(); // Increment by size of the payload
        
        // Acknowledgment number is set to the sequence number of the last received packet
        packet.ack_num = ack_num;
        
        // Data offset: fixed size of TCP header (5 words = 20 bytes)
        packet.data_offset = 5;

        // Example: Flags set for data transmission (no SYN/ACK/FIN)
        packet.flags = 0x18; // Example with PSH (Push) and ACK flags set

        // Window size: Fixed for simplicity
        packet.window_size = 1024; // Example window size

        // Dummy checksum calculation (we'll calculate it after creating the packet)
        packet.checksum = 0;

        // No urgent pointer for simplicity
        packet.urgent_pointer = 0;

        // Add the message data (the text typed by the user)
        packet.data = ui->messageInput->text().toUtf8();

        // Serialize the TCP packet into QByteArray
        QByteArray message;
        message.append(reinterpret_cast<char*>(&packet.src_port), sizeof(packet.src_port));
        message.append(reinterpret_cast<char*>(&packet.dest_port), sizeof(packet.dest_port));
        message.append(reinterpret_cast<char*>(&packet.seq_num), sizeof(packet.seq_num));
        message.append(reinterpret_cast<char*>(&packet.ack_num), sizeof(packet.ack_num));
        message.append(reinterpret_cast<char*>(&packet.data_offset), sizeof(packet.data_offset));
        message.append(reinterpret_cast<char*>(&packet.flags), sizeof(packet.flags));
        message.append(reinterpret_cast<char*>(&packet.window_size), sizeof(packet.window_size));
        message.append(reinterpret_cast<char*>(&packet.checksum), sizeof(packet.checksum));
        message.append(reinterpret_cast<char*>(&packet.urgent_pointer), sizeof(packet.urgent_pointer));
        message.append(packet.data);

        // Calculate the checksum (after the packet is serialized)
        packet.checksum = calculateChecksum(message);
        
        // Replace the checksum in the message with the calculated value
        message.remove(16, 2); // Remove old checksum from message
        message.insert(16, reinterpret_cast<char*>(&packet.checksum), sizeof(packet.checksum));

        // Send the message
        socket->write(message);

        ui->chatHistory->append("Sent: " + QString(packet.data));
        ui->messageInput->clear();
    }
}

void ChatWindow::readMessage() {
    QByteArray message = socket->readAll();

    // Ensure there's enough data for the TCP header (at least 20 bytes)
    if (message.size() >= 20) {
        TCPPacket packet;

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

        // Extract the payload data (after the header)
        // The data offset tells us where the data begins (multiplied by 4 as it's in words)
        int headerLength = 5 * 4;

        if (message.size() > headerLength) {
            packet.data = message.mid(headerLength);  // Get the message after the header
            qDebug() << "Here";
        }
        qDebug() << "msg size = " << message.size();
        // Debugging: print all TCP packet details
        qDebug() << "Received TCP Packet:";
        qDebug() << "Source Port:" << packet.src_port;
        qDebug() << "Destination Port:" << packet.dest_port;
        qDebug() << "Sequence Number:" << packet.seq_num;
        qDebug() << "Acknowledgment Number:" << packet.ack_num;
        qDebug() << "Flags:" << QString::number(packet.flags, 16);
        qDebug() << "Window Size:" << packet.window_size;
        qDebug() << "Checksum:" << packet.checksum;
        qDebug() << "Urgent Pointer:" << packet.urgent_pointer;
        qDebug() << "Data:" << packet.data;

        // Only display the actual message (data)
        if (!packet.data.isEmpty()) {
            ui->chatHistory->append("Received: " + QString(packet.data));
        } else {
            qDebug() << "No data in the packet.";
        }

        // Update the acknowledgment number for the next message
        ack_num = packet.seq_num + packet.data.size();
    } else {
        qDebug() << "Received message is too small to contain a full TCP packet.";
    }
}


// Calculate the checksum (Internet Checksum Algorithm)
quint16 ChatWindow::calculateChecksum(const QByteArray &data) {
    quint32 checksum = 0;
    const quint16 *dataPtr = reinterpret_cast<const quint16*>(data.constData());
    int dataSize = data.size();

    while (dataSize > 1) {
        checksum += *dataPtr++;
        dataSize -= 2;
    }

    // Add the remaining byte if any
    if (dataSize > 0) {
        checksum += *(reinterpret_cast<const quint8*>(dataPtr));
    }

    // Fold 32-bit sum to 16 bits
    while (checksum >> 16) {
        checksum = (checksum & 0xFFFF) + (checksum >> 16);
    }

    return static_cast<quint16>(~checksum);
}
