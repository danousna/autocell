#include "src/ui/mainwindow.h"
#include <QApplication>

#include "simulateur.h"

int main(int argc, char *argv[]) {
     // QApplication a(argc, argv);
     // MainWindow w;
     // w.setFixedSize(1000, 500);
     // w.show();
     // return a.exec();

    AutomateElementaire a(30);
    std::cout << "automate élémentaire : " << a << "\n";

    Simulateur s(a, 10);
    std::cout << s << "\n";

    return 0;
}
