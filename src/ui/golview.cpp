#include "golview.h"
#include "ui_golview.h"

GoLView::GoLView(QWidget *parent): 
AutomateView(parent, 40, 20, 0, 100, true, AutomateGoL::getInstance()), ui(new Ui::GoLView), 
voisinsMin(2), voisinsMax(3), dimensions(24) {
    ui->setupUi(this);

    srand(time(NULL));

    // UI Speed
    connect(ui->inputSpeed, SIGNAL(valueChanged(int)), this, SLOT(changeSpeed(int)));
    ui->inputSpeed->setValue(speed);

    // UI Taille
    connect(ui->btnRefreshTaille, SIGNAL(clicked()), this, SLOT(refreshTaille()));
    ui->inputDimensions->setValue(dimensions);
    ui->inputTailleCell->setValue(tailleCell);

    // UI Voisins
    connect(ui->inputVoisinsMin, SIGNAL(valueChanged(int)), this, SLOT(changeVoisinsMin(int)));
    connect(ui->inputVoisinsMax, SIGNAL(valueChanged(int)), this, SLOT(changeVoisinsMax(int)));
    ui->inputVoisinsMin->setValue(voisinsMin);
    ui->inputVoisinsMax->setValue(voisinsMax);

    // UI Steps
    connect(ui->inputSteps, SIGNAL(valueChanged(int)), this, SLOT(changeSteps(int)));
    ui->inputSteps->setValue(steps);

    // UI Gen
    connect(ui->btnGenRandom, SIGNAL(clicked()), this, SLOT(randomGen()));

    // UI Symetrie
    connect(ui->btnSymetrie, SIGNAL(clicked()), this, SLOT(symetrie()));
    
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
    // Si elle était vivante (blanche), on la rend morte.
    if (ui->grille->item(item->row(), item->column())->background() == Qt::white) {
        ui->grille->item(item->row(), item->column())->setBackground(Qt::black);
    }
    // Cas contraire 
    else {
        ui->grille->item(item->row(), item->column())->setBackground(Qt::white);
    }
}

void GoLView::next() {
    Grille2D g(dimensions);
    this->syncGrilles(&g, ui->grille, false);
    Simulateur s(*automate, g, dimensions);

    for (int i = 0; i < steps; i++) {
        if (i <= stepState) {
            ui->stepsLabel->setText(QStringLiteral("%1 sur").arg(i + 1));
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
                ui->stepsLabel->setText(QStringLiteral("%1 sur").arg(i + 1));
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
                    grilleQT->item(j, i)->setBackground(Qt::black);
                } else {
                    grilleQT->item(j, i)->setBackground(Qt::white);
                }
            }
        }
    } else {
        for (unsigned int i = 0; i < grilleAutomate->getTaille(); i++) {
            for (unsigned int j = 0; j < grilleAutomate->getTaille(); j++) {
                if (grilleQT->item(j, i)->background() == Qt::black) {
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
            ui->grille->item(i, j)->setBackground(Qt::white);
        }
    }
}

void GoLView::drawGrille(QTableWidget* grille, unsigned int tCell, unsigned int n) {
    grille->setColumnCount(n);
    grille->setRowCount(n);

    grille->setFixedSize(tCell * n + 2, tCell * n - (n/2 * tCell));

    for (unsigned int i = 0; i < n; ++i) {
        grille->setRowHeight(i, tCell);

        for (unsigned int j = 0; j < n; j++) {
            grille->setColumnWidth(j, tCell);
            grille->setItem(i, j, new QTableWidgetItem(""));
            grille->item(i, j)->setBackground(Qt::white);
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
                ui->grille->item(j, i)->setBackground(Qt::black);
            } else {
                ui->grille->item(j, i)->setBackground(Qt::white);
            }
        }
    }
}

void GoLView::symetrie() {
    int size = this->getTaille();
    for (int i = 0; i < size/2; i++) {
        for (int j = 0; j < size/2; j++) {
            ui->grille->item(j, size-i-1)->setBackground(ui->grille->item(j, i)->background());
            ui->grille->item(size-j-1, size-i-1)->setBackground(ui->grille->item(j, i)->background());
            ui->grille->item(size-j-1, i)->setBackground(ui->grille->item(j, i)->background());
        }
    }
}

void GoLView::save(QFile* f, bool showDialog) {
    QDataStream output(f);
    output.setVersion(QDataStream::Qt_4_5);

    QXmlStreamWriter stream(output.device());
    stream.setAutoFormatting(true);
    stream.setCodec("UTF-8");

    stream.writeStartDocument();
    stream.writeStartElement("automate");

    // Config de l'automate
    stream.writeAttribute("type", "gol");
    stream.writeTextElement("voisinsmin", QString::number(ui->inputVoisinsMin->value()));
    stream.writeTextElement("voisinsmax", QString::number(ui->inputVoisinsMax->value()));
    stream.writeTextElement("vitesse", QString::number(ui->inputSpeed->value()));
    stream.writeTextElement("dimensions", QString::number(ui->inputDimensions->value()));
    stream.writeTextElement("taillecell", QString::number(ui->inputTailleCell->value()));
    stream.writeTextElement("steps", QString::number(ui->inputSteps->value()));

    // Grille
    stream.writeStartElement("grille");
    for (unsigned int i = 0; i < dimensions; i++) {
        QString label = QStringLiteral("row_%1").arg(i);
        QString row("");
        for (unsigned int j = 0; j < dimensions; j++) {
            if (ui->grille->item(i, j)->background() == Qt::black) {
                row += "1";
            } else {
                row += "0";
            }
        }
        stream.writeTextElement(label, row);
    }
    stream.writeEndElement(); // grille

    stream.writeEndElement(); // automate
    stream.writeEndDocument();

    if (showDialog) {
        QMessageBox::information(this, tr("Succès"), tr("Automate sauvegardé."));
    }
}

void GoLView::import(QXmlStreamReader* reader) {
    while (reader->readNextStartElement()) {
        if (reader->name() == "voisinsmin") {
            voisinsMin = reader->readElementText().toInt();
            ui->inputVoisinsMin->setValue(voisinsMin);
        }
        else if (reader->name() == "voisinsmax") {
            voisinsMax = reader->readElementText().toInt();
            ui->inputVoisinsMax->setValue(voisinsMax);
        }
        else if (reader->name() == "vitesse") {
            speed = reader->readElementText().toInt();
            ui->inputSpeed->setValue(speed);
        } 
        else if (reader->name() == "dimensions") {
            dimensions = reader->readElementText().toInt();
            ui->inputDimensions->setValue(dimensions);
        } 
        else if (reader->name() == "taillecell") {
            tailleCell = reader->readElementText().toInt();
            ui->inputTailleCell->setValue(tailleCell);
        }
        else if (reader->name() == "steps") {
            steps = reader->readElementText().toInt();
            ui->inputSteps->setValue(steps);
        }
        else if (reader->isStartElement() && reader->name() == "grille") {
            int row = 0;
            QString rowString;
            refreshTaille();

            while (reader->readNextStartElement() && row <= dimensions) {
                rowString = reader->readElementText();

                for (unsigned int i = 0; i < rowString.length(); i++) {
                    if (rowString[i] == "1") {
                        ui->grille->item(row, i)->setBackground(Qt::black);
                    } else {
                        ui->grille->item(row, i)->setBackground(Qt::white);
                    }
                }
                row++;
            }
        }
        else {
            reader->skipCurrentElement();
        }
    }
}

