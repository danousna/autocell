#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
QMainWindow(parent), ui(new Ui::MainWindow),
elementaireAutomate(new ElementaireView(parent)), golAutomate(new GoLView(parent)), wwAutomate(new WWView(parent)) {
    ui->setupUi(this);

    ui->menuBar->setNativeMenuBar(false);

    // Connection des save/import aux dialogues box.
    connect(ui->actionImporter, SIGNAL(triggered()), this, SLOT(showImportDialog()));
    connect(ui->actionEnregistrer, SIGNAL(triggered()), this, SLOT(showSaveDialog()));

    // Connection des actions aux fenetres
    connect(ui->actionElementaire, SIGNAL(triggered()), this, SLOT(showFenetreAutomateElementaire()));
    connect(ui->actionGoL, SIGNAL(triggered()), this, SLOT(showFenetreAutomateGoL()));
    connect(ui->actionWireworld, SIGNAL(triggered()), this, SLOT(showFenetreAutomateWireworld()));

    // Ajout des interfaces des automates au stackedWidget.
    ui->stackedWidget->insertWidget(0, elementaireAutomate);
    ui->stackedWidget->insertWidget(1, golAutomate);
    ui->stackedWidget->insertWidget(2, wwAutomate);

    loadAppState();
}

MainWindow::~MainWindow() {
    saveAppState();
    delete ui;
}

void MainWindow::saveAppState() {
    QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QDir dir(path);

    if (!dir.exists()) {
        dir.mkpath(path);
    }

    QFile fileElementaire(path + "/elementaire.xml");
    if (fileElementaire.open(QIODevice::WriteOnly)) {
        elementaireAutomate->save(&fileElementaire, false);
    }

    QFile fileGoL(path + "/gol.xml");
    if (fileGoL.open(QIODevice::WriteOnly)) {
        golAutomate->save(&fileGoL, false);
    }

    QFile fileWW(path + "/ww.xml");
    if (fileWW.open(QIODevice::WriteOnly)) {
        wwAutomate->save(&fileWW, false);
    }
}

void MainWindow::loadAppState() {
    QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);

    QFile fileElementaire(path + "/elementaire.xml");
    if (fileElementaire.open(QIODevice::ReadOnly)) {
        QDataStream in(&fileElementaire);
        in.setVersion(QDataStream::Qt_4_5);
        QXmlStreamReader xml(in.device());
        import(xml);
    }

    QFile fileGoL(path + "/gol.xml");
    if (fileGoL.open(QIODevice::ReadOnly)) {
        QDataStream in(&fileGoL);
        in.setVersion(QDataStream::Qt_4_5);
        QXmlStreamReader xml(in.device());
        import(xml);
    }

    QFile fileWW(path + "/ww.xml");
    if (fileWW.open(QIODevice::ReadOnly)) {
        QDataStream in(&fileWW);
        in.setVersion(QDataStream::Qt_4_5);
        QXmlStreamReader xml(in.device());
        import(xml);
    }
}

void MainWindow::showImportDialog() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Ouvrir un automate"), "/", tr("Fichier XML (*.xml);;Tous les fichiers (*)"));

    if (filename.isEmpty()) {
        return;
    } else {
        QFile file(filename);

        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Impossible d'ouvrir le fichier."), file.errorString());
            return;
        }

        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_4_5);

        QXmlStreamReader xml(in.device());

        import(xml);
    }
}

void MainWindow::import(QXmlStreamReader& reader) {
    if(reader.readNextStartElement()) {
        if (reader.name() == "automate" && reader.attributes().hasAttribute("type")) {
            QString type = reader.attributes().value("type").toString();

            if (type == "elementaire") {
                elementaireAutomate->import(&reader);
                ui->stackedWidget->setCurrentIndex(0);
            } 
            else if (type == "gol") {
                golAutomate->import(&reader);
                ui->stackedWidget->setCurrentIndex(1);
            } 
            else if (type == "ww") {
                wwAutomate->import(&reader);
                ui->stackedWidget->setCurrentIndex(2);
            }
            else QMessageBox::information(this, tr("Erreur"), tr("Type d'automate non reconnu."));
        } else QMessageBox::information(this, tr("Fichier non valide"), tr("Ce fichier est incompatible avec AutoCell."));
    } else QMessageBox::information(this, tr("Fichier non valide"), tr("Ce fichier est incompatible avec AutoCell."));
}

void MainWindow::showSaveDialog() {
    QString filename = QFileDialog::getSaveFileName(this, tr("Enregistrer l'automate"), "/", tr("Fichier XML (*.xml)"));

    if (filename.isEmpty()) {
        return;
    } else {
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Impossible d'ouvrir le fichier."), file.errorString());
            return;
        }

        if (ui->stackedWidget->currentIndex() == 0) {
            elementaireAutomate->save(&file, true);
        } 
        else if (ui->stackedWidget->currentIndex() == 1) {
            golAutomate->save(&file, true);
        } 
        else if (ui->stackedWidget->currentIndex() == 2) {
            wwAutomate->save(&file, true);
        } 
        else {
            return;
        }
    }
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
