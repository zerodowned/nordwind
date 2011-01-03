#include "LoginDialog.hpp"

#include <QSettings>

using namespace gui;

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent){
    setupUi(this);
    QSettings* settings = qApp->findChild<QSettings*>();
    mAccount->setText(settings->value("Account").toString());
    mPassword->setText(settings->value("Password").toString());
    setModal(true);
}

void LoginDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void LoginDialog::accept() {
    QDialog::accept();
    emit authenticated(account(),password());
}

QString LoginDialog::password() const {
    return mPassword->text();
}

QString LoginDialog::account() const {
    return mAccount->text();
}
