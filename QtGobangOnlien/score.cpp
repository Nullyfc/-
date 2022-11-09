#include "score.h"
#include "ui_score.h"

Score::Score(QString username, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Score)
{

    QString name =username;
    ui->setupUi(this);
    this->setWindowTitle("战绩");
    QString sql="select * from user where username='"+name+"';";
    QString str=name+"的战绩";
    ui->label->setText(str);
    QSqlQuery query;
    query.exec(sql);
    while(query.next())
    {
       qDebug()<<query.value(3).toString();
       qDebug()<<query.value(4).toString();
       qDebug()<<query.value(3).toString();
       ui->total->setText(query.value(3).toString());
       ui->win->setText(query.value(4).toString());
       ui->loss->setText(query.value(5).toString());
       float rate=query.value(4).toFloat()/query.value(3).toFloat()*100.0;
       QString winra=QString("%1%").arg(rate);
       ui->label_9->setText(winra);
    }



    dbc.close();
}

Score::~Score()
{
    delete ui;
}

void Score::connectDB()
{
    dbc=QSqlDatabase::addDatabase("QMYSQL"); //打开mysql数据库驱动
    dbc.setDatabaseName("test");
    dbc.setHostName("8.130.36.138");
    dbc.setUserName("yfc");
    dbc.setPassword("1342148264Yfc");
    dbc.open();

}

void Score::on_pushButton_clicked()
{
    this->hide();
}
