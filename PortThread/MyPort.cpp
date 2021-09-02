#include "MyPort.h"
#include <QDebug>
#include <QThread>
MyPort::MyPort(QObject *parent) : QObject(parent)
{
    //()
    mythread = new QThread();
    m_port =new QSerialPort();
    connect(m_port,&QSerialPort::readyRead,this,&MyPort::ReadyReceive);
    this->moveToThread(mythread);
    m_port->moveToThread(mythread);
    mythread->start();
}

MyPort::~MyPort()
{
    m_port->close();
    mythread->quit();
    mythread->wait();
    m_port = nullptr;
    mythread = nullptr;
}

void MyPort::parseData(QByteArray arr)
{
    if(arr.startsWith("GPGGA")){

        QList<QByteArray> gpsByteArrays = arr.split(','); //从内存中获取卫星接收机发送的信号,以逗号分割保存在Array中

        int  gpsLat_1 = static_cast<int>(gpsByteArrays.at(2).toDouble()/100); //除100留下度数
        double gpsLat_2 = (gpsByteArrays.at(2).toDouble() - gpsLat_1 * 100)/60; //分数转度数需要除60
        gpsLat=gpsLat_1 + gpsLat_2; //纬度（得到纯度数表达式）

        int gpsLong_1 = static_cast<int>(gpsByteArrays.at(4).toDouble()/100);
        double gpsLong_2 = (gpsByteArrays.at(4).toDouble()-gpsLong_1 * 100)/60.0;
        gpsLong = gpsLong_1 + gpsLong_2; //经度

    }else if(arr.startsWith("GPVTG")){
        QList<QByteArray> gpsByteArrays = arr.split(','); //从内存中获取卫星接收机发送的信号,以逗号分割保存在Array中
        if(gpsByteArrays.size()<=8){
            return;
        }
        speedk = gpsByteArrays.at(7).toDouble();//  km/h
    }else if(arr.startsWith("$ANGLE")){
        //方向+角度值，如“12047”，‘1’代表左偏，“2047”代表角度值，2047/100可以得到真正的角度值
        QList<QByteArray> gpsByteArrays = arr.split(',');
        QByteArray data = gpsByteArrays.at(1);
        double getangle =data.mid(1,5).toDouble()/100.0;
        int LeftOrRight = data.mid(0,1).toInt(); //1左偏，0右偏
        if(LeftOrRight==1){
            angle = -getangle;
        }else {
            angle = getangle;
        }
        //qDebug()<<"方向和角度"<<LeftOrRight<<getangle;

    }else if(arr.startsWith("PSAT,HPR")){
        QList<QByteArray> gpsByteArrays = arr.split(','); //从内存中获取卫星接收机发送的信号,以逗号分割保存在Array中
        headAngle =gpsByteArrays.at(3).toDouble();

    }else if(arr.startsWith("GPZDA")){
        return;
    }
}

void MyPort::ReadyReceive()
{

    qa=m_port->readAll();
    foreach(char b,qa){
        if(b != '\n'){
            m_arr.append(b);
        }else{
            m_arr.append('\n');
            if(m_arr.contains("$GPGGA")){
                QList<QByteArray> arr=m_arr.split('$');
                for(int i=0;i<arr.size();++i){
                    parseData(arr.at(i));
                }
                qDebug()<<QThread::currentThreadId()<<QThread::currentThread();
                emit sendGpsData(gpsLong,gpsLat,speedk,headAngle);
                m_arr.clear();
            }else if(m_arr.contains("$ANGLE")){
                parseData(m_arr);
                qDebug()<<QThread::currentThreadId()<<QThread::currentThread();
                emit sendSensorAngle(angle);
            }

        }
    }

}

void MyPort::startConnect(QString portName)
{
    if(portName == "None"){
        return;
    }
    m_port->close(); //先关闭串口实例化对象
    m_port->setPortName(portName); //设定连接串口
    m_port->open(QIODevice::ReadWrite); //串口读写打开
    m_port->setBaudRate(QSerialPort::Baud115200);
    m_port->setDataBits(QSerialPort::Data8);//设置数据位8
    m_port->setParity(QSerialPort::NoParity); //无校验位
    m_port->setStopBits(QSerialPort::OneStop);//停止位设置为1
    m_port->setFlowControl(QSerialPort::NoFlowControl);//设置为无流控制
}


