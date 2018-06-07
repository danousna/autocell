#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

    // Connect code
    connect(ui->numeroInput, SIGNAL(valueChanged(int)), this, SLOT(synchronizeNumToNumBit(int)));

    for (unsigned int i = 0; i < 8; ++i) {
        numeroBit[i]->setValidator(zeroOneValidator);
        connect(numeroBit[i], SIGNAL(textChanged(QString)), this, SLOT(synchronizeNumBitToNum(QString)));
    }

    // Set table view
    for(unsigned int counter = 0; counter < 2; ++counter) {
        ui->table_etats_depart->setColumnWidth(counter, 40);
        ui->table_etats_depart->setItem(0, counter, new QTableWidgetItem(""));
        ui->table_etats_depart->item(0, counter)->setBackgroundColor("white");
        ui->table_etats_depart->item(0, counter)->setTextColor("white");
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::synchronizeNumToNumBit(int i)
{
    std::string numBit = NumToNumBit(i);
    for (unsigned int i = 0; i < numBit.size(); ++i)
    {
        numeroBit[i]->setText((QString) numBit[i]);
    }
}

void MainWindow::synchronizeNumBitToNum(const QString& s)
{
    if (s == "") return;

    std::string string;

    for (unsigned int i = 0; i < 8; ++i)
    {
        string += numeroBit[i]->text().toStdString();
    }

    short unsigned int numero = NumBitToNum(string);

    ui->numeroInput->setValue(numero);
}
