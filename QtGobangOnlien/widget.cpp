#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{


    ui->setupUi(this);
    QDesktopWidget *desktop =QApplication::desktop();
    move((desktop->width() - width())/ 2, (desktop->height() - height()) /2);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setAttribute(Qt::WA_TranslucentBackground);
    connect(ui->label_2,SIGNAL(clicked()),this,SLOT(onLabelClicked()));
    QSettings setting ("./config.ini",QSettings::IniFormat);  //设置一个用于存储的ini文件
    QString zhangHao = setting.value("zhangHao").toString();//读取账号
    QString miMa = setting.value("miMa").toString();//读取密码
    ui->lineEdit->setText(zhangHao); //设置账号
    ui->lineEdit_2->setText(miMa);  //设置密码

}

Widget::~Widget()
{

    if(ui->checkBox->isChecked()==true)
    {
        QSettings setting ("./config.ini",QSettings::IniFormat);  //设置一个用于存储的ini文件
        setting.setValue("miMA",ui->lineEdit->text());
        setting.setValue("zhangHao",ui->lineEdit_2->text());
        setting.sync(); //同步设置，将信息存储到INI里面

    }
    delete ui;

    //放到析构函数中，关闭程序则将值记录下来

}


void Widget::on_toolButton_clicked()
{
    close();
}

void Widget::on_toolButton_2_clicked()
{
    showMinimized();
}
void Widget::mousePressEvent(QMouseEvent *e)
{

    if(e->button() == Qt::LeftButton)
    {
        //求坐标差值
        //当前点击坐标-窗口左上角坐标
        p = e->globalPos() - this->frameGeometry().topLeft();
    }
}

void Widget::openAibattle()//显示人机对战界面
{


    this->hide();
    AiBattle *a = new AiBattle;
    a->show();
}

void Widget::playSound()
{
    //TPDO


}

void Widget::checkConnection()
{
    db=QSqlDatabase::addDatabase("QMYSQL"); //打开mysql数据库驱动
    db.setDatabaseName("test");
    db.setHostName("8.130.36.138");
    db.setUserName("yfc");
    db.setPassword("1342148264Yfc");
    if(db.open())
    {
        QMessageBox::information(this,"连接提示","连接成功");
    }
    else
    {
        QMessageBox::warning(this,"连接提示","连接失败");
        qDebug()<<"error open database because"<<db.lastError().text();
        qDebug()<<QSqlDatabase::drivers();
    }

}


void Widget::connectDB()
{
    db=QSqlDatabase::addDatabase("QMYSQL"); //打开mysql数据库驱动
    db.setDatabaseName("test");
    db.setHostName("8.130.36.138");
    db.setUserName("yfc");
    db.setPassword("1342148264Yfc");
    db.open();
}

void Widget::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        //移到左上角
        move(e->globalPos() - p);
    }

}

void Widget::on_pushButton_2_clicked()
{
    openAibattle();
}

void Widget::onLabelClicked()
{

    Register *r =new Register;
    this->hide();

    r->show();
}

void Widget::on_pushButton_clicked()
{
    QString name= ui->lineEdit->text();
    QString passwd=ui->lineEdit_2->text();
    connectDB();
    QString sql=QString ("SELECT * from user WHERE username='%1' AND userpassword='%2';").arg(name).arg(passwd);
    qDebug()<<sql;
    QSqlQuery query;

    if(query.exec(sql))
    {
        if(query.next())
        {

            QMessageBox::information(this, "登陆提示", "登陆成功");
            this->hide();
            Net *n=new Net(db,name);
            n->show();

        }
        else
        {

            QMessageBox::information(this, "登陆提示", "登陆失败");

        }
    }


}





