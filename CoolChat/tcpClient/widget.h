#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void newConnect();
    void readMessage();
    void displayError(QAbstractSocket::SocketError);
    void on_pushButton_clicked();
    void on_sendButton_clicked();
    void on_clearButton_clicked();
    void on_CloseButton_clicked();
private:
    Ui::Widget *ui;
    QTcpSocket *tcpSocket;
    QString message;
    quint16 blockSize;
};

#endif // WIDGET_H
