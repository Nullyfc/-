#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <vector>
#include <algorithm>
using namespace std;
const int BOARD_GRAD_SIZE =15;
namespace Ui {
class server;
}

class server : public QWidget
{
    Q_OBJECT

public:
    bool isTurnyou=true;
    explicit server(QWidget *parent = nullptr);
    ~server();

      QString sign="T|"; //向客户端发送标识，收到T为自己的回合
      QString color="black|"; //玩家棋子颜色 black为黑色

public slots:

    void NewConnection();
    void disconnectedSlot();
    void ReadyRead();

public:
    QTcpServer *tcpServer; //监听套接字
    QList<QTcpSocket*> tcpClientList;//通信套接字
    QTcpSocket *tcpSocket;

private slots:
    void on_pushButton_connect_clicked();

    void on_pushButton_clearGetText_clicked();

    void on_pushButton_clearSend_clicked();

    void on_pushButton_send_clicked();

    void on_pushButton_appoint_clicked();

private:
    Ui::server *ui;

    quint16 port;
    QString ip;
};

#endif // SERVER_H
