#ifndef AUTOMATEVIEW_H
#define AUTOMATEVIEW_H

#include <thread>
#include <chrono>
#include <stdlib.h>
#include <time.h>

#include <QWidget>
#include <QLineEdit>
#include <QIntValidator>
#include <QTableWidget>
#include <QMessageBox>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QFile>

#include "../simulateur.h"

class AutomateView : public QWidget
{
    Q_OBJECT
protected:
    int tailleCell;
    int steps;
    int stepState;
    int speed;
    bool paused;

    // L'automate de la view. C'est un singleton.
    Automate* automate;
public:
    explicit AutomateView(QWidget *parent = 0, int tCell = 40, int s = 21, int sS = 0, int vitesse = 100, bool p = true, Automate* a = 0);
    virtual ~AutomateView() {}
    
    virtual int getTaille() = 0;
    virtual void save(QFile* f, bool showDialog) = 0;
    virtual void import(QXmlStreamReader* reader) = 0;
private slots:
    virtual void toggleCell(QTableWidgetItem* item) = 0;
    virtual void next() = 0;
    virtual void play(int startStep = 0) = 0;
    virtual void togglePlayPause() = 0;
    virtual void reset() = 0;
    virtual void refreshTaille() = 0;
};

#endif // AUTOMATEVIEW_H