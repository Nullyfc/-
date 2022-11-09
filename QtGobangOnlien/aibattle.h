#ifndef AIBATTLE_H
#define AIBATTLE_H
#include <QDesktopWidget>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <math.h>
#include <QTimer>
#include<QMessageBox>
#include<QSqlDatabase>
#include<QDebug>
#include<QSqlError>
#include<QSqlQuery>
#include<QtSql/QSqlDatabase>
#include<iostream>
#include<QSqlTableModel>

#include "GameModel.h"
#include "widget.h"
namespace Ui {
class AiBattle;
}

class AiBattle : public QWidget
{
    Q_OBJECT

public:
    explicit AiBattle(QWidget *parent = nullptr);


    int playclickRow=1,playclickCol=1;

    int aiclickPosRow=1,aiclickPosCol=1;


    ~AiBattle();


private:
    Ui::AiBattle *ui;
    GameModel *game;
    GameType  game_type;
    int flag;
    int clickPosRow,clickPosCol;
    int defaultrow=7;
    bool selectPos =false;
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void initGame();
    void initAIGame();
    void chessOneByPerson();

private slots:
    void chessOneByAI();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
};

#endif // AIBATTLE_H
