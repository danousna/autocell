#ifndef GOLVIEW_H
#define GOLVIEW_H

#include <thread>
#include <chrono>

#include <QWidget>
#include <QLineEdit>
#include <QIntValidator>
#include <QTableWidget>

#include "../simulateur.h"
#include "../automate2d.h"

namespace Ui {
class GoLView;
}

class GoLView : public QWidget
{
    Q_OBJECT
    Ui::GoLView *ui;
    int dimensions;
    int steps;
    int stepState;
    int speed;
    bool paused;

    // L'automate de cette view. C'est un singleton.
    AutomateGoL* automate;
public:
    explicit GoLView(QWidget *parent = 0);
    ~GoLView();
private slots:
};

#endif // GOLVIEW_H
