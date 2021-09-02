#ifndef MYPORT_H
#define MYPORT_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
class MyPort : public QObject
{
    Q_OBJECT
public:
    explicit MyPort(QObject *parent = nullptr);
    ~MyPort();
private:
    QByteArray qa,m_arr;
    QSerialPort* m_port;
    QThread* mythread;

    void parseData(QByteArray arr);

    double gpsLat=0,gpsLong=0,speedk=0,angle=0,headAngle=0;
public slots:
    void startConnect(QString portName);
private slots:
    void ReadyReceive();


signals:
    void sendGpsData(double,double,double,double);
    void sendSensorAngle(double);
};

#endif // MYPORT_H
