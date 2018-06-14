#include "golview.h"
#include "ui_golview.h"

GoLView::GoLView(QWidget *parent):
QWidget(parent), ui(new Ui::GoLView),
voisinsMin(2), voisinsMax(3), dimensions(25), tailleCell(25), steps(11), stepState(0), speed(100), paused(true),
automate(AutomateGoL::getInstance()) {
    ui->setupUi(this);

    srand(time(NULL));

    // UI Speed
    connect(ui->inputSpeed, SIGNAL(valueChanged(int)), this, SLOT(changeSpeed(int)));

    // UI Taille
    connect(ui->btnRefreshTaille, SIGNAL(clicked()), this, SLOT(refreshTaille()));

    // UI Voisins
    connect(ui->inputVoisinsMin, SIGNAL(valueChanged(int)), this, SLOT(changeVoisinsMin(int)));
    connect(ui->inputVoisinsMax, SIGNAL(valueChanged(int)), this, SLOT(changeVoisinsMax(int)));

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

GoLView::~GoLView()
{
    delete ui;
}

void GoLView::toggleCell(QTableWidgetItem* item) {
    // Si elle était vivante (blanche), on la rends morte.
    if (ui->grille->item(item->row(), item->column())->backgroundColor() == "white") {
        ui->grille->item(item->row(), item->column())->setBackgroundColor("black");
    }
    // Cas contraire 
    else {
        ui->grille->item(item->row(), item->column())->setBackgroundColor("white");
    }
}

void GoLView::next() {
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

void GoLView::togglePlayPause() {
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

void GoLView::play(int startStep) {
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

void GoLView::reset() {
    viderGrille();
    ui->btnNext->setEnabled(true);
    ui->btnPlay->setEnabled(true);
    ui->stepsLabel->setText(QString("0 sur"));
    stepState = 0;
}

void GoLView::refreshTaille() {
    dimensions = ui->inputDimensions->value();
    tailleCell = ui->inputTailleCell->value();
    drawGrille(ui->grille, tailleCell, dimensions);
}

// Si set TRUE, grilleAutomate SET les valeurs de TableWidget.
// Si set FALSE, grilleAutomate GET les valeurs de TableWidget.
void GoLView::syncGrilles(Grille* grilleAutomate, QTableWidget* grilleQT, bool set) {
    if (grilleQT->columnCount() != grilleAutomate->getTaille()) throw new AutoCellException("La grille Qt n'a pas les mêmes dimensions que la grille de l'automate.");

    if (set) {
        for (unsigned int i = 0; i < grilleAutomate->getTaille(); i++) {
            for (unsigned int j = 0; j < grilleAutomate->getTaille(); j++) {
                if (grilleAutomate->getCellVal(i, j)) {
                    grilleQT->item(j, i)->setBackgroundColor("black");
                } else {
                    grilleQT->item(j, i)->setBackgroundColor("white");
                }
            }
        }
    } else {
        for (unsigned int i = 0; i < grilleAutomate->getTaille(); i++) {
            for (unsigned int j = 0; j < grilleAutomate->getTaille(); j++) {
                if (grilleQT->item(j, i)->background().color() == QColor("black")) {
                    Cell c(Etat(1, "vivante"));
                    grilleAutomate->setCell(c, i, j);
                } else {
                    Cell c(Etat(0, "morte"));
                    grilleAutomate->setCell(c, i, j);
                }
            }
        }
    }
}

void GoLView::viderGrille() {
    for (unsigned int i = 0; i < dimensions; i++) {
        for (unsigned int j = 0; j < dimensions; j++) {
            ui->grille->item(i, j)->setBackgroundColor("white");
            ui->grille->item(i, j)->setTextColor("white");
        }
    }
}

void GoLView::drawGrille(QTableWidget* grille, unsigned int tCell, unsigned int n) {
    grille->setColumnCount(n);
    grille->setRowCount(n);

    grille->setFixedSize(tCell * n + 2, tCell * n - 2 * tCell);

    for (unsigned int i = 0; i < n; ++i) {
        grille->setRowHeight(i, tCell);

        for (unsigned int j = 0; j < n; j++) {
            grille->setColumnWidth(j, tCell);
            grille->setItem(i, j, new QTableWidgetItem(""));
            grille->item(i, j)->setBackgroundColor("white");
            grille->item(i, j)->setTextColor("white");
        }
    }
}

void GoLView::toggleUI() {
    bool enabled = ui->grille->isEnabled();

    ui->inputSpeed->setEnabled(!enabled);
    ui->inputDimensions->setEnabled(!enabled);
    ui->btnRefreshTaille->setEnabled(!enabled);
    ui->inputVoisinsMin->setEnabled(!enabled);
    ui->inputVoisinsMax->setEnabled(!enabled);
    ui->inputSteps->setEnabled(!enabled);
    ui->grille->setEnabled(!enabled);
}

void GoLView::changeVoisinsMin(int vMin) {
    voisinsMin = vMin;
    automate = AutomateGoL::getInstance(voisinsMin, voisinsMax);
}

void GoLView::changeVoisinsMax(int vMax) {
    voisinsMax = vMax;
    automate = AutomateGoL::getInstance(voisinsMin, voisinsMax);
}

void GoLView::changeSpeed(int s) {
    speed = s;
}

void GoLView::changeSteps(int n) {
    steps = n;
}

void GoLView::randomGen() {
    int randZeroOne = 0;    

    for (unsigned int i = 0; i < dimensions; i++) {
        for (unsigned int j = 0; j < dimensions; j++) {
            randZeroOne = rand() % 2;

            if (randZeroOne == 1) {
                ui->grille->item(j, i)->setBackgroundColor("black");
            } else {
                ui->grille->item(j, i)->setBackgroundColor("white");
            }
        }
    }
}

