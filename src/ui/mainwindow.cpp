#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
QMainWindow(parent), ui(new Ui::MainWindow), taille(23), tailleCell(40), steps(11) {
    ui->setupUi(this);

    // Connection des actions aux fenetres
    connect(ui->actionElementaire, SIGNAL(triggered()), this, SLOT(showFenetreAutomateElementaire()));
    connect(ui->actionGoL, SIGNAL(triggered()), this, SLOT(showFenetreAutomateGoL()));
    connect(ui->actionWireworld, SIGNAL(triggered()), this, SLOT(showFenetreAutomateWireworld()));

    // Setup des UI des trois automates
    setupAutomateElementaireWindow();
    setupAutomateGoLWindow();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::showFenetreAutomateElementaire() {
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::showFenetreAutomateGoL() {
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::showFenetreAutomateWireworld() {
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::setupAutomateElementaireWindow() {
    numeroBit[0] = ui->bit1;
    numeroBit[1] = ui->bit2;
    numeroBit[2] = ui->bit3;
    numeroBit[3] = ui->bit4;
    numeroBit[4] = ui->bit5;
    numeroBit[5] = ui->bit6;
    numeroBit[6] = ui->bit7;
    numeroBit[7] = ui->bit8;

    zeroOneValidator = new QIntValidator(this);
    zeroOneValidator->setRange(0, 1);

    // Configuration de l'automate.
    connect(ui->numeroInput, SIGNAL(valueChanged(int)), this, SLOT(synchronizeNumToNumBit(int)));

    for (unsigned int i = 0; i < 8; i++) {
        numeroBit[i]->setValidator(zeroOneValidator);
        connect(numeroBit[i], SIGNAL(textChanged(QString)), this, SLOT(synchronizeNumBitToNum(QString)));
    }

    // Affichage et interaction avec la grille de départ.
    ui->grilleDepart->setColumnCount(taille);
    ui->grilleDepart->setRowCount(1);
    ui->grilleDepart->setFixedSize(tailleCell * taille + 2, tailleCell);

    for(unsigned int i = 0; i < taille; i++) {
        ui->grilleDepart->setColumnWidth(i, tailleCell);
        ui->grilleDepart->setItem(0, i, new QTableWidgetItem(""));
        if (ui->grilleDepart->item(0, i) != NULL) {
            ui->grilleDepart->item(0, i)->setBackgroundColor("white");
            ui->grilleDepart->item(0, i)->setTextColor("white");
        }
    }

    connect(ui->grilleDepart, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(toggleCell(QTableWidgetItem*)));
    connect(ui->btnPlaySimulation, SIGNAL(clicked()), this, SLOT(playSimulation()));

    // Affichage de la grille.
    ui->grille->setColumnCount(taille);
    ui->grille->setRowCount(steps);
    ui->grille->setFixedSize(tailleCell * taille + 2, tailleCell * steps - 40);

    for (unsigned int i = 0; i < steps; ++i) {
        ui->grille->setRowHeight(i, tailleCell - 5);

        for (unsigned int j = 0; j < taille; j++) {
            ui->grille->setColumnWidth(j, tailleCell);
            ui->grille->setItem(i, j, new QTableWidgetItem(""));
            ui->grille->item(i, j)->setBackgroundColor("white");
            ui->grille->item(i, j)->setTextColor("white");
        }
    }
}

void MainWindow::setupAutomateGoLWindow() {
    
}

void MainWindow::toggleCell(QTableWidgetItem* item) {
    // Si elle était vivante (blanche), on la rends morte.
    if (ui->grilleDepart->item(0, item->column())->backgroundColor() == "white") {
        ui->grilleDepart->item(0, item->column())->setBackgroundColor("black");
    }
    // Cas contraire 
    else {
        ui->grilleDepart->item(0, item->column())->setBackgroundColor("white");
    }
}

void MainWindow::playSimulation() {
    this->viderGrille();

    AutomateElementaire a(30);

    Grille1D g(taille);
    this->syncGrilles(&g, ui->grilleDepart, 0, false);

    Simulateur s(a, g, taille);

    for (int i = 0; i < steps; i++) {
        this->syncGrilles(&s.dernier(), ui->grille, i, true);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        QCoreApplication::processEvents();
        s.next();
    }
}

// Si set TRUE, grilleAutomate SET les valeurs de TableWidget.
// Si set FALSE, grilleAutomate GET les valeurs de TableWidget.
void MainWindow::syncGrilles(Grille* grilleAutomate, QTableWidget* grilleQT, int step, bool set) {
    if (grilleQT->columnCount() != grilleAutomate->getTaille()) throw new AutoCellException("La grille Qt n'a pas la même taille que la grille de l'automate.");

    if (set) {
        for (unsigned int i = 0; i < grilleAutomate->getTaille(); i++) {
            if (grilleAutomate->getCellVal(i, 0)) {
                grilleQT->item(step, i)->setBackgroundColor("black");
            } else {
                grilleQT->item(step, i)->setBackgroundColor("white");
            }
        }
    } else {
        for (unsigned int i = 0; i < grilleAutomate->getTaille(); i++) {
            if (grilleQT->item(0, i)->background().color() == QColor("black")) {
                Cell c(Etat(1, "vivante"));
                grilleAutomate->setCell(c, i, 0);
            } else {
                Cell c(Etat(0, "morte"));
                grilleAutomate->setCell(c, i, 0);
            }
        }
    }    
}

void MainWindow::viderGrille() {
    for (unsigned int i = 0; i < steps; ++i) {
        for (unsigned int j = 0; j < taille; j++) {
            ui->grille->item(i, j)->setBackgroundColor("white");
            ui->grille->item(i, j)->setTextColor("white");
        }
    }
}

void MainWindow::synchronizeNumToNumBit(int i) {
    std::string numBit = NumToNumBit(i);

    for (unsigned int i = 0; i < numBit.size(); ++i)
    {
        numeroBit[i]->setText((QString) numBit[i]);
    }
}

void MainWindow::synchronizeNumBitToNum(const QString& s) {
    if (s == "") return;

    std::string string;

    for (unsigned int i = 0; i < 8; ++i)
    {
        string += numeroBit[i]->text().toStdString();
    }

    short unsigned int numero = NumBitToNum(string);

    ui->numeroInput->setValue(numero);
}
