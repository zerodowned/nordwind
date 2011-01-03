#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <QMainWindow>
#include <QVector3D>

namespace gui {
class Client : public QMainWindow
{
    Q_OBJECT;
    Q_PROPERTY(QString shard READ objectName);
public:
    Client(const QString& name);

    bool load();
public slots:
    void startup();
    void ingame(quint32 serial, quint16 body, quint8 direction, const QVector3D &pos, const QSize& map);
    void addChild(QWidget *widget,Qt::WindowFlags windowFlags = 0);
    void onMessage(const QString& message, QString from);
    void onError(const QString& message, QString from);
signals:
    void loadFinished();
    void authenticated(const QString& account, const QString& password);
private:
    QObject* mNetwork;
};
}

#endif // CLIENT_HPP
