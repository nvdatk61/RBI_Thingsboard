#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>
#include <QtWebKit>
#include <QThread>
#include <QFile>
#include <QTime>
#include "filedata.h"
#include <QFileDialog>
#include "data.h"
#include "math.h"
#include "QDir"
#include "QCoreApplication"
#include "QEventLoop"
#include "QPrintDialog"
#include "QPrinter"
#include "QTextCursor"
#include "qtmosq.h"
#include "readfile.h"
#include "confmqtt.h"
#include <stdlib.h>
#include <iostream>
//#include <wiringPi.h>

//class WebPage : public QWebPage
//{
//    public:  WebPage(QObject * p = 0) : QWebPage(p) {}
//    private: QString userAgentForUrl(const QUrl&) const { return "Chrome/1.0"; }
//};
confmqtt xx;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    joinedMac = new int[20];
    joinedAddress = new QString[20];
    indexJoined = 0;
    http1 = new QNetworkAccessManager(this);
    console = new Console;
    console->setEnabled(true);
    console->setLocalEchoEnabled(true);
    ui->layConsole->addWidget(console);
   
    //init Data
    hash["62"] = "05";
    hash["61"] = "08";
    d1 = 20, d2 =20;
    receivedFlag = false;

    //sendMqttData
    //Lora
    lora = new tranceiverlora();
    loraStarted =false;
    thread_lora =new QThread();
    lora->moveToThread(thread_lora);
    connect(lora, SIGNAL(workRequestedLR()), thread_lora, SLOT(start()));
    connect(thread_lora,SIGNAL(started()),lora,SLOT(doWorkLR()));
    connect(lora, SIGNAL(receivedDataLR(QString)), SLOT(onreceivedDataLR(QString)));;
    connect(lora, SIGNAL(tempAndHum(QString)), SLOT(onTempAndHumLR(QString)));
    connect(lora,SIGNAL(completeDust(QString)), SLOT(oncompleteDust(QString)));
    connect(lora,SIGNAL(sendDust(int,double)),SLOT(SendAIDD(int,double)));
    connect(lora,SIGNAL(completeST(QString)),SLOT(onST(QString)));
 

    connect(ui->actionQuit, SIGNAL(triggered()), SLOT(close()));

    setWindowTitle(tr("GATEWAY IOT"));

    setDate();

    //<Chinh sua code cho UAV>

    /*
     * thiet dat thoi gian time_out cho UAV gui lenh lay du lieu
     */
    QImage image("/home/flytos/GATEWAY_RBI/build/WSAN/logo.png");
    ui->lblImage->setPixmap (QPixmap::fromImage (image));
   // int time_out = 30000;
    //fileName = "/home/lab411/Desktop/LogfileUAV/logfile_" + QDate::currentDate().toString() + "_" + QTime::currentTime().toString();
    //AlwaysOpenPort();
    AlwaysOpenPortLR();
    //SetupPortSerial();

//    </Chinh sua code cho UAV>
    lib_init();
    mqttConnect();
    subscribePressed();
    qDebug()<<"Setup Success";
}
// load output.txt



/*
 * Ham ghi du lieu thu thap duoc vao file txt. Luu o thu muc ~/Desktop/LogfileUAV
 * Duoc su dung tai cac ham onNodeJoin, onTempAndHum, ~MainWindow
 *
 */
// void MainWindow::WriteDatatoLogfile(QString data)
// {
//     QFile file(fileName);
//     file.open(QIODevice::WriteOnly | QIODevice::Append);
//     QTextStream out(&file);
//     out<<data;
//     file.close();
// }

MainWindow::~MainWindow()
{
    //khi dong chuong trinh. Ket thuc ghi file
    //WriteDatatoLogfile("\n---------------End File----------");
    mosq->mosquittopp::disconnect();
    mosq->loop_stop();
    lib_cleanup();
    delete mosq;
    delete ui;
}


/* nxt code
 * auto connect mqtt
 *
*/
void MainWindow::mqttConnect()
{
    char* s,*access;
    QByteArray ba = xx.hostMqtt.toLatin1();
    s = ba.data();
    ba = xx.accessToken.toLatin1();
    access=ba.data();
    
    mosq = new qtmosq();
    mosq->username_pw_set(access);

    connect(mosq, SIGNAL(connected()), this, SLOT(connectEnabled()));
    connect(mosq, SIGNAL(subscribed()), this ,SLOT(subscribed()));
    connect(mosq, SIGNAL(messageReceived(QString)), this, SLOT(getSubMQTT(QString)));
    mosq->connect_async(s,xx.portMqtt);
    mosq->loop_start();
}

void MainWindow::connectEnabled()
{
    console->insertPlainText("connect to MQTT !!!\n");
}

void MainWindow::sendMqtt()
{
    QByteArray topic = xx.topic1.toAscii();
    QString payload = "{";
    payload += "\"}";
    QByteArray datasend=payload.toLocal8Bit();
    mosq->publish(mosq->getMID(),topic.data(),datasend.size(),datasend.data(),2,false);
    //console->insertPlainText("sent data to server!!");
}

//NXT
// THEM CAC DONG SAU VAO CODE
// THEM KHAI BAO HAM TRONG FILE mainwindow.h
void MainWindow::subscribePressed()
{
    QString topicSub="v1/devices/me/rpc/request/+";
    QByteArray topic = topicSub.toLocal8Bit();

    mosq->subscribe(mosq->getMID(), topic.data(), 2);
}

void MainWindow::subscribed()
{
    console->insertPlainText("Subscribe Complete!!\n");

}
void MainWindow::getSubMQTT(QString mess)
{
    QStringList cmd=mess.split('"');
    if (cmd[3]=="PUMP") {
        if (cmd[6].mid(1,cmd[6].size()-2)=="true") {
            console->insertPlainText("Request On Pump from Server\n\r");//thay chu hien thi ra man hinh
            //viet them code dieu khien bom qua lora
            onLR(90,5);

        }
        else {
            console->insertPlainText("Request Off Pump from Server\n\r");//thay chu hien thi ra man hinh
            //viet them code dieu khien bom qua lora
            onLR(90,6);
        }
    } else if(cmd[3]=="LIGHT") {
        if(cmd[6].mid(1,cmd[6].size()-2)=="true") {
            console->insertPlainText("Request On Light from Server \n");//thay chu hien thi ra man hinh
        //viet them code dieu khien bom qua lora
            onLR(90,3);
        }
        else {
            console->insertPlainText("Request Off Light from Server\n");//thay chu hien thi ra man hinh
            //viet them code dieu khien bom qua lora
            onLR(90,4);
        }
    }
}

void MainWindow::onreceivedDataLR(QString data)
{   
    
 //console->insertPlainText(data);
}
//ket thuc them

void MainWindow::sendMqttData(int mac,double data,QString type)
{
	mac = 50;
    QString model = "T1000";
    QString name = "Data_RBI_";
    QString tmp;
    name.append(QString::number(mac));
    QDateTime current = QDateTime::currentDateTime();
    uint timestame = current.toTime_t();
    QString payload = "{\"";
    payload += name;
    payload += "\": [{\"ts\":";
    payload += QString::number(timestame);payload+="000,\"values\":";
    payload += "{\""+ type+ "\":";
    payload += QString::number(data);
    payload += "}}]}";

    tmp += type + " : ";
    tmp += QString::number(data); 
    QByteArray datasend=payload.toLocal8Bit();
    QByteArray topic= xx.topic2.toAscii();
    mosq->publish(mosq->getMID(),topic.data(),datasend.size(),datasend.data(),2,false);
    //console->insertPlainText("Sent data to the server of Thingsboard!!! \r\n");
    console->insertPlainText(tmp + "\n");
}

//end code mqtt

void MainWindow::loadHtmlPage()
{
    QFile htmlFile(":/html/index.html");
    if( ! htmlFile.open(QFile::ReadOnly) )
    {
        errorOccured( "I can't read html file" );
        return;
    }
}

void MainWindow::errorOccured(QString error)
{
    QMessageBox::warning(this, trUtf8("Error"), error);
}

void MainWindow::on_btnExit_clicked()
{
    this->close();
}

void MainWindow::AlwaysOpenPortLR()
{
    readfile x;
    if (loraStarted) {
        if (lora->port->isOpen()) {
            lora->port->close();
            console->insertPlainText("\n-------------- Lora Closed -------------\n");
        } else {
            if (lora->port->portName().isEmpty()) lora->port->setPortName(x.LORA_PORT_DEFAULT);
            QSettings settings(m_organizationName, m_appName);
            QString tmp = settings.value("TranceiverBaudrate").toString();
            if (tmp.isEmpty()) lora->port->setBaudRate((BaudRateType)x.LORA_BAUDRATE_DEFAULT);
            lora->port->open(QIODevice::ReadWrite);
            console->insertPlainText("\n------------- Port Lora Data Opened ------------\n");
        }
    } else {
        lora->requestWorkLR();
        console->insertPlainText("\n-------------- Port Lora Data Opened -------------\n");
        loraStarted = true;
    }
}


void MainWindow::onImageReceived(QString FileName)
{
    QImage image(FileName);
    ui->lblImage->setPixmap (QPixmap::fromImage (image));
}


void MainWindow::onTempAndHumLR(QString data)
{
    QString tmp;
    QStringList data_lst = data.split(":");
    console->moveCursor(QTextCursor::End);
//    FileData file(DATA_PATH);
    QString mac = data_lst.value(0);

    tmp = "\n["+QTime::currentTime().toString()+"] " +"Collect data from sensor\r\n";

    tmp += "Data_RBI_";
    tmp += data_lst.value(0);
    tmp += "\nTemperature: ";
    tmp += data_lst.value(1);
    tmp += " oC\nHumiumidity: ";
    tmp += data_lst.value(2);
    tmp += " %";
    bool ok;
    int mac1=data_lst.value(0).toInt(&ok,10);
    double temp =data_lst.value(1).toDouble(&ok);
    double humi =data_lst.value(2).toDouble(&ok);
   // WriteDatatoLogfile(tmp);
    console->insertPlainText(tmp + "\n");
	double eq = rand() % 50 + 100;
    double retemp = rand() % 50 + 100;
	double com = rand() % 10 + 50;
	double dia = rand() % 1 + 50;
	double maxtemp = rand() % 50 + 150;
	double maxpress = rand() % 1 + 5;
	double flowrate = rand() % 10 + 50;
	
    sendMqttData(mac1, eq, "Equipment Volume");
    sendMqttData(mac1, retemp, "Required Temperature");
	sendMqttData(mac1, com, "Component Volume");
	sendMqttData(mac1, dia, "Nominal Diameter");
	sendMqttData(mac1, maxtemp, "Max Temperature");
	sendMqttData(mac1, maxpress, "Max Pressure");
	sendMqttData(mac1, flowrate, "Flow Rate"); 
    receivedFlag = true;
//    emit readySend();
}



void MainWindow::oncompleteLux(QString data)
{
    QString tmp;
    QStringList data_lst = data.split(":");
    console->moveCursor(QTextCursor::End);
//    FileData file(DATA_PATH);
    QString mac = data_lst.value(0);
    //int index  = getIndexMarker(mac.toInt());
    //ListSensor[index]->cur_temp = data_lst.value(2).toDouble();

    tmp = "\n["+QTime::currentTime().toString()+"] " +"information of luminance from sensor ";
    tmp += data_lst.value(0);
    tmp += ", Address Ip ";
    tmp += data_lst.value(1);
    tmp += "\nLuminance:        ";
    tmp += data_lst.value(2);
    tmp += "\n";

    //WriteDatatoLogfile(tmp);
    console->insertPlainText(tmp + "\n");
    receivedFlag = true;
}

void MainWindow::oncompleteMois(QString data)
{
    QString tmp;
    QStringList data_lst = data.split(":");
    console->moveCursor(QTextCursor::End);
//    FileData file(DATA_PATH);
//    QString mac = data_lst.value(0);
    //int index  = getIndexMarker(mac.toInt());
    //ListSensor[index]->cur_temp = data_lst.value(2).toDouble();

    tmp = "\n["+QTime::currentTime().toString()+"] " +"information of soil moisture from sensor ";
    tmp += data_lst.value(0);
    tmp += "soil moisture :    ";
    tmp += data_lst.value(1);
    tmp += "%\n";

    //WriteDatatoLogfile(tmp);
    console->insertPlainText(tmp + "\n");
    receivedFlag = true;
}


void MainWindow::on_btnClear_clicked()
{
    console->clear();
}


QString MainWindow::findNearestNode(){
    readfile x;
    FileData file(x.DATA_PATH);
    int N = file.length();
    double *dis = new double[N];
    for(int i = 0;i < N; i++){
        QString line = file.readLine(i+1);
        QStringList lst = line.split(",");
        double Nodelat = QString (lst.value(1)).toDouble();
        double Nodelng = QString (lst.value(2)).toDouble();
        dis[i] = sqrt(pow((DATA::lat.toDouble() - Nodelat)*110.574, 2) + pow((DATA::lng.toDouble()*cos(DATA::lat.toDouble()) - Nodelng*cos(Nodelat))*111.320, 2));
        //dis[i]  = dis()
    }
    int index = 0;
    for(int j = 0; j < N; j++){
        if(dis[j] < dis[index])
            index = j;
    }
    QString str = file.readLine(index + 1);
    QString mac = QStringList(str.split(",")).value(0);
    delete dis;
    return mac;
}



void MainWindow::delay(int secondsToWait){
    QTime dieTime = QTime::currentTime().addSecs(secondsToWait);
    while(QTime::currentTime() < dieTime){
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
}

int MainWindow::tPredict(QTime t1, QTime t2, double d1, double d2) {
    int time = fabs(t1.secsTo(t2));
    double v = d1/time;
    return int(d2/v);
}

double MainWindow::distance(double lat1, double lng1, double lat2, double lng2){
    double R = 6371;
    double dLat = deg2rad(fabs(lat2 - lat1));
    double dLng = deg2rad(fabs(lng2 - lng1));
    double a = pow(sin(dLat/2),2) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * pow(sin(dLng/2), 2);
    double b = 2 * atan2(sqrt(a), sqrt(1-a));
    return R * b * 1000;
}

double MainWindow::deg2rad(double deg){
   return deg * M_PI/180;
}

// void MainWindow::WriteTextAppend(QString FileName, QString Text)
// {
//     QFile file(FileName);
//     if(file.open(QIODevice::Append))
//     {
//         QTextStream stream(&file);
//         stream << Text << endl;
//         file.close();
//     }
// }


void MainWindow::on_btnPrint_clicked()
{
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    dialog.setWindowTitle(tr("Print Document"));
    QTextCursor c = QTextCursor(console->document());
    console->textCursor() = c;
    if(c.hasSelection())
        dialog.addEnabledOption(QAbstractPrintDialog::PrintSelection);
    if(dialog.exec() != QDialog::Accepted){
        return;
    }
    console->document()->print(&printer);
}



double MainWindow::bearing(double lat1, double lng1, double lat2, double lng2){
    double a = cos(deg2rad(lat2)) * sin(deg2rad(lng2 -lng1));
    double b = cos(deg2rad(lat1))*sin(deg2rad(lat2)) - sin(deg2rad(lat1))*cos(deg2rad(lat2))*cos(deg2rad(lng2-lng1));
    a_test = a;
    b_test = b;
    return atan2(a,b);
}

void MainWindow::findLastPoint(QString &lat2, QString &lng2, double lat1, double lng1, double dis, double bear){
    lat1 = deg2rad(lat1);
    lng1 = deg2rad(lng1);
    double ad = (dis/6371000);
    double m_lat = asin(sin(lat1)*cos(ad) + cos(lat1)*sin(ad)*cos(bear));
    double m_lng = lng1 + atan2(sin(bear)*sin(ad)*cos(lat1), cos(ad) - sin(lat1)*sin(m_lat));
    m_lat *= 180/M_PI;
    m_lng *= 180/M_PI;
    lat2 = QString::number(m_lat,'f',6);
    lng2 = QString::number(m_lng,'f',6);
}

// void MainWindow::mySendCommand(int mac, int cmd)
// {
//     sendCommand(mac, cmd);
// }

void MainWindow::makePlot(){


}

void MainWindow::setDate()
{

}


void MainWindow::onTempHumi(int mac, double temp, double humi)
{
    //qDebug()<<DATA::temp<<"AA";
    //if(temp>DATA::temp_t || humi<DATA::hump_t){
        //sendCommand(06,3); //bat may bom
       // onLR(1,3);
    //}
    //else
        //sendCommand(06,4); //tat may bom
        //onLR(1,4);
    if(temp>DATA::temp_t)
    {
        console->insertPlainText("Temperature exceeds the threshold!!");
    }
    if(humi > DATA::hump_t)
    {
        console->insertPlainText("Temperature exceeds the threshold!!");
    }
}

void MainWindow::onLux_t(int mac, double lux)
{
    if(lux<=DATA::lux_t&&!DATA::onLight){
        //sendCommand(06,5);//bat den
        onLR(90,5);
    }
    else if(lux>DATA::lux_t&&DATA::onLight){
        //sendCommand(06,6); //tat den
        onLR(90,6);
    }
}

void MainWindow::onST(QString tmp)
{
    console->moveCursor(QTextCursor::End);
    console->insertPlainText(tmp+"\n\r");

}
//void MainWindow::onLRHTD(int mac, int td, int rhd){
//    float temp=(float)td/100;
//    float humi=(float)rhd/100;
//    sendMqttData(mac,temp,"temparature");
//    sendMqttData(mac,humi,"humidity");

//}

void MainWindow::onMois_t(int mac, double mois)
{
    if(mois<=DATA::mois_t&&!DATA::onPump){
        //sendCommand(06,5);//bat den
        onLR(90,3);//bat bom

    }
    //else {
        //sendCommand(06,6); //tat den
    if(mois>DATA::mois_t2&&DATA::onPump)
    {
        onLR(90,4);//tat bom
    }
    //}
}
void MainWindow::onLR(int mac,int index)
{
    QString Cmd;
    QString tmp;
    switch(index)
    {
    case 3:
        Cmd = QString::number(mac) +"630$";
        tmp = "Send Code " + Cmd +"\n";
        //emit runTimerP();
        break;
    case 4:
        Cmd = QString::number(mac) +"640$";
        tmp = "Send Code " + Cmd +"\n";
        break;
    case 5:
        Cmd = QString::number(mac) +"610$";
        tmp = "Send Code " + Cmd +"\n";
        break;
        //emit runTimerL();
    case 6:
        Cmd = QString::number(mac) +"620$";
        tmp = "Send Code " + Cmd +"\n";

        break;
    case 7:
        Cmd = QString::number(mac) +"500$";
        tmp = "Send Code " + Cmd +"\n";
        break;
    case 8:
        Cmd = "TAKEDUST" + QString::number(mac);
        tmp = "Send Code " + Cmd +"\n";
        break;
    default:
        Cmd="c$";
        tmp = "Send Code " + Cmd + "\n";
    }
    lora->writeData(Cmd);
    console->insertPlainText(tmp);
}

void MainWindow::oncompleteDust(QString data)
{
    QString tmp;
    QStringList data_lst = data.split(":");
    //console->moveCursor(QTextCursor::End);
    //tmp = "\n["+QTime::currentTime().toString()+ "] " + "\nData from sensor:";
    //tmp += data_lst.value(0);
    // tmp += "\nconcentration of dust:        ";
    // tmp += data_lst.value(1);
    //tmp += "ug/m^3";

    //WriteDatatoLogfile(tmp);
    //console->insertPlainText(tmp + "\n");
    receivedFlag = true;
}

//Sang lọc du lieu cho dust density
void MainWindow::SendAIDD(int mac, double dus)
{
    QString smac;
    QString tmp;
    if (mac < 10) {
        smac.append(QString::number(mac));
        smac="0" + smac;
    } else {
        smac.append(QString::number(mac));
    }
    QFile file("./WSAN/AIDustDensity.txt");
        QLabel *testLabel= new QLabel;
        QString line;
        if (file.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::ReadWrite)) {
            QString s;
            QTextStream stream(&file);
            while (!stream.atEnd()) {
                QString read = stream.readLine();
                //qDebug()<<read;
                //qDebug()<<read.mid(0,2);
                tmp = "\n[" + QTime::currentTime().toString() + "] \n" + "Data from sensor RBI " + smac + "\n";
                console->insertPlainText(tmp);
                double ph = rand() % 1 + 14;
                bool incladd = true;
				double cthickness = 0;
				double rate = 0;
				double type = 0;
				double condition = 0;
				if (incladd){
					cthickness = rand() % 10 + 50;
					rate = rand () % 10 + 50;
				}
				else{
					cthickness = 0;
					rate = 0;
				}
				bool inlining = true;
				if ( inlining ){
					type = rand () % 10 + 50;
					condition = rand () % 10 + 50;
				}
				else{
					type = 0;
					condition = 0;
				} 
                sendMqttData(mac, dus + 125, "Dust");
                /*sendMqttData(mac, eq, "equipment_volume");
                sendMqttData(mac, dis, "distance_to_ground_water");
                sendMqttData(mac, H2S, "current_thickness");
                sendMqttData(mac, dis, "max.operating_temperature");
                sendMqttData(mac, H2S, "min.operating_temperature");
                sendMqttData(mac, dis, "max.operating_pressure");
                sendMqttData(mac, H2S, "min.operating_pressure");*/
                //sendMqttData(mac, dis, "operating_hydrogen_partial_pressure");
                sendMqttData(mac, ph, "pH of Water");
                sendMqttData(mac, incladd, "Internal Cladding");
                //sendMqttData(mac, eq, "critical_exposure_temperature");
                //sendMqttData(mac, eq, "cladding_thinkness");
                sendMqttData(mac, cthickness, "Cladding Thickness");
                sendMqttData(mac, rate, "Cladding Corrosion Rate");
                //sendMqttData(mac, dis, "h2s_content_in_water");
                sendMqttData(mac, inlining, "Internal Lining");
                sendMqttData(mac, type, "Internal Liner Type");
                sendMqttData(mac, condition, "Internal Liner Condition");
               

                /*if (read.mid(0, 2).toInt() == mac) {
                    if (read.mid(3, 5).toDouble()-dus > 1 || read.mid(3, 5).toDouble()-dus < -1) {
                        sendMqttData(mac, dus, "MaxOT");
                    } else {
                        console->insertPlainText("Call SendAIDD \r\n");
                    }
                } else {
                    qDebug()<<"send new";
                    if (!read.contains("DELETE")) {
                        //delete
                        s.append(read + "\n");
                    }
                } */
            }
            file.resize(0);
            stream << s;
            stream << smac << ":" << dus << endl;
            testLabel->setText(line + "\n");
        }
        file.close();
}
