#include "register.h"
#include "ui_register.h"

Register::Register(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    QDesktopWidget *desktop =QApplication::desktop();
    move((desktop->width() - width())/ 2, (desktop->height() - height()) /2);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setAttribute(Qt::WA_TranslucentBackground);
}
Register::~Register()
{
    delete ui;
}

void Register::connectDB()
{
    db=QSqlDatabase::addDatabase("QMYSQL"); //打开mysql数据库驱动
    db.setDatabaseName("test");
    db.setHostName("8.130.36.138");
    db.setUserName("yfc");
    db.setPassword("1342148264Yfc");
    db.open();
}




void Register::on_toolButton_2_clicked()
{
    showMinimized();
}

void Register::on_toolButton_clicked()
{
    close();
}

void Register::on_backButton_clicked()
{
    this->hide();
    Widget *w = new Widget;
    w->show();
}



void Register::on_registButton_clicked()
{
    connectDB();
    QString name = ui->nameEdit->text();
    QString password = ui->passwordEdit->text();
    QString password2 = ui->passwordEdit2->text();
    if(name == NULL||password==NULL||password2==NULL)
    {
        QMessageBox::warning(this,"警告","账户或密码输入为空！");
        return;
    }
    else if(password!=password2)
    {
        QMessageBox::warning(this,"警告","两次密码不一致，请重新输入！");
        return;

    }
    else
    {


        QSqlQuery sql;

        QString sqlstr =QString("insert into user(username,userpassword)values('%1','%2')").arg(name).arg(password);

        if(!sql.exec(sqlstr) )
        {
            QMessageBox::warning(this,"警告","该账号存在！");
            return ;
        }
        else
        {
            QMessageBox::information(this,"通知","注册成功！\n");
        }
    }
    db.close();

}

