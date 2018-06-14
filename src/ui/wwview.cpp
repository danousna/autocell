#include "wwview.h"
#include "ui_wwview.h"

WWView::WWView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WWView)
{
    ui->setupUi(this);
}

WWView::~WWView()
{
    delete ui;
}
