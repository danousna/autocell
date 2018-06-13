#ifndef GOLVIEW_H
#define GOLVIEW_H

#include <QWidget>

namespace Ui {
class GoLView;
}

class GoLView : public QWidget
{
    Q_OBJECT

public:
    explicit GoLView(QWidget *parent = 0);
    ~GoLView();

private:
    Ui::GoLView *ui;
};

#endif // GOLVIEW_H
