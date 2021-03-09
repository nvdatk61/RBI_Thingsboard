/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../GATEWAY/mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      24,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   23,   23,   23, 0x05,

 // slots: signature, parameters, type, tag, flags
      24,   23,   23,   23, 0x08,
      46,   23,   23,   23, 0x08,
      68,   23,   23,   23, 0x08,
      93,   23,   23,   23, 0x08,
     119,   23,   23,   23, 0x08,
     142,   23,   23,   23, 0x08,
     166,   23,   23,   23, 0x08,
     190,   23,   23,   23, 0x08,
     214,   23,   23,   23, 0x08,
     235,   23,   23,   23, 0x08,
     246,   23,   23,   23, 0x08,
     260,   23,   23,   23, 0x08,
     277,   23,   23,   23, 0x08,
     296,   23,   23,   23, 0x08,
     309,   23,   23,   23, 0x08,
     329,  362,   23,   23, 0x08,
     365,  362,   23,   23, 0x08,
     395,  409,   23,   23, 0x08,
     411,  409,   23,   23, 0x08,
     432,  409,   23,   23, 0x08,
     452,  409,   23,   23, 0x08,
     473,   23,   23,   23, 0x08,
     487,   23,   23,   23, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0readySend()\0\0on_btnPrint_clicked()\0"
    "on_btnClear_clicked()\0onImageReceived(QString)\0"
    "onreceivedDataLR(QString)\0"
    "oncompleteLux(QString)\0onTempAndHumLR(QString)\0"
    "oncompleteMois(QString)\0oncompleteDust(QString)\0"
    "on_btnExit_clicked()\0sendMqtt()\0"
    "mqttConnect()\0connectEnabled()\0"
    "subscribePressed()\0subscribed()\0"
    "getSubMQTT(QString)\0"
    "sendMqttData(int,double,QString)\0,,\0"
    "onTempHumi(int,double,double)\0"
    "onLR(int,int)\0,\0SendAIDD(int,double)\0"
    "onLux_t(int,double)\0onMois_t(int,double)\0"
    "onST(QString)\0makePlot()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->readySend(); break;
        case 1: _t->on_btnPrint_clicked(); break;
        case 2: _t->on_btnClear_clicked(); break;
        case 3: _t->onImageReceived((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->onreceivedDataLR((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->oncompleteLux((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->onTempAndHumLR((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->oncompleteMois((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->oncompleteDust((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->on_btnExit_clicked(); break;
        case 10: _t->sendMqtt(); break;
        case 11: _t->mqttConnect(); break;
        case 12: _t->connectEnabled(); break;
        case 13: _t->subscribePressed(); break;
        case 14: _t->subscribed(); break;
        case 15: _t->getSubMQTT((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 16: _t->sendMqttData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 17: _t->onTempHumi((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 18: _t->onLR((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 19: _t->SendAIDD((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 20: _t->onLux_t((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 21: _t->onMois_t((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 22: _t->onST((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 23: _t->makePlot(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 24)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 24;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::readySend()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
