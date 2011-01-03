#include "CharacterListDialog.hpp"
#include <QListWidgetItem>

CharacterListDialog::CharacterListDialog(QWidget *parent) :
        QDialog(parent){
    setupUi(this);
    setModal(true);
}

void CharacterListDialog::changeEvent(QEvent *e)
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

void CharacterListDialog::addCharacter(const QString &name, const QString &password) {
    QListWidgetItem* character = new QListWidgetItem(name,mCharacterList);
    character->setData(Qt::UserRole,password);
}

void CharacterListDialog::accept() {
    QListWidgetItem* character = mCharacterList->currentItem();
    if(character) {
        QDialog::accept();
        emit characterChosen(character->text(),character->data(Qt::UserRole).toString(),mCharacterList->currentRow());
    }
}
