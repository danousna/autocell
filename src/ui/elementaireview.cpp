#include "elementaireview.h"
#include "ui_elementaireview.h"

ElementaireView::ElementaireView(QWidget *parent): 
AutomateView(parent, 40, 20, 0, 100, true, AutomateElementaire::getInstance(30)), ui(new Ui::ElementaireView), 
taille(24) {
    ui->setupUi(this);

    srand(time(NULL));

    // UI Speed
    connect(ui->inputSpeed, SIGNAL(valueChanged(int)), this, SLOT(changeSpeed(int)));
    ui->inputSpeed->setValue(speed);

    // UI Taille
    connect(ui->btnRefreshTaille, SIGNAL(clicked()), this, SLOT(refreshTaille()));
    ui->inputTaille->setValue(taille);
    ui->inputTailleCell->setValue(tailleCell);

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
    connect(ui->inputNumero, SIGNAL(valueChanged(int)), this, SLOT(synchronizeNumToNumBit(int)));
    ui->inputNumero->setValue(30);

    for (unsigned int i = 0; i < 8; i++) {
        numeroBit[i]->setValidator(zeroOneValidator);
        connect(numeroBit[i], SIGNAL(textChanged(QString)), this, SLOT(synchronizeNumBitToNum(QString)));
    }

    // UI Steps
    connect(ui->inputSteps, SIGNAL(valueChanged(int)), this, SLOT(changeSteps(int)));
    ui->inputSteps->setValue(steps);

    // UI Gen
    connect(ui->btnGenRandom, SIGNAL(clicked()), this, SLOT(randomGen()));

    // UI Symetrie
    connect(ui->btnSymetrie, SIGNAL(clicked()), this, SLOT(symetrie()));

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
    if (ui->grilleDepart->item(0, item->column())->backgroundColor() == Qt::white) {
        ui->grilleDepart->item(0, item->column())->setBackground(Qt::black);
    }
    // Cas contraire 
    else {
        ui->grilleDepart->item(0, item->column())->setBackground(Qt::white);
    }
}

void ElementaireView::next() {
    Grille1D g(taille);
    this->syncGrilles(&g, ui->grilleDepart, 0, false);
    Simulateur s(*automate, g, taille);

    for (int i = 0; i < steps; i++) {
        if (i <= stepState) {
            ui->stepsLabel->setText(QStringLiteral("%1 sur").arg(i + 1));
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
                ui->stepsLabel->setText(QStringLiteral("%1 sur").arg(i + 1));
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
    ui->stepsLabel->setText(QString("0 sur"));
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
                grilleQT->item(step, i)->setBackground(Qt::black);
            } else {
                grilleQT->item(step, i)->setBackground(Qt::white);
            }
        }
    } else {
        for (unsigned int i = 0; i < grilleAutomate->getTaille(); i++) {
            if (grilleQT->item(0, i)->background() == Qt::black) {
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
            ui->grille->item(i, j)->setBackground(Qt::white);
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
                grille->item(0, i)->setBackground(Qt::white);
            }
        }
    }
    // 2D 
    else {
        grille->setFixedSize(tCell * t + 2, tCell * t - (t/2 * tCell));

        for (unsigned int i = 0; i < r; ++i) {
            grille->setRowHeight(i, tCell);

            for (unsigned int j = 0; j < t; j++) {
                grille->setColumnWidth(j, tCell);
                grille->setItem(i, j, new QTableWidgetItem(""));
                grille->item(i, j)->setBackground(Qt::white);
            }
        }
    }
}

void ElementaireView::toggleUI() {
    bool enabled = ui->grilleDepart->isEnabled();

    ui->inputSpeed->setEnabled(!enabled);
    ui->inputTaille->setEnabled(!enabled);
    ui->btnRefreshTaille->setEnabled(!enabled);
    ui->inputNumero->setEnabled(!enabled);
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

    ui->inputNumero->setValue(numero);

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

    for (unsigned int i = 0; i < this->getTaille(); i++) {
        randZeroOne = rand() % 2;

        if (randZeroOne == 1) {
            ui->grilleDepart->item(0, i)->setBackground(Qt::black);
        } else {
            ui->grilleDepart->item(0, i)->setBackground(Qt::white);
        }
    }
}

void ElementaireView::symetrie() {
    int size = this->getTaille();
    for (int i = 0; i < size/2; i++) {
        ui->grilleDepart->item(0, size-i-1)->setBackground(ui->grilleDepart->item(0, i)->background());
    }
}

void ElementaireView::save(QFile* f, bool showDialog) {
    QDataStream output(f);
    output.setVersion(QDataStream::Qt_4_5);

    QXmlStreamWriter stream(output.device());
    stream.setAutoFormatting(true);
    stream.setCodec("UTF-8");

    stream.writeStartDocument();
    stream.writeStartElement("automate");

    // Config de l'automate
    stream.writeAttribute("type", "elementaire");
    stream.writeTextElement("numero", QString::number(ui->inputNumero->value()));
    stream.writeTextElement("vitesse", QString::number(ui->inputSpeed->value()));
    stream.writeTextElement("taille", QString::number(ui->inputTaille->value()));
    stream.writeTextElement("taillecell", QString::number(ui->inputTailleCell->value()));
    stream.writeTextElement("steps", QString::number(ui->inputSteps->value()));
    
    // Grille initiale
    QString row("");
    for (unsigned int i = 0; i < taille; i++) {
        if (ui->grilleDepart->item(0, i)->background() == Qt::black) {
            row += "1";
        } else {
            row += "0";
        }
    }
    stream.writeTextElement("grilledepart", row);
    
    // Grille
    stream.writeStartElement("grille");
    for (unsigned int i = 0; i < steps; i++) {
        QString label = QStringLiteral("row_%1").arg(i);
        QString row("");
        for (unsigned int j = 0; j < taille; j++) {
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

void ElementaireView::import(QXmlStreamReader* reader) {
    while (reader->readNextStartElement()) {
        if (reader->name() == "numero") {
            ui->inputNumero->setValue(reader->readElementText().toInt());
        }
        else if (reader->name() == "vitesse") {
            speed = reader->readElementText().toInt();
            ui->inputSpeed->setValue(speed);
        } 
        else if (reader->name() == "taille") {
            taille = reader->readElementText().toInt();
            ui->inputTaille->setValue(taille);
        } 
        else if (reader->name() == "taillecell") {
            tailleCell = reader->readElementText().toInt();
            ui->inputTailleCell->setValue(tailleCell);
        }
        else if (reader->name() == "steps") {
            steps = reader->readElementText().toInt();
            ui->inputSteps->setValue(steps);
        }
        else if (reader->name() == "grilledepart") {
            QString row = reader->readElementText();
            refreshTaille();

            for (unsigned int i = 0; i < row.length(); i++) {
                if (row[i] == "1") {
                    ui->grilleDepart->item(0, i)->setBackground(Qt::black);
                } else {
                    ui->grilleDepart->item(0, i)->setBackground(Qt::white);
                }
            }
        }
        else if (reader->isStartElement() && reader->name() == "grille") {
            int row = 0;
            QString rowString;

            while (reader->readNextStartElement() && row <= taille) {
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
