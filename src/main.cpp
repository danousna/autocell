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

    Grille1D g(10);
    Cell c(Etat(1, "vivante"));
    g.setCell(c, 5);

    Simulateur s(a, g, 10);

    s.run(10);

    return 0;
}
