#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "console.h"
#include <QTimer>
#include "qextserialenumerator.h"
#include "qextserialport.h"
#include "hled.h"
#include <QThread>
#include <QList>
#include "define.h"
#include "tranceiverlora.h"
#include <QNetworkAccessManager>
#include "QHash"
#include "QTime"
#include "qtmosq.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //Q_INVOKABLE void mySendCommand(int, int);

protected:
    //void changeEvent(QEvent *e);

private:
    QString getIp(int);
    void loadHtmlPage();
    void errorOccured(QString);
    void showMarker(int);
    bool checkJoin(int);
    int getIndexSensor(int);
    int getIndexMarker(int);
    QString findNearestNode();
    void updateListSensor();
    void delay(int secondsToWait);
    int tPredict(QTime t1, QTime t2, double d1, double d2);
    double distance(double lat1, double lng1, double lat2, double lng2);
    double bearing(double lat1, double lng1, double lat2, double lng2);
    double deg2rad(double deg);
    void WriteTextAppend(QString, QString);
    void findLastPoint(QString &lat2, QString &lng2, double lat1, double lng1, double dis, double bear);
    void setDate();

    //for UAV
   // void SetupPortSerial();

    void AlwaysOpenPortLR();
signals:
    void readySend();

private slots:
    void on_btnPrint_clicked();
    void on_btnClear_clicked();
    
   
//     on_pushButton_2_clicked();
    //Q_INVOKABLE void sendCommand(int, int);
  
    void onImageReceived(QString);
   
    //Tranceiver
	void onreceivedDataLR(QString);
   
    void oncompleteLux(QString);
    void onTempAndHumLR(QString);
    void oncompleteMois(QString);
    void oncompleteDust(QString);


   
  
    //void SetupSerialPort();
  
    //Push button
    void on_btnExit_clicked();

    //////////////
    
    //mqtt
    void sendMqtt();
    void mqttConnect();
    void connectEnabled();
    void subscribePressed();
    void subscribed();
    void getSubMQTT(QString);
    void sendMqttData(int, double, QString);
   
    void onTempHumi(int, double, double);
    void onLR(int,int);

    

    void SendAIDD(int , double );
   
    void onLux_t(int, double);
    void onMois_t(int, double);
    void onST(QString);
//    void onLRHTD(int,int,int);
    //Graph
    void makePlot();


private:
    Ui::MainWindow *ui;
    Console *console;
    QThread *thread_gps, *thread_tranceiver, *thread_receivefromweb,*thread_lora;
    QString m_organizationName, m_appName;
    bool gpsStarted, tranceiverStarted, gpsDataReceived, sended, detectedFlag, receivedFlag, loraStarted;
    QList<bool> ListSensorStt;
    QList<double> ListSensorArg;
    QList<double> ListSensorTemp;
    QList<double> ListSensorDeltaTemp;
    tranceiverlora * lora;
    QNetworkAccessManager *http1;
    int *joinedMac;
    QString *joinedAddress;
    int indexJoined;
    QHash <QString, QString> hash;
    QHash <QString, QTime> movDetectTime;
    QHash <QString, double> maxTemp;
    double d1, d2;
    double a_test, b_test;
    //DbManager database;
     /*mqtt */
    qtmosq* mosq = NULL;
    //

};

#endif // MAINWINDOW_H
