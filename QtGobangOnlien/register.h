#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include "widget.h"
#include<QSqlError>
#include<QSqlQuery>
#include<QtSql/QSqlDatabase>
#include"aibattle.h"

namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();
    void connectDB();


private slots:


    void on_toolButton_2_clicked();

    void on_toolButton_clicked();

    void on_backButton_clicked();

    void on_registButton_clicked();

private:
    Ui::Register *ui;
    QSqlDatabase  db;

};

#endif // REGISTER_H
