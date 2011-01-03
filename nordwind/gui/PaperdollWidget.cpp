#include "PaperdollWidget.hpp"
#include "ui_PaperdollWidget.h"

Paperdoll::Paperdoll(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Paperdoll)
{
    ui->setupUi(this);
}

Paperdoll::~Paperdoll()
{
    delete ui;
}
