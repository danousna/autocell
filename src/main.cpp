#include "src/ui/mainwindow.h"
#include <QApplication>

#include "simulateur.h"

int main(int argc, char *argv[]) {
    // Automate Elementaire
    
    // AutomateElementaire a(30);
    // std::cout << a << "\n";

    // Grille1D g(23);
    // Cell c(Etat(1, "vivante"));
    // g.setCell(c, 11);

    // Simulateur s(a, g, 23);
    // s.run(11);


    // Automate Jeu de la Vie

    AutomateGoL a;
    std::cout << a << "\n";

    Grille2D g(7);
    Cell c(Etat(1, "vivante"));
    g.setCell(c, 2, 1);
    g.setCell(c, 2, 2);
    g.setCell(c, 2, 3);

    Simulateur s(a, g, 7);
    s.run(3);

    return 0;

    // QApplication a(argc, argv);
    // MainWindow w;
    // w.setFixedSize(1000, 500);
    // w.show();

    // return a.exec();
}
