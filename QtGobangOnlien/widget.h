#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include "aibattle.h"
#include "mylabel.h"
#include "register.h"
#include<QSettings>
#include<QDesktopWidget>
#include "net.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();


    void mouseMoveEvent(QMouseEvent *e);//鼠标移动
    void mousePressEvent(QMouseEvent *e);//鼠标按下移动
    void openAibattle();
    void playSound();
    void checkConnection();
    void connectDB();


private slots:
    void on_toolButton_clicked();
    void on_toolButton_2_clicked();
    void on_pushButton_2_clicked();
    void onLabelClicked();




    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    QPoint p;
    QSqlDatabase  db;
    mylabel * mylable=new mylabel;

};
#endif // WIDGET_H
