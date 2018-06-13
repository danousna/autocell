#include "golview.h"
#include "ui_golview.h"

GoLView::GoLView(QWidget *parent):
QWidget(parent), ui(new Ui::GoLView),
automate(AutomateGoL::getInstance()) {
    ui->setupUi(this);
}

GoLView::~GoLView()
{
    delete ui;
}
