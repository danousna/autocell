#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QIntValidator>
#include <QTableWidget>
#include "../automate1d.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
    Ui::MainWindow *ui;
    QIntValidator* zeroOneValidator;
    QLineEdit* numeroBit[8];
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();    
private slots:
    void synchronizeNumToNumBit(int i);
    void synchronizeNumBitToNum(const QString& s);
};

#endif // MAINWINDOW_H
