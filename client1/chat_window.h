#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QTcpSocket>
#include <QByteArray>

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow();

private slots:
    void connectToServer();
    void sendMessage();
    void readMessage();

private:
    Ui::ChatWindow *ui;
    QTcpSocket *socket;
    quint32 seq_num;   // Sequence number (client side)
    quint32 ack_num;   // Acknowledgment number (client side)

    // Helper function to calculate checksum (Internet Checksum)
    quint16 calculateChecksum(const QByteArray &data);
};

#endif // CHATWINDOW_H
