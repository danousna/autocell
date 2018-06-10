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

    Grille1D g(23);
    Cell c(Etat(1, "vivante"));
    g.setCell(c, 11);

    Simulateur s(a, g, 23);

    s.run(11);

    return 0;
}
