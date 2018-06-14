#ifndef WWVIEW_H
#define WWVIEW_H

#include <QWidget>

namespace Ui {
class WWView;
}

class WWView : public QWidget
{
    Q_OBJECT

public:
    explicit WWView(QWidget *parent = 0);
    ~WWView();

private:
    Ui::WWView *ui;
};

#endif // WWVIEW_H
