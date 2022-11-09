#include "server.h"
#include "ui_server.h"
#include <QHostInfo>
#include <QDebug>


#include <QNetworkInterface>
#include <QMessageBox>


server::server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::server)
{
    ui->setupUi(this);
    port = 8000;
    ip = "8.130.36.138";

    tcpSocket = nullptr;
    tcpServer = nullptr;
    tcpServer = new QTcpServer(this);

    ui->comboBox_clinet->setEditText("客户端");

}

server::~server()
{
    delete ui;
}

void server::on_pushButton_connect_clicked()
{
    QString temp;
    if(tcpServer->isListening()==true)//服务器正在监听
    {
        if(tcpSocket != nullptr)//判断是否有客户端连接
        {
            for(int i=tcpClientList.length()-1;i>=0;i--)//断开所有连接应从最后开始断开
            {
                temp =QString("[%1:%2]:退出").arg(tcpClientList.at(i)->peerAddress().toString()).arg(tcpClientList.at(i)->peerPort());
                ui->textEdit_GetText->append(temp);
                tcpClientList.at(i)->disconnectFromHost();//会触发disconnected信号
                tcpClientList.removeAt(i);

            }
            if(tcpClientList.isEmpty()==true)
            {
                tcpServer->close();             //关闭服务器
                tcpSocket=nullptr;
                ui->textEdit_GetText->append("停止监听成功");
                ui->pushButton_connect->setText("连接");
                disconnect(tcpServer,&QTcpServer::newConnection,this,&server::NewConnection);
            }
        }
        else
        {
            QString str = "当前没有连接,可重新监听";
            tcpServer->close();
            ui->textEdit_GetText->append(str);
            ui->pushButton_connect->setText("连接");
            disconnect(tcpServer,&QTcpServer::newConnection,this,&server::NewConnection);
        }
    }
    else
    {
        //获取IP和端口

        if(ui->lineEdit_Ip->text() != "")
            ip = ui->lineEdit_Ip->text();
        ui->lineEdit_Ip->setText(ip);
        if(ui->lineEdit_prot->text()=="")
        {
            port=8000;
        }
        else
        {
            port = quint16(ui->lineEdit_prot->text().toUInt());
        }
        ui->lineEdit_prot->setText(QString::number(port));
        //if(true == tcpServer->listen(QHostAddress::Any,port))//监听所有Ip
        if(true == tcpServer->listen(QHostAddress::AnyIPv4,port))
        {
            ui->textEdit_GetText->append("开始监听");
            ui->pushButton_connect->setText("停止");
            connect(tcpServer,&QTcpServer::newConnection,this,&server::NewConnection);
        }
        else
        {
            ui->textEdit_GetText->append("监听失败");
            return ;
        }
    }
}

void server::NewConnection()
{
    //取出建立好的套接字


    tcpSocket = tcpServer->nextPendingConnection();
    //获取对方Ip和端口
    QString cip = tcpSocket->peerAddress().toString();
    quint16 prot = tcpSocket->peerPort();
    QString temp= QString("%1.%2:成功连接").arg(cip).arg(prot);
    ui->textEdit_GetText->append(temp);
    //添加到界面以及socket列表中
    temp=temp.split(":")[0];
    ui->comboBox_clinet->addItem(temp);
    tcpClientList.append(tcpSocket);
    if(tcpClientList.size()>1)
    {

        ui->textEdit_GetText->append("游戏人数已满，停止监听");
        ui->pushButton_connect->setText("连接");
        for(int i=0;i<tcpClientList.length();i++)
        {
            //给所有socket发送数据
            QString str="ready";
            tcpClientList[i]->write(str.toUtf8().data());
        }
        tcpServer->close();
        disconnect(tcpServer,&QTcpServer::newConnection,this,&server::NewConnection);
        tcpClientList[0]->write(sign.toUtf8().data());
        tcpClientList[0]->write(color.toUtf8().data());
    }

    connect(tcpSocket,&QTcpSocket::readyRead,this,&server::ReadyRead);
    connect(tcpSocket,&QTcpSocket::disconnected,this,&server::disconnectedSlot);
}

void server::disconnectedSlot()
{
    tcpSocket = (QTcpSocket *)this->sender();//得到发出信号的客户端
    QString temp =QString("%1.%2:退出").arg(tcpSocket->peerAddress().toString()).arg(tcpSocket->peerPort());
    ui->textEdit_GetText->append(temp);
    temp=temp.split(":")[0];
    //从链表中删除
    for(int i=0;i<tcpClientList.length();i++)
    {
        if(tcpClientList.at(i)->peerAddress() == tcpSocket->peerAddress())
        {
            if(tcpClientList.at(i)->peerPort() == tcpSocket->peerPort())
            {
                if(ui->comboBox_clinet->itemText(i) == temp)
                    ui->comboBox_clinet->removeItem(i);
                if(ui->comboBox_clinet->count()==0)
                    ui->comboBox_clinet->setCurrentText("客户端");
                tcpClientList.removeAt(i);
                break;
            }
        }
    }
    if(ui->comboBox_clinet->itemText(0)=="客户端")
    {
        ui->textEdit_GetText->append("开始监听");
        ui->pushButton_connect->setText("停止");
        connect(tcpServer,&QTcpServer::newConnection,this,&server::NewConnection);
    }
}

void server::ReadyRead()
{

    //获取对方的IP和端口
    //QString cip = tcpSocket->peerAddress().toString();
    //quint16 prot = tcpSocket->peerPort();
    tcpSocket = (QTcpSocket *)this->sender();//得到发出信号的客户端
    QByteArray array = tcpSocket->readAll();
    ui->textEdit_GetText->append(array);
    QString pos=array;
    if((pos.contains("&",Qt::CaseSensitive)))//表示发送信息
    {
        for(int i=0;i<tcpClientList.length();i++)
        {
            //给所有socket发送数据
            tcpClientList[i]->write(pos.toUtf8().data());
        }

    }
    else  if(pos.contains("|",Qt::CaseSensitive))
    {
        if(tcpClientList.at(0)->peerAddress()==tcpSocket->peerAddress()&&tcpClientList.at(0)->peerPort()==tcpSocket->peerPort()) //判断是否是第一位玩家
        {
            if(isTurnyou)
            {
                ui->textEdit_GetText->append("ip1: 向白棋发送黑棋坐标："+sign+pos);
                tcpClientList[1]->write(sign.toUtf8().data());
                tcpClientList[1]->write(pos.toUtf8().data());
                isTurnyou=!isTurnyou;
            }
            else
            {
                ui->textEdit_GetText->append("ip1:向黑棋发送白棋坐标："+sign+pos);
                tcpClientList[0]->write(sign.toUtf8().data());
                tcpClientList[0]->write(pos.toUtf8().data());
                isTurnyou=!isTurnyou;
            }
        }
        if(tcpClientList.at(1)->peerAddress()==tcpSocket->peerAddress()&&tcpClientList.at(1)->peerPort()==tcpSocket->peerPort()) //判断是否是第二位玩家
        {
            if(!isTurnyou)
            {
                ui->textEdit_GetText->append("ip2:向黑棋发送白棋坐标："+sign+pos);
                tcpClientList[0]->write(sign.toUtf8().data());
                tcpClientList[0]->write(pos.toUtf8().data());
                isTurnyou=!isTurnyou;
            }
            else
            {
                ui->textEdit_GetText->append("ip2:向白棋发送白棋坐标："+sign+pos);
                tcpClientList[1]->write(sign.toUtf8().data());
                tcpClientList[1]->write(pos.toUtf8().data());
                isTurnyou=!isTurnyou;
            }
        }
        else
        {
            ui->textEdit_GetText->append("服务器异常");
            return;
        }
    }
    else if((pos.contains("%n",Qt::CaseSensitive)))//发送用户名
    {
        if(tcpClientList.size()>1)
        {

            if(tcpClientList.at(0)->peerAddress()==tcpSocket->peerAddress()&&tcpClientList.at(0)->peerPort()==tcpSocket->peerPort()) //判断是否是第一位玩家
            {
                ui->textEdit_GetText->append("ip1: 向白棋发送用户名："+pos);
                tcpClientList[1]->write(pos.toUtf8().data());
            }

            if(tcpClientList.at(1)->peerAddress()==tcpSocket->peerAddress()&&tcpClientList.at(1)->peerPort()==tcpSocket->peerPort()) //判断是否是第二位玩家
            {
                ui->textEdit_GetText->append("ip2:向黑棋发送用户名："+pos);
                tcpClientList[0]->write(pos.toUtf8().data());
            }
        }
    }
    else if(pos.contains("regret",Qt::CaseSensitive))
    {
        if(tcpClientList.at(0)->peerAddress()==tcpSocket->peerAddress()&&tcpClientList.at(0)->peerPort()==tcpSocket->peerPort()) //判断是否是第一位玩家
        {
            if(isTurnyou)
            {
                ui->textEdit_GetText->append("ip1: 向白棋发送黑棋坐标："+pos);
                tcpClientList[1]->write(pos.toUtf8().data());
            }
            else
            {
                ui->textEdit_GetText->append("ip1:向黑棋发送白棋坐标："+pos);
                tcpClientList[0]->write(pos.toUtf8().data());
            }
        }
        if(tcpClientList.at(1)->peerAddress()==tcpSocket->peerAddress()&&tcpClientList.at(1)->peerPort()==tcpSocket->peerPort()) //判断是否是第二位玩家
        {
            if(!isTurnyou)
            {
                ui->textEdit_GetText->append(pos);
                tcpClientList[0]->write(pos.toUtf8().data());

            }
            else
            {
                ui->textEdit_GetText->append("ip2:向白棋发送白棋坐标："+pos);
                tcpClientList[1]->write(pos.toUtf8().data());
            }
        }
    }
    else if(pos.contains("Surrender",Qt::CaseSensitive))
    {
        if(tcpClientList.at(0)->peerAddress()==tcpSocket->peerAddress()&&tcpClientList.at(0)->peerPort()==tcpSocket->peerPort()) //判断是否是第一位玩家
        {
            if(isTurnyou)
            {
                ui->textEdit_GetText->append("ip1: 向白棋发送投降："+pos);
                tcpClientList[1]->write(pos.toUtf8().data());
                isTurnyou=true;
            }
            else
            {
                ui->textEdit_GetText->append("ip1:向黑棋发送白棋投降："+pos);
                tcpClientList[0]->write(pos.toUtf8().data());
                isTurnyou=true;
            }
        }
        if(tcpClientList.at(1)->peerAddress()==tcpSocket->peerAddress()&&tcpClientList.at(1)->peerPort()==tcpSocket->peerPort()) //判断是否是第二位玩家
        {
            if(!isTurnyou)
            {
                ui->textEdit_GetText->append("ip1:向黑棋发送白棋投降："+pos);
                ui->textEdit_GetText->append(pos);
                tcpClientList[0]->write(pos.toUtf8().data());
                isTurnyou=true;

            }
            else
            {
                ui->textEdit_GetText->append("ip2:向白棋发送白棋投降："+pos);
                tcpClientList[1]->write(pos.toUtf8().data());
                isTurnyou=true;
            }
        }

    }
}



void server::on_pushButton_clearGetText_clicked()
{
    ui->textEdit_GetText->clear();
}

void server::on_pushButton_clearSend_clicked()
{
    ui->textEdit_sendText->clear();
}

void server::on_pushButton_send_clicked()
{
    if(tcpSocket == nullptr)
    {
        ui->textEdit_sendText->append("当前无连接");
        return;
    }
    //获取编辑区内容
    QString str = ui->textEdit_sendText->toPlainText();
    for(int i=0;i<tcpClientList.length();i++)
    {
        //给所有socket发送数据
        tcpClientList[i]->write(str.toUtf8().data());
    }
}

void server::on_pushButton_appoint_clicked()
{
    //获取编辑区内容
    QString str = ui->textEdit_sendText->toPlainText();
    QString temp,temp1;
    if(ui->comboBox_clinet->count() != 0)
    {
        for(int i = 0; i<tcpClientList.length(); i++)
        {
            temp1 = ui->comboBox_clinet->currentText();
            temp = ui->comboBox_clinet->itemText(i);
            if(temp == temp1)
            {
                tcpClientList[i]->write(str.toUtf8().data());
            }
        }
    }
    else
    {
        ui->textEdit_sendText->append("当前无连接");
        return;
    }
}
