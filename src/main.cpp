#include "src/ui/mainwindow.h"
#include <QApplication>

#include "simulateur.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName(QString("AutoCell"));
    MainWindow w;
    w.show();

    return a.exec();
}
