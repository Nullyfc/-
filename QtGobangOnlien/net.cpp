#include "net.h"
#include "ui_net.h"

Net::Net(QSqlDatabase db, QString name, QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::Net)
{

    dbc=db;
    game=new GameModel();
    ui->setupUi(this);
    setMouseTracking(true);
    socket =new QTcpSocket(this) ;//创建socket对象
    myname=name;
    //设置窗口大小60+40*16,60+40*16 700,700
    QDesktopWidget *desktop =QApplication::desktop();
    move((desktop->width() - width())/ 2, (desktop->height() - height()) /2);
    setFixedSize(MARGIN*2+BLOCK_SIZE *BOARD_GRAD_SIZE+200,
                 MARGIN*2+BLOCK_SIZE *BOARD_GRAD_SIZE);
    this->setWindowTitle("五子棋对战");
    QString IP =ui->lineEdit->text();
    QString port =ui->lineEdit_2->text();
    ui->label_7->setText(name);

    connect(socket,&QTcpSocket::connected,this,&Net::connectsucceed);
    connect(socket,&QTcpSocket::disconnected,this,&Net::connectfailed);
    connect(socket,&QTcpSocket::readyRead,this,&Net::recvdata) ;

    initGame();
}

Net::~Net()
{
    delete ui;
    if(game)
        delete  game;
}

void Net::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);
    QPixmap pixmap(":/image/chessboard_background.png");//背景图片
    QPixmap white("://image/white.png");//白棋
    QPixmap black("://image/black.png");//黑棋


    painter.drawPixmap(this->rect(),pixmap);
    painter.setRenderHint(QPainter::Antialiasing,true);//抗锯齿

    for(int i=0;i<BOARD_GRAD_SIZE+1 ;i++)
    {
        //从左到右画横线
        //30+40*0,30,70
        painter.drawLine(MARGIN+BLOCK_SIZE*i,MARGIN,
                         MARGIN+BLOCK_SIZE*i,size().width()-MARGIN-200);

        //从上到下画竖线
        painter.drawLine(MARGIN,MARGIN+BLOCK_SIZE*i,
                         size().width()-MARGIN-200,MARGIN+BLOCK_SIZE*i);

    }
    QBrush brush;

    if(isTurnYou) //是否轮到你
    {
        //绘制选中点

        brush.setStyle(Qt::SolidPattern);
        if(clickPosRow>0 && clickPosRow <BOARD_GRAD_SIZE &&
                clickPosCol >0 && clickPosCol<BOARD_GRAD_SIZE &&
                game->gameMapVec[clickPosRow][clickPosCol]==0)
        {
            if(yourcolor)
            {
                painter.drawPixmap(QRect(MARGIN+BLOCK_SIZE*clickPosCol-MARK_SIZE/2,MARGIN+BLOCK_SIZE*clickPosRow-MARK_SIZE/2,MARK_SIZE,MARK_SIZE),black);
            }
            else
            {
                painter.drawPixmap(QRect(MARGIN+BLOCK_SIZE*clickPosCol-MARK_SIZE/2,MARGIN+BLOCK_SIZE*clickPosRow-MARK_SIZE/2,MARK_SIZE,MARK_SIZE),white);
            }
        }
    }
    //绘制棋子

    for(int i=0;i<BOARD_GRAD_SIZE;i++)
    {
        for(int j=0;j<BOARD_GRAD_SIZE;j++)
        {
            if(game->gameMapVec[i][j]==1)
            {
                brush.setColor(Qt::black);
                painter.drawPixmap(QRect(MARGIN+BLOCK_SIZE*j-CHESS_RADIUS,
                                         MARGIN+BLOCK_SIZE*i-CHESS_RADIUS,
                                         CHESS_RADIUS*2,CHESS_RADIUS*2),black);

            }
            else if(game->gameMapVec[i][j]==-1)
            {
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawPixmap(QRect(MARGIN+BLOCK_SIZE*j-CHESS_RADIUS,
                                         MARGIN+BLOCK_SIZE*i-CHESS_RADIUS,
                                         CHESS_RADIUS*2,CHESS_RADIUS*2),white);

            }

        }

    }
    //判断输赢
    if(clickPosCol>0 && clickPosCol<BOARD_GRAD_SIZE &&
            clickPosRow>0 && clickPosRow<BOARD_GRAD_SIZE &&
            (game->gameMapVec[clickPosRow][clickPosCol]==1||
             game->gameMapVec[clickPosRow][clickPosCol]==-1))
    {
        if(game->isWin(clickPosRow,clickPosCol) && game->gameStatus == PLAYING)
        {
            game->gameStatus = WIN;
            QString str;
            str = game->gameMapVec[clickPosRow][clickPosCol]==1?"黑棋":"白棋";
            QMessageBox::StandardButton btnValue = QMessageBox::information(this,"五子棋",str+"胜利");
            if(btnValue == QMessageBox::Ok)
            {
                if(yourcolor)
                {
                    isTurnYou=true;
                }
                else
                {
                    isTurnYou=false;

                }

                game->startGame(game_type);
                game->gameStatus = PLAYING;

            }

        }

    }

}

void Net::mouseMoveEvent(QMouseEvent *event)
{
    if(isTurnYou)
    {
        int x=event->x();
        int y=event->y();
        if(x>=MARGIN+BLOCK_SIZE/2 &&
                x<size().width()-MARGIN-BLOCK_SIZE/2 &&
                y>=MARGIN +BLOCK_SIZE/2 &&
                y<size().height()-MARGIN-BLOCK_SIZE/2)
        {
            int col=(x-MARGIN)/BLOCK_SIZE;
            int row=(y-MARGIN)/BLOCK_SIZE;

            int leftTopPosX=MARGIN +BLOCK_SIZE *col;
            int leftTopPosY=MARGIN +BLOCK_SIZE *row;
            clickPosCol=-1;
            clickPosRow=-1;
            int len=0;
            selectPos =false;
            len=sqrt((x-leftTopPosX)*(x-leftTopPosX)+(y-leftTopPosY)*(y-leftTopPosY));
            if(len<POS_OFFSET)
            {
                clickPosCol=col;
                clickPosRow=row;
                if(game->gameMapVec[clickPosRow][clickPosCol]==0)
                {
                    selectPos=true;

                }

            }
            len=sqrt((x-leftTopPosX-BLOCK_SIZE)*(x-leftTopPosX-BLOCK_SIZE)+(y-leftTopPosY-BLOCK_SIZE)*(y-leftTopPosY-BLOCK_SIZE));
            if(len<POS_OFFSET)
            {
                clickPosCol=col+1;
                clickPosRow=row+1;
                if(game->gameMapVec[clickPosRow][clickPosCol]==0)
                {
                    selectPos=true;
                }

            }
            update();
        }
    }



}

void Net::mouseReleaseEvent(QMouseEvent *event)
{
    if(selectPos == false){
        return;
    }
    else{
        selectPos = false;
    }
    //由人下棋
    chessOneByPerson();

}

void Net::initGame()
{
    game_type =MAN;
    game->gameStatus = PLAYING;
    game->startGame(game_type);
    update();
}

void Net::chessOneByPerson()
{
    if(clickPosRow!=-1 && clickPosCol!=-1 &&
            game->gameMapVec[clickPosRow][clickPosCol]==0)
    {

        playclickCol=clickPosCol;
        playclickRow=clickPosRow;
        game->netupdate(clickPosRow,clickPosCol,yourcolor);
        //音效
        //TODO:
        QString row=QString::number(playclickRow);
        QString col=QString::number(playclickCol);
        QString gamedata=row+"|"+col;
        socket->write(gamedata.toUtf8().data());
        update();
        isTurnYou=false;
        ui->waitLabel->setText("等待对方落子");
    }
}

//void Net::connectDB()
//{
//    db=QSqlDatabase::addDatabase("QMYSQL"); //打开mysql数据库驱动
//    db.setDatabaseName("test");
//    db.setHostName("8.130.36.138");
//    db.setUserName("yfc");
//    db.setPassword("1342148264Yfc");
//    db.open();
//}


void Net::connectsucceed()
{
    QMessageBox::information(this,"连接提示","服务器连接成功！");
    ui->label_4->setText("服务器连接成功");
    game->gameStatus = PLAYING;
    update();
}

void Net::recvdata()
{

    QByteArray ba=socket->readAll();
    ui->textEdit->append(ba);
    QString str;
    str.append(ba);
    if(execonly) //只获取一次
    {
        if(str.contains("black",Qt::CaseSensitive))//如果字符串中包含black 表示自己是黑棋
        {
            yourcolor=true;//黑棋
            game->playerFlag=true;
        }
        else
        {
            yourcolor=false;//白棋
            game->playerFlag=false;
        }
        if(str.contains("%n",Qt::CaseSensitive))
        {
            QString yourname1=str.remove("%n");

            ui->yourname_Label->setText(yourname1);
            qDebug()<<yourname1;

        }
        execonly=false;

    }

    if(str=="ready")
    {
        QString str=myname+"%n";
        socket->write(str.toUtf8().data());
        qDebug()<<str;
    }
    if(str.contains("%n",Qt::CaseSensitive))//如果字符串中包含%n 表示对方的名字
    {

        if(execonly2)
        {
            QString yourname1=str.remove("%n");
         ;
            ui->yourname_Label->setText(yourname1);
            QString str=myname+"%n";
            socket->write(str.toUtf8().data());
            qDebug()<<yourname1;

        }
        execonly2=false;


    }
    if(str.contains("T",Qt::CaseSensitive))//如果字符串中包含T 表示轮到自己
    {
        isTurnYou=true;
        QFont ft;
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::red);
        ft.setPointSize(18);
        ui->waitLabel->setFont(ft);
        ui->waitLabel->setPalette(pa);
        ui->waitLabel->setText("轮到你了");

    }
    if(str.contains("同意悔棋!",Qt::CaseSensitive))//如果字符串中包含同意悔棋!
    {
        game->gameMapVec[playclickRow][playclickCol]=0;
        game->gameMapVec[yourplayclickRow][yourplayclickCol]=0;
        update();
    }
    if(str.contains("&",Qt::CaseSensitive))//如果字符串中包含& 表示轮到对方发来信息
    {

        ui->textEdit_2->append(str.section('&',1,1));
        qDebug()<<str.section('&',1,1);
    }

    if(str.contains("regret",Qt::CaseSensitive))
    {
        QMessageBox *msgBox = new QMessageBox(QMessageBox::Question,tr("提示"),
                                              QString(tr("对方请求悔棋，是否同意")),
                                              QMessageBox::Yes | QMessageBox::No);
        msgBox->button(QMessageBox::Yes)->setText("同意");
        msgBox->button(QMessageBox::No)->setText("拒绝");
        flag  = msgBox->exec();
        if(flag==QMessageBox::Yes)
        {
            if(playclickCol>0&&playclickRow>0&&yourplayclickRow>0&&yourplayclickCol>0)
            {
                QString str="&同意悔棋!&";
                socket->write(str.toUtf8().data());
                game->gameMapVec[playclickRow][playclickCol]=0;
                game->gameMapVec[yourplayclickRow][yourplayclickCol]=0;
                update();
            }
            else

            {

                QMessageBox::information(this,"提示","无法悔棋");
            }


        }
        else
        {
            QString str="&拒绝悔棋&";
            socket->write(str.toUtf8().data());
        }
    }
    if(str.contains("Surrender",Qt::CaseSensitive))
    {
        QString str="&对方投降&";
        socket->write(str.toUtf8().data());
        QMessageBox::information(this,"提示","对方已经投降！");
        QString sql="update user set total=total+1 where username='"+myname+"';";
        QString sql2="update user set win=win+1 where username='"+myname+"';";
        QSqlQuery query;
        query.exec(sql);
        query.exec(sql2);
        if(yourcolor)
        {
            isTurnYou=true;
        }
        else
        {
            isTurnYou=false;

        }
        initGame();

    }
    int row=str.section('|',1,1).toInt();
    int col=str.section('|',2,2).toInt();
    qDebug()<<row;
    qDebug()<<col;
    qDebug()<<str;
    if ((row<=1) ||(row>=16)||(col<=0) ||(col>=16))
    {
        return;
    }

    yourplayclickRow=row;
    yourplayclickCol=col;
    game->netupdate(row,col,!yourcolor);
    update();


}


void Net::connectfailed()
{
    ui->label_4->setText("服务器断开连接");
    QMessageBox::warning(this,"连接提示","服务器断开连接！");


}

void Net::on_connectButton_clicked() //连接服务器
{
    QString IP =ui->lineEdit->text();
    QString port =ui->lineEdit_2->text();
    socket->connectToHost(QHostAddress(IP),port.toShort());

    initGame();
    update();
}

void Net::on_pushButton_clicked()
{
    if(isTurnYou)
    {
        QString row=QString::number(playclickRow);
        QString col=QString::number(playclickCol);
        QString gamedata=row+","+col;
        socket->write(gamedata.toUtf8().data());
    }//向服务器发送数据

}



void Net::on_backButton_clicked()//返回
{
    QMessageBox *msgBox = new QMessageBox(QMessageBox::Question,tr("提示"),
                                          QString(tr("确定要返回主界面吗?")),
                                          QMessageBox::Yes | QMessageBox::No);
    msgBox->button(QMessageBox::Yes)->setText("确定");
    msgBox->button(QMessageBox::No)->setText("取消");
    flag  = msgBox->exec();
    if(flag==QMessageBox::Yes)
    {
        socket->close();
        this->hide();
        Widget *w=new Widget;
        w->show();

    }
}



void Net::on_sendmessage_Button_clicked()//聊天
{
    QString str;
    str="&"+myname+":"+ui->sendmessage_LineEdit->text()+"&";
    qDebug()<<str;
    socket->write(str.toUtf8().data());
}

void Net::on_regret_Button_clicked()//请求悔棋
{
    if(isTurnYou)
    {
        QString str="regret";
        socket->write(str.toUtf8().data());
    }
    else
    {
        QMessageBox::information(this,"提示","未轮到你");
    }
}
void Net::on_showMeScore_Button_clicked()//展示自己战绩
{
    Score *s=new Score(myname);
    s->show();
}
void Net::on_showYourSocre_Button_clicked()//展示对方战绩战绩
{
    if((ui->yourname_Label->text().isEmpty()))
    {
        QMessageBox::warning(this,"提示","未找到对手");
        return;
    }
    yourname=ui->yourname_Label->text();
    Score *s=new Score(yourname);
    s->show();

}

void Net::on_surrender_Button_clicked() //点击投降
{
    if(!(ui->yourname_Label->text().isEmpty()))
    {
        if(isTurnYou)
        {
            QString str="Surrender";
            socket->write(str.toUtf8().data());
            QString sql="update user set total=total+1 where username='"+myname+"';";
            QString sql2="update user set loss=loss+1 where username='"+myname+"';";
            QSqlQuery query;
            query.exec(sql);
            query.exec(sql2);
            if(yourcolor)
            {
                isTurnYou=true;
            }
            else
            {
                isTurnYou=false;

            }
            initGame();
        }
        else
        {
            QMessageBox::information(this,"提示","未轮到你！");
        }
    }
    else
    {
        QMessageBox::information(this,"提示","没有对手，无法投降！");
    }
}
