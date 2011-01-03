#ifndef CHARACTERLISTDIALOG_HPP
#define CHARACTERLISTDIALOG_HPP

#include "ui_CharacterListDialog.h"

class CharacterListDialog : public QDialog, private Ui::CharacterListDialog
{
    Q_OBJECT

public:
    explicit CharacterListDialog(QWidget *parent = 0);
public slots:
    void accept();
    void addCharacter(const QString& name, const QString& password);
    // TODO complete addCity
    //void addCity(quint8 index, const QString& name, const QString& area);
signals:
    void characterChosen(const QString& name, const QString& password, quint32 index);
    // TODO characterCreated!
    // TODO characterDeleted!
protected:
    void changeEvent(QEvent *e);
};

#endif // CHARACTERLISTDIALOG_HPP
