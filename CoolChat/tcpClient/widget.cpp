#include "widget.h"
#include "ui_widget.h"
#include "QDebug"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    tcpSocket =new QTcpSocket(this);

    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::newConnect()
{
    blockSize =0;
    tcpSocket->abort();
    tcpSocket->connectToHost(ui->hostLineEdit->text(),ui->portLineEdit->text().toInt());
    qDebug()<<"connect with:"<<ui->hostLineEdit->text();
}

void Widget::readMessage()
{
    QByteArray buffer;
    //读取缓冲区数据
    buffer = tcpSocket->readAll();
    if(!buffer.isEmpty())
    {
        QString str = ui->recvTextEdit->toPlainText();
        //str+=tr(buffer);
        qDebug()<<buffer;
        str=tr(buffer);
        //刷新显示
        ui->recvTextEdit->append(str);
    }

}

void Widget::displayError(QAbstractSocket::SocketError)
{
    qDebug()<<tcpSocket->errorString();
}

void Widget::on_pushButton_clicked()
{
    newConnect();
}

void Widget::on_sendButton_clicked()
{

    //qDebug() << "Send: " << ui->sendMsgTextEdit->text();
    //获取文本框内容并以ASCII码形式发送
    tcpSocket->write(ui->sendMsgTextEdit->text().toLatin1());
    tcpSocket->flush();
    ui->sendMsgTextEdit->clear();
    qDebug()<<"send"<< ui->sendMsgTextEdit->text();
}
void Widget::on_clearButton_clicked()
{
    ui->recvTextEdit->clear();
}
void Widget::on_CloseButton_clicked()
{
    tcpSocket->disconnectFromHost();
    qDebug()<<"Disconnect";
}
