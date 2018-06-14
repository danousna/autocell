#include "elementaireview.h"
#include "ui_elementaireview.h"

ElementaireView::ElementaireView(QWidget *parent): 
QWidget(parent), ui(new Ui::ElementaireView), 
taille(23), tailleCell(40), steps(11), stepState(0), speed(100), paused(true),
automate(AutomateElementaire::getInstance(30)) {
    ui->setupUi(this);

    srand(time(NULL));

    // UI Speed
    connect(ui->inputSpeed, SIGNAL(valueChanged(int)), this, SLOT(changeSpeed(int)));

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

    connect(ui->numeroInput, SIGNAL(valueChanged(int)), this, SLOT(synchronizeNumToNumBit(int)));
    for (unsigned int i = 0; i < 8; i++) {
        numeroBit[i]->setValidator(zeroOneValidator);
        connect(numeroBit[i], SIGNAL(textChanged(QString)), this, SLOT(synchronizeNumBitToNum(QString)));
    }

    // UI Steps
    connect(ui->inputSteps, SIGNAL(valueChanged(int)), this, SLOT(changeSteps(int)));

    // UI Gen
    connect(ui->btnGenRandom, SIGNAL(clicked()), this, SLOT(randomGen()));

    // Affichage et interaction avec la grille de départ.
    drawGrille(ui->grilleDepart, tailleCell, taille, 1);
    connect(ui->grilleDepart, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(toggleCell(QTableWidgetItem*)));
    
    // Boutons play/pause et reset
    connect(ui->btnNext, SIGNAL(clicked()), this, SLOT(next()));
    connect(ui->btnPlay, SIGNAL(clicked()), this, SLOT(togglePlayPause()));
    connect(ui->btnReset, SIGNAL(clicked()), this, SLOT(reset()));

    // Affichage de la grille.
    drawGrille(ui->grille, tailleCell, taille, steps);
}

ElementaireView::~ElementaireView()
{
    delete ui;
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

void ElementaireView::next() {
    Grille1D g(taille);
    this->syncGrilles(&g, ui->grilleDepart, 0, false);
    Simulateur s(*automate, g, taille);

    for (int i = 0; i < steps; i++) {
        if (i <= stepState) {
            this->syncGrilles(&s.dernier(), ui->grille, i, true);
            s.next();
        }

        if (i > stepState) {
            stepState++;
            return;
        }
    }

    // Si on a atteint la fin, on met en pause.
    if (stepState == steps - 1) {
        // On disable bouton next.
        ui->btnNext->setEnabled(false);
        return;
    }

    stepState++;
}

void ElementaireView::togglePlayPause() {
    toggleUI();

    if (paused) {
        paused = false;
        ui->btnPlay->setText("Mettre en pause");
        play(stepState);
    } else {
        paused = true;
        ui->btnPlay->setText("Lancer la simulation");
    }
}

void ElementaireView::play(int startStep) {
    Grille1D g(taille);
    this->syncGrilles(&g, ui->grilleDepart, 0, false);

    Simulateur s(*automate, g, taille);

    for (int i = 0; i < steps; i++) {
        stepState = i;

        // Si on est pas encore à l'étape ou on avait pausé, on fait juste avancer le simulateur au point atteint.
        if (i < startStep) {
            s.next();
        }
        else {
            if (!paused) {
                this->syncGrilles(&s.dernier(), ui->grille, i, true);
                std::this_thread::sleep_for(std::chrono::milliseconds(speed));
                QCoreApplication::processEvents();
                s.next();

                // Si on a atteint la fin, on met en pause.
                if (stepState == steps - 1) {
                    togglePlayPause();
                    // On disable bouton play, si on click sur reset, on remet enable.
                    ui->btnPlay->setEnabled(false);
                    return;
                }
            } else {
                return;
            }
        }
    }
}

void ElementaireView::reset() {
    viderGrille();
    ui->btnNext->setEnabled(true);
    ui->btnPlay->setEnabled(true);
    stepState = 0;
}

void ElementaireView::refreshTaille() {
    taille = ui->inputTaille->value();
    tailleCell = ui->inputTailleCell->value();

    drawGrille(ui->grilleDepart, tailleCell, taille, 1);
    drawGrille(ui->grille, tailleCell, taille, steps);
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
        grille->setFixedSize(tCell * t + 2, tCell * 11 + 2);

        for (unsigned int i = 0; i < r; ++i) {
            grille->setRowHeight(i, tCell);

            for (unsigned int j = 0; j < t; j++) {
                grille->setColumnWidth(j, tCell);
                grille->setItem(i, j, new QTableWidgetItem(""));
                grille->item(i, j)->setBackgroundColor("white");
                grille->item(i, j)->setTextColor("white");
            }
        }
    }
}

void ElementaireView::toggleUI() {
    bool enabled = ui->grilleDepart->isEnabled();

    ui->inputSpeed->setEnabled(!enabled);
    ui->inputTaille->setEnabled(!enabled);
    ui->btnRefreshTaille->setEnabled(!enabled);
    ui->numeroInput->setEnabled(!enabled);
    ui->bit1->setEnabled(!enabled);
    ui->bit2->setEnabled(!enabled);
    ui->bit3->setEnabled(!enabled);
    ui->bit4->setEnabled(!enabled);
    ui->bit5->setEnabled(!enabled);
    ui->bit6->setEnabled(!enabled);
    ui->bit7->setEnabled(!enabled);
    ui->bit8->setEnabled(!enabled);
    ui->inputSteps->setEnabled(!enabled);
    ui->grilleDepart->setEnabled(!enabled);
}

void ElementaireView::synchronizeNumToNumBit(int i) {
    std::string numBit = NumToNumBit(i);

    for (unsigned int i = 0; i < numBit.size(); ++i)
    {
        numeroBit[i]->setText((QString) numBit[i]);
    }

    // Charger l'automate correspondant.
    automate = AutomateElementaire::getInstance(i);
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

    // Charger l'automate correspondant.
    automate = AutomateElementaire::getInstance(numero);
}

void ElementaireView::changeSpeed(int s) {
    speed = s;
}

void ElementaireView::changeSteps(int n) {
    steps = n;
    drawGrille(ui->grille, tailleCell, taille, steps);
}

void ElementaireView::randomGen() {
    int randZeroOne = 0;    

    for (unsigned int i = 0; i < taille; i++) {
        randZeroOne = rand() % 2;

        if (randZeroOne == 1) {
            ui->grilleDepart->item(0, i)->setBackgroundColor("black");
        } else {
            ui->grilleDepart->item(0, i)->setBackgroundColor("white");
        }
    }
}