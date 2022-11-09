#include "aibattle.h"
#include "ui_aibattle.h"

AiBattle::AiBattle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AiBattle)
{
    game=new GameModel();
    ui->setupUi(this);
    setMouseTracking(true);
    //设置窗口大小60+40*16,60+40*16 700,700
    QDesktopWidget *desktop =QApplication::desktop();
    move((desktop->width() - width())/ 2, (desktop->height() - height()) /2);
    setFixedSize(MARGIN*2+BLOCK_SIZE *BOARD_GRAD_SIZE+200,
                 MARGIN*2+BLOCK_SIZE *BOARD_GRAD_SIZE);


    initGame();
    QMessageBox *msgBox = new QMessageBox(QMessageBox::Question,tr("提示"),
                                          QString(tr("执黑还是执白?")),
                                          QMessageBox::Yes | QMessageBox::No);
    msgBox->button(QMessageBox::Yes)->setText("黑棋");
    msgBox->button(QMessageBox::No)->setText("白棋");
    flag  = msgBox->exec();
    if(flag==QMessageBox::No)
    {
        game->gameMapVec[8][7]=1;
        update();
        game->playerFlag=false;

    }
}

AiBattle::~AiBattle()
{
    delete ui;
    if(game)
        delete  game;

}

void AiBattle::paintEvent(QPaintEvent *event)
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

    //绘制选中点

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);

    if(clickPosRow>0 && clickPosRow <BOARD_GRAD_SIZE &&
            clickPosCol >0 && clickPosCol<BOARD_GRAD_SIZE &&
            game->gameMapVec[clickPosRow][clickPosCol]==0)
    {
        if(game->playerFlag)
            //            brush.setColor(Qt::black);
            painter.drawPixmap(QRect(MARGIN+BLOCK_SIZE*clickPosCol-MARK_SIZE/2,MARGIN+BLOCK_SIZE*clickPosRow-MARK_SIZE/2,MARK_SIZE,MARK_SIZE),black);
        else
            painter.drawPixmap(QRect(MARGIN+BLOCK_SIZE*clickPosCol-MARK_SIZE/2,MARGIN+BLOCK_SIZE*clickPosRow-MARK_SIZE/2,MARK_SIZE,MARK_SIZE),white);
        //            brush.setColor(Qt::white);
        //        painter.setBrush(brush);
        //        painter.drawRect(MARGIN+BLOCK_SIZE*clickPosCol-MARK_SIZE/2,MARGIN+BLOCK_SIZE*clickPosRow-MARK_SIZE/2,MARK_SIZE,MARK_SIZE);



    }
    //绘制棋子

    for(int i=0;i<BOARD_GRAD_SIZE;i++)
    {
        for(int j=0;j<BOARD_GRAD_SIZE;j++)
        {
            if(game->gameMapVec[i][j]==1)
            {
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                //                painter.drawEllipse(MARGIN+BLOCK_SIZE*j-CHESS_RADIUS,
                //                                    MARGIN+BLOCK_SIZE*i-CHESS_RADIUS,
                //                                    CHESS_RADIUS*2,CHESS_RADIUS*2);
                painter.drawPixmap(QRect(MARGIN+BLOCK_SIZE*j-CHESS_RADIUS,
                                         MARGIN+BLOCK_SIZE*i-CHESS_RADIUS,
                                         CHESS_RADIUS*2,CHESS_RADIUS*2),black);

            }
            else if(game->gameMapVec[i][j]==-1)
            {
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                //                painter.drawEllipse(MARGIN+BLOCK_SIZE*j-CHESS_RADIUS,
                //                                    MARGIN+BLOCK_SIZE*i-CHESS_RADIUS,
                //                                    CHESS_RADIUS*2,CHESS_RADIUS*2);
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
                if(flag==QMessageBox::No)
                {
                    game->startGame(game_type);
                    game->gameStatus = PLAYING;
                    game->gameMapVec[8][7]=1;
                    update();
                    game->playerFlag=false;
                    return;

                }

                else {
                    game->startGame(game_type);
                    game->gameStatus = PLAYING;

                }

            }

        }

    }
}

void AiBattle::initGame()
{

    initAIGame();
}

void AiBattle::initAIGame()
{
    game_type =AI;
    game->gameStatus = PLAYING;
    game->startGame(game_type);
    update();
}

void AiBattle::chessOneByPerson()
{



    if(clickPosRow!=-1 && clickPosCol!=-1 &&
            game->gameMapVec[clickPosRow][clickPosCol]==0)
    {

        playclickCol=clickPosCol;
        playclickRow=clickPosRow;
        game->actionByperson(clickPosRow,clickPosCol);


        //音效
        //TODO:


        update();

    }
}

void AiBattle::mouseMoveEvent(QMouseEvent *event)
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


    }
    update();

}

void AiBattle::mouseReleaseEvent(QMouseEvent *event)
{
    if(selectPos == false){
        return;
    }
    else{
        selectPos = false;
    }
    //由人下棋



    chessOneByPerson();
    if(game_type == AI)
    {
        //AI 下棋
        QTimer::singleShot(AI_THINK_TIME,this,SLOT(chessOneByAI()));

    }

}

void AiBattle::chessOneByAI()
{
    game->actionByAI(clickPosRow,clickPosCol);
    update();
}

void AiBattle::on_pushButton_clicked() //选棋
{
    if(flag==QMessageBox::No)
    {

        game->startGame(game_type);
        game->gameStatus = PLAYING;
        update();
        game->gameMapVec[8][7]=1;
        update();
        game->playerFlag=false;
        return;
    }
    else
    {
        game->startGame(game_type);
        game->gameStatus = PLAYING;
        update();

    }

}

void AiBattle::on_pushButton_2_clicked() //悔棋
{
    aiclickPosCol=game->aiCol;
    aiclickPosRow=game->aiRow;
    int sum=0;
    for(int i=0;i<BOARD_GRAD_SIZE;i++)
    {
        for(int j=0;j<BOARD_GRAD_SIZE;j++)
        {

            if(game->gameMapVec[i][j]!=0)
                sum++;
        }
    }
    if(sum==0)
    {
        QMessageBox::information(this,"提示","开局不能悔棋");
        return;


    }
    else if(flag==QMessageBox::No)
    {
        if(sum==1)
            QMessageBox::information(this,"提示","开局不能悔棋");
        else
        {
            game->gameMapVec[playclickRow][playclickCol]=0;
            game->gameMapVec[aiclickPosRow][aiclickPosCol]=0;
            update();

        }
        return;

    }
    else
    {

        if((game->gameMapVec[playclickRow][playclickCol])==0 && (game->gameMapVec[aiclickPosRow][aiclickPosCol]==0)&&(sum!=0))
        {

            QMessageBox::information(this,"提示","只能悔一步棋");
            return;
        }

        game->gameMapVec[playclickRow][playclickCol]=0;
        game->gameMapVec[aiclickPosRow][aiclickPosCol]=0;
        update();

    }


}

void AiBattle::on_pushButton_3_clicked()  //返回
{
    this->hide();
    Widget *w = new Widget;
    w->show();


}
