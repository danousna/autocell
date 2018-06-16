#include "wwview.h"
#include "ui_wwview.h"

WWView::WWView(QWidget *parent): 
AutomateView(parent, 40, 20, 0, 100, true, AutomateWW::getInstance()), ui(new Ui::WWView),
dimensions(24) {
    ui->setupUi(this);

    srand(time(NULL));

    // Assez bizarrement, le constructeur de AutomateView avec AutomateWW::getInstance()
    // pour le membre automate ne fonctionne pas. On est obligé de faire une affectation 
    // sinon automate est un pointeur null.
    automate = AutomateWW::getInstance();

    // UI Speed
    connect(ui->inputSpeed, SIGNAL(valueChanged(int)), this, SLOT(changeSpeed(int)));
    ui->inputSpeed->setValue(speed);

    // UI Taille
    connect(ui->btnRefreshTaille, SIGNAL(clicked()), this, SLOT(refreshTaille()));
    ui->inputDimensions->setValue(dimensions);
    ui->inputTailleCell->setValue(tailleCell);

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

WWView::~WWView()
{
    delete ui;
}

void WWView::toggleCell(QTableWidgetItem* item) {
    // Si une cellule était vide (noire), on la rend conductrice.
    if (ui->grille->item(item->row(), item->column())->background() == Qt::black) {
        ui->grille->item(item->row(), item->column())->setBackground(Qt::yellow);
    }
    // Si elle était conductrice (jaune), on la rend tête d'électron.
    else if (ui->grille->item(item->row(), item->column())->background() == Qt::yellow) {
        ui->grille->item(item->row(), item->column())->setBackground(Qt::blue);
    }
    // Si elle était tête (bleue), on la rend queue.
    else if (ui->grille->item(item->row(), item->column())->background() == Qt::blue) {
        ui->grille->item(item->row(), item->column())->setBackground(Qt::red);
    }
    // Cas restant : elle était queue, on la rend vide
    else {
        ui->grille->item(item->row(), item->column())->setBackground(Qt::black);
    }
}

void WWView::next() {
    Grille2D g(dimensions);
    
    try {
        this->syncGrilles(&g, ui->grille, false); 
    } catch (AutoCellException* e) {
        std::cout << e->getInfo();
    }
    
    Simulateur s(*automate, g, dimensions);

    for (int i = 0; i < steps; i++) {
        if (i <= stepState) {
            ui->stepsLabel->setText(QStringLiteral("%1 sur").arg(i + 1));
            
            try {
                this->syncGrilles(&s.dernier(), ui->grille, true);
                s.next();
            } catch (AutoCellException* e) {
                std::cout << e->getInfo();
            }
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
                    grilleQT->item(j, i)->setBackground(Qt::black);
                } 
                else if (grilleAutomate->getCellVal(i, j) == 1) {
                    grilleQT->item(j, i)->setBackground(Qt::yellow);
                } 
                else if (grilleAutomate->getCellVal(i, j) == 2) {
                    grilleQT->item(j, i)->setBackground(Qt::blue);
                } 
                else if (grilleAutomate->getCellVal(i, j) == 3) {
                    grilleQT->item(j, i)->setBackground(Qt::red);
                }
                else throw new AutoCellException("Une cellule a une valeur illégale.");
            }
        }
    } else {
        QBrush tmp;
        for (unsigned int i = 0; i < grilleAutomate->getTaille(); i++) {
            for (unsigned int j = 0; j < grilleAutomate->getTaille(); j++) {
                tmp = grilleQT->item(j, i)->background();
                if (tmp == Qt::black) {
                    grilleAutomate->setCell(automate->getEtatsPossibles()[0], i, j);
                } else if (tmp == Qt::yellow) {
                    grilleAutomate->setCell(automate->getEtatsPossibles()[1], i, j);
                } else if (tmp == Qt::blue) {
                    grilleAutomate->setCell(automate->getEtatsPossibles()[2], i, j);
                } else if (tmp == Qt::red) {
                    grilleAutomate->setCell(automate->getEtatsPossibles()[3], i, j);
                } else throw new AutoCellException("Une cellule a une couleur illégale.");
            }
        }
    }
}

void WWView::viderGrille() {
    for (unsigned int i = 0; i < dimensions; i++) {
        for (unsigned int j = 0; j < dimensions; j++) {
            ui->grille->item(i, j)->setBackground(Qt::black);
        }
    }
}

void WWView::drawGrille(QTableWidget* grille, unsigned int tCell, unsigned int n) {
    grille->setColumnCount(n);
    grille->setRowCount(n);

    grille->setFixedSize(tCell * n + 2, tCell * n - 10 * tCell);

    for (unsigned int i = 0; i < n; ++i) {
        grille->setRowHeight(i, tCell);

        for (unsigned int j = 0; j < n; j++) {
            grille->setColumnWidth(j, tCell);
            grille->setItem(i, j, new QTableWidgetItem(""));
            grille->item(i, j)->setBackground(Qt::black);
            grille->item(i, j)->setTextColor(Qt::black);
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
                ui->grille->item(j, i)->setBackground(Qt::black);
            } else if (randZeroThree == 1) {
                ui->grille->item(j, i)->setBackground(Qt::yellow);
            } else if (randZeroThree == 2) {
                ui->grille->item(j, i)->setBackground(Qt::blue);
            } else {
                ui->grille->item(j, i)->setBackground(Qt::red);
            }
        }
    }
}

void WWView::save(QFile* f, bool showDialog) {
    QDataStream output(f);
    output.setVersion(QDataStream::Qt_4_5);

    QXmlStreamWriter stream(output.device());
    stream.setAutoFormatting(true);
    stream.setCodec("UTF-8");

    stream.writeStartDocument();
    stream.writeStartElement("automate");

    // Config de l'automate
    stream.writeAttribute("type", "ww");
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
                row += "0";
            }
            else if (ui->grille->item(i, j)->background() == Qt::yellow) {
                row += "1";
            }
            else if (ui->grille->item(i, j)->background() == Qt::blue) {
                row += "2";
            } 
            else if (ui->grille->item(i, j)->background() == Qt::red) {
                row += "3";
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

void WWView::import(QXmlStreamReader* reader) {
    while (reader->readNextStartElement()) {
        if (reader->name() == "vitesse") {
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
                    if (rowString[i] == "0") {
                        ui->grille->item(row, i)->setBackground(Qt::black);
                    } else if (rowString[i] == "1") {
                        ui->grille->item(row, i)->setBackground(Qt::yellow);
                    } else if (rowString[i] == "2") {
                        ui->grille->item(row, i)->setBackground(Qt::blue);
                    } else if (rowString[i] == "3") {
                        ui->grille->item(row, i)->setBackground(Qt::red);
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

void WWView::symetrie() {
    int size = this->getTaille();
    for (int i = 0; i < size/2; i++) {
        for (int j = 0; j < size/2; j++) {
            ui->grille->item(j, size-i-1)->setBackground(ui->grille->item(j, i)->background());
            ui->grille->item(size-j-1, size-i-1)->setBackground(ui->grille->item(j, i)->background());
            ui->grille->item(size-j-1, i)->setBackground(ui->grille->item(j, i)->background());
        }
    }
}
