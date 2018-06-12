#include "elementaireview.h"
#include "ui_elementaireview.h"

ElementaireView::ElementaireView(QWidget *parent): 
QWidget(parent), ui(new Ui::ElementaireView), taille(23), tailleCell(40), steps(11), automate(AutomateElementaire::getInstance(30)) {
    ui->setupUi(this);

    // UI Taille
    connect(ui->btnRefreshTaille, SIGNAL(clicked()), this, SLOT(refreshTaille()));

    // UI Règle
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
    drawGrille(ui->grilleDepart, tailleCell, taille, 1);

    connect(ui->grilleDepart, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(toggleCell(QTableWidgetItem*)));
    connect(ui->btnPlaySimulation, SIGNAL(clicked()), this, SLOT(playSimulation()));

    // Affichage de la grille.
    drawGrille(ui->grille, tailleCell, taille, steps);
}

ElementaireView::~ElementaireView()
{
    delete ui;
}

void ElementaireView::refreshTaille() {
    taille = ui->inputTaille->value();
    drawGrille(ui->grilleDepart, tailleCell, taille, 1);
    drawGrille(ui->grille, tailleCell, taille, steps);
}

void ElementaireView::toggleCell(QTableWidgetItem* item) {
    // Si elle était vivante (blanche), on la rends morte.
    if (ui->grilleDepart->item(0, item->column())->backgroundColor() == "white") {
        ui->grilleDepart->item(0, item->column())->setBackgroundColor("black");
    }
    // Cas contraire 
    else {
        ui->grilleDepart->item(0, item->column())->setBackgroundColor("white");
    }
}

void ElementaireView::playSimulation() {
    this->viderGrille();

    Grille1D g(taille);
    this->syncGrilles(&g, ui->grilleDepart, 0, false);

    Simulateur s(*automate, g, taille);

    for (int i = 0; i < steps; i++) {
        this->syncGrilles(&s.dernier(), ui->grille, i, true);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        QCoreApplication::processEvents();
        s.next();
    }
}

// Si set TRUE, grilleAutomate SET les valeurs de TableWidget.
// Si set FALSE, grilleAutomate GET les valeurs de TableWidget.
void ElementaireView::syncGrilles(Grille* grilleAutomate, QTableWidget* grilleQT, int step, bool set) {
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

void ElementaireView::viderGrille() {
    for (unsigned int i = 0; i < steps; ++i) {
        for (unsigned int j = 0; j < taille; j++) {
            ui->grille->item(i, j)->setBackgroundColor("white");
            ui->grille->item(i, j)->setTextColor("white");
        }
    }
}

void ElementaireView::drawGrille(QTableWidget* grille, unsigned int tCell, unsigned int t, unsigned int r) {
    grille->setColumnCount(t);
    grille->setRowCount(r);

    // 1D
    if (r == 1) {
        grille->setFixedSize(tCell * t + 2, tCell);

        for(unsigned int i = 0; i < t; i++) {
            grille->setColumnWidth(i, tCell);
            grille->setItem(0, i, new QTableWidgetItem(""));
            if (grille->item(0, i) != NULL) {
                grille->item(0, i)->setBackgroundColor("white");
                grille->item(0, i)->setTextColor("white");
            }
        }
    }
    // 2D 
    else {
        grille->setFixedSize(tCell * t + 2, tCell * r - 40);

        for (unsigned int i = 0; i < r; ++i) {
            grille->setRowHeight(i, tCell - 5);

            for (unsigned int j = 0; j < t; j++) {
                grille->setColumnWidth(j, tCell);
                grille->setItem(i, j, new QTableWidgetItem(""));
                grille->item(i, j)->setBackgroundColor("white");
                grille->item(i, j)->setTextColor("white");
            }
        }
    }
}

void ElementaireView::synchronizeNumToNumBit(int i) {
    std::string numBit = NumToNumBit(i);

    for (unsigned int i = 0; i < numBit.size(); ++i)
    {
        numeroBit[i]->setText((QString) numBit[i]);
    }
}

void ElementaireView::synchronizeNumBitToNum(const QString& s) {
    if (s == "") return;

    std::string string;

    for (unsigned int i = 0; i < 8; ++i)
    {
        string += numeroBit[i]->text().toStdString();
    }

    short unsigned int numero = NumBitToNum(string);

    ui->numeroInput->setValue(numero);
}
