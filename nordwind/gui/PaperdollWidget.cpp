#include "PaperdollWidget.hpp"
#include "ui_PaperdollWidget.h"

PaperdollWidget::PaperdollWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaperdollWidget)
{
    ui->setupUi(this);
}

PaperdollWidget::~PaperdollWidget()
{
    delete ui;
}
