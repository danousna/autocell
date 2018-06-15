#include "wwview.h"
#include "ui_wwview.h"

WWView::WWView(QWidget *parent):
QWidget(parent), ui(new Ui::WWView),
dimensions(25), tailleCell(25), steps(11), stepState(0), speed(100), paused(true),
automate(AutomateWW::getInstance()) {
    ui->setupUi(this);

    srand(time(NULL));

    // UI Speed
    connect(ui->inputSpeed, SIGNAL(valueChanged(int)), this, SLOT(changeSpeed(int)));

    // UI Taille
    connect(ui->btnRefreshTaille, SIGNAL(clicked()), this, SLOT(refreshTaille()));

    // UI Steps
    connect(ui->inputSteps, SIGNAL(valueChanged(int)), this, SLOT(changeSteps(int)));

    // UI Gen
    connect(ui->btnGenRandom, SIGNAL(clicked()), this, SLOT(randomGen()));

    // UI play/pause et reset
    connect(ui->btnNext, SIGNAL(clicked()), this, SLOT(next()));
    connect(ui->btnPlay, SIGNAL(clicked()), this, SLOT(togglePlayPause()));
    connect(ui->btnReset, SIGNAL(clicked()), this, SLOT(reset()));

    // UI et interaction avec la grille.
    drawGrille(ui->grille, tailleCell, dimensions);
    connect(ui->grille, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(toggleCell(QTableWidgetItem*)));
}

WWView::~WWView()
{
    delete ui;
}

void WWView::toggleCell(QTableWidgetItem* item) {
    // Si une cellule était vide (noire), on la rend conductrice.
    if (ui->grille->item(item->row(), item->column())->backgroundColor() == "black") {
        ui->grille->item(item->row(), item->column())->setBackgroundColor("yellow");
    }
    // Si elle était conductrice (jaune), on la rend tête d'électron.
    else if (ui->grille->item(item->row(), item->column())->backgroundColor() == "yellow") {
        ui->grille->item(item->row(), item->column())->setBackgroundColor("blue");
    }
    // Si elle était tête (bleue), on la rend queue.
    else if (ui->grille->item(item->row(), item->column())->backgroundColor() == "blue") {
        ui->grille->item(item->row(), item->column())->setBackgroundColor("red");
    }
    // Cas restant : elle était queue, on la rend vide
    else {
        ui->grille->item(item->row(), item->column())->setBackgroundColor("black");
    }
}

void WWView::next() {
    Grille2D g(dimensions);
    this->syncGrilles(&g, ui->grille, false);
    Simulateur s(*automate, g, dimensions);

    for (int i = 0; i < steps; i++) {
        if (i <= stepState) {
            ui->stepsLabel->setText(QStringLiteral("%1 sur").arg(i));
            this->syncGrilles(&s.dernier(), ui->grille, true);
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

void WWView::togglePlayPause() {
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

void WWView::play(int startStep) {
    Grille2D g(dimensions);
    this->syncGrilles(&g, ui->grille, false);

    Simulateur s(*automate, g, dimensions);

    for (int i = 0; i < steps; i++) {
        stepState = i;

        // Si on est pas encore à l'étape ou on avait pausé, on fait juste avancer le simulateur au point atteint.
        if (i < startStep) {
            s.next();
        }
        else {
            if (!paused) {
                ui->stepsLabel->setText(QStringLiteral("%1 sur").arg(i));
                this->syncGrilles(&s.dernier(), ui->grille, true);
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

void WWView::reset() {
    viderGrille();
    ui->btnNext->setEnabled(true);
    ui->btnPlay->setEnabled(true);
    stepState = 0;
}

void WWView::refreshTaille() {
    dimensions = ui->inputDimensions->value();
    tailleCell = ui->inputTailleCell->value();
    drawGrille(ui->grille, tailleCell, dimensions);
}

// Si set TRUE, grilleAutomate SET les valeurs de TableWidget.
// Si set FALSE, grilleAutomate GET les valeurs de TableWidget.
void WWView::syncGrilles(Grille* grilleAutomate, QTableWidget* grilleQT, bool set) {
    if (grilleQT->columnCount() != grilleAutomate->getTaille()) throw new AutoCellException("La grille Qt n'a pas les mêmes dimensions que la grille de l'automate.");

    /* Rappel :
     * 0 = cellule vide
     * 1 = cellule conductrice
     * 2 = tête d'électron
     * 3 = queue d'électron */

    if (set) {
        for (unsigned int i = 0; i < grilleAutomate->getTaille(); i++) {
            for (unsigned int j = 0; j < grilleAutomate->getTaille(); j++) {
                if (grilleAutomate->getCellVal(i, j) == 0) {
                    grilleQT->item(j, i)->setBackgroundColor("black");
                } else if (grilleAutomate->getCellVal(i, j) == 1) {
                    grilleQT->item(j, i)->setBackgroundColor("yellow");
                } else if (grilleAutomate->getCellVal(i, j) == 2) {
                    grilleQT->item(j, i)->setBackgroundColor("blue");
                } else {
                    grilleQT->item(j, i)->setBackgroundColor("red");
                }
            }
        }
    } else {
        for (unsigned int i = 0; i < grilleAutomate->getTaille(); i++) {
            for (unsigned int j = 0; j < grilleAutomate->getTaille(); j++) {
                if (grilleQT->item(j, i)->background().color() == QColor("black")) {
                    Cell c(Etat(0, "vide"));
                    grilleAutomate->setCell(c, i, j);
                } else if (grilleQT->item(j, i)->background().color() == QColor("yellow")) {
                    Cell c(Etat(1, "conductrice"));
                    grilleAutomate->setCell(c, i, j);
                } else if (grilleQT->item(j, i)->background().color() == QColor("blue")) {
                    Cell c(Etat(2, "tête d'électron"));
                    grilleAutomate->setCell(c, i, j);
                } else {
                    Cell c(Etat(3, "queue d'électron"));
                    grilleAutomate->setCell(c, i, j);
                }
            }
        }
    }
}

void WWView::viderGrille() {
    for (unsigned int i = 0; i < dimensions; i++) {
        for (unsigned int j = 0; j < dimensions; j++) {
            ui->grille->item(i, j)->setBackgroundColor("black");
            ui->grille->item(i, j)->setTextColor("black");
        }
    }
}

void WWView::drawGrille(QTableWidget* grille, unsigned int tCell, unsigned int n) {
    grille->setColumnCount(n);
    grille->setRowCount(n);

    grille->setFixedSize(tCell * n + 2, tCell * n - 2 * tCell);

    for (unsigned int i = 0; i < n; ++i) {
        grille->setRowHeight(i, tCell);

        for (unsigned int j = 0; j < n; j++) {
            grille->setColumnWidth(j, tCell);
            grille->setItem(i, j, new QTableWidgetItem(""));
            grille->item(i, j)->setBackgroundColor("black");
            grille->item(i, j)->setTextColor("black");
        }
    }
}

void WWView::toggleUI() {
    bool enabled = ui->grille->isEnabled();

    ui->inputSpeed->setEnabled(!enabled);
    ui->inputDimensions->setEnabled(!enabled);
    ui->btnRefreshTaille->setEnabled(!enabled);
    ui->inputSteps->setEnabled(!enabled);
    ui->grille->setEnabled(!enabled);
}

void WWView::changeSpeed(int s) {
    speed = s;
}

void WWView::changeSteps(int n) {
    steps = n;
}

void WWView::randomGen() {
    int randZeroThree = 0;

    for (unsigned int i = 0; i < dimensions; i++) {
        for (unsigned int j = 0; j < dimensions; j++) {
            randZeroThree = rand() % 4;

            if (randZeroThree == 0) {
                ui->grille->item(j, i)->setBackgroundColor("black");
            } else if (randZeroThree == 1) {
                ui->grille->item(j, i)->setBackgroundColor("yellow");
            } else if (randZeroThree == 2) {
                ui->grille->item(j, i)->setBackgroundColor("blue");
            } else {
                ui->grille->item(j, i)->setBackgroundColor("red");
            }
        }
    }
}
