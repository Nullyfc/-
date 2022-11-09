#ifndef NET_H
#define NET_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QPainter>
#include <QMouseEvent>
#include <math.h>
#include <QTimer>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <iostream>
#include <QSqlTableModel>
#include <QTcpSocket>
#include <QHostAddress>
#include <QPalette>
#include "GameModel.h"
#include "widget.h"
#include "score.h"

namespace Ui {
class Net;
}

class Net : public QMainWindow
{
    Q_OBJECT

public:
    explicit Net(QSqlDatabase db, QString name, QMainWindow *parent = nullptr);

    int playclickRow,playclickCol;//用于保存上一步棋
    int yourplayclickRow,yourplayclickCol;//用于保存上一步棋
    bool isTurnYou=false;//是否轮到自己
    bool yourcolor;


    ~Net();

private:
    Ui::Net *ui;
    GameModel *game;
    GameType  game_type;
    QString myname,yourname;
    int flag;
    int clickPosRow,clickPosCol;
    int defaultrow=7;
    bool selectPos =false;
    bool execonly=true; //让函数执行一次
    bool execonly2=true;
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void initGame();
    void initAIGame();
    void chessOneByPerson();
 //   void connectDB();



    QSqlDatabase  dbc;
    QTcpSocket *socket;

private slots:
    void connectsucceed();
    void recvdata();
    void connectfailed();
    void on_connectButton_clicked();
    void on_pushButton_clicked();
    void on_backButton_clicked();
    void on_showMeScore_Button_clicked();
    void on_sendmessage_Button_clicked();
    void on_regret_Button_clicked();
    void on_showYourSocre_Button_clicked();
    void on_surrender_Button_clicked();
};

#endif // NET_H
