#ifndef LOGINDIALOG_HPP
#define LOGINDIALOG_HPP

#include "ui_LoginDialog.h"

namespace gui {

class LoginDialog : public QDialog, private Ui::LoginDialog {
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    QString password() const;
    QString account() const;
public slots:
    void accept();
signals:
    void authenticated(const QString& account,const QString& password);
protected:
    void changeEvent(QEvent *e);
};

}

#endif // LOGINDIALOG_HPP
