#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QMouseEvent>
#include<QMessageBox>

class mylabel : public QLabel
{
    Q_OBJECT
public:
    explicit mylabel(QWidget *parent = nullptr);

signals:
    void clicked();

protected:
    virtual void mousePressEvent(QMouseEvent* event);//重写mousePressEvent事件
};

#endif // MYLABEL_H
