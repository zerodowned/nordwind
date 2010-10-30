#include "Login.hpp"
#include "ui_Login.h"
#include <qabstractsocket.h>

using namespace gui;

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    connect(ui->mAccountInput,SIGNAL(textChanged(QString)),qApp,SLOT(setAccount(QString)));
    connect(ui->mPasswordInput,SIGNAL(textChanged(QString)),qApp,SLOT(setPassword(QString)));
    connect(ui->mLoginButton,SIGNAL(clicked()),qApp->findChild<QAbstractSocket*>("LoginServer"),SLOT(establishConnection()));
    connect(ui->mLoginButton,SIGNAL(clicked()),SLOT(close()));
}

Login::~Login()
{
    delete ui;
}
