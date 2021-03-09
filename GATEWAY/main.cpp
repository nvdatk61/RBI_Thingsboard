#include "mainwindow.h"
#include <QApplication>
#include "define.h"
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont f = a.font();
	f.setFamily("Monaco");
	f.setPointSize(11);
	a.setFont(f);
	//QQuickView w;
    MainWindow w;
    w.show();
    w.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    w.setFixedSize(w.size());
    return a.exec();
}
