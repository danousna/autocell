// #include "src/ui/mainwindow.h"
// #include <QApplication>

#include "automate1d.h"
#include "simulateur.h"

int main(int argc, char *argv[]) {
    // QApplication a(argc, argv);
    // MainWindow w;
    // w.show();

    // return a.exec();

    Automate1D a(1, 2, 30);
    std::cout << "automate : " << a << "\n";

    Etat e(22);
    e.setCellule(11, true);

    Simulateur s(a, e);

    std::cout << e << "\n";   
    for (unsigned int i = 0; i < 10; i++) {
        s.next();
        std::cout << s.dernier() << "\n";
    }

    return 0;
}
