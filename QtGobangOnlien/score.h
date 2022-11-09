#ifndef SCORE_H
#define SCORE_H
#include<QSqlDatabase>
#include <QMainWindow>
#include<QSqlQuery>
#include<QDebug>
namespace Ui {
class Score;
}

class Score : public QMainWindow
{
    Q_OBJECT

public:
    explicit Score(QString username, QWidget *parent = nullptr);
    ~Score();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Score *ui;
    void connectDB();
    QSqlDatabase  dbc;
};

#endif // SCORE_H
