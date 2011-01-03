#ifndef SHARDPAGE_HPP
#define SHARDPAGE_HPP

#include <QWizardPage>
#include "ui_ShardPage.h"

class ShardPage : public QWizardPage, public Ui::ShardPage
{
    Q_OBJECT;
public:
    enum ShardData {
        ShardIndex  = Qt::UserRole,
        ShardUsage  = Qt::UserRole + 1,
        ShardIPv4   = Qt::UserRole + 2,
        ShardTimeZone = Qt::UserRole + 3,
    };
    explicit ShardPage(QWidget *parent = 0);
    ~ShardPage();
Q_SIGNALS:
    void login(const QString& account, const QString& password);
public Q_SLOTS:
    void onNext();
    void addShard(quint16 index, const QString& name, quint8 usage, quint8 timeZone, const QString& ip);
    void on_shardList_currentRowChanged(int currentRow);
};

#endif // SHARDSLIST_HPP
