#include "ShardPage.hpp"

ShardPage::ShardPage(QWidget *parent) :
    QWizardPage(parent)
{
    setupUi(this);
    setCommitPage(true);
    setButtonText(QWizard::NextButton,tr("Select"));
    registerField("shard",shardList);
}

ShardPage::~ShardPage() {
}

void ShardPage::addShard(quint16 index, const QString &name, quint8 usage, quint8 timeZone, const QString& host) {
    QListWidgetItem* item = new QListWidgetItem(name,shardList);
    item->setData(ShardIndex,index);
    item->setData(ShardUsage,usage);
    item->setData(ShardTimeZone,timeZone);
    item->setData(ShardIPv4,host);
}

void ShardPage::on_shardList_currentRowChanged(int currentRow) {
    if(currentRow!=-1) {
        shardInfo->show();
        QListWidgetItem* item = shardList->item(currentRow);
        shardUsage->setValue(item->data(ShardUsage).toUInt());
        shardIP->setText(item->data(ShardIPv4).toString());
    } else
        shardInfo->hide();
}

void ShardPage::onNext() {
    emit login(field("account").toString(),field("password").toString());
}
