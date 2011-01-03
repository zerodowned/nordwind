#include "Client.hpp"

#include <QSettings>
#include <QDir>
#include <QPluginLoader>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QPoint>
#include <QStatusBar>
#include <QMessageBox>
#include <QTime>

#include "game/TileData.hpp"
#include "game/Scene.hpp"
#include "LoginDialog.hpp"
#include "CharacterListDialog.hpp"
#include "ViewWidget.hpp"

using namespace gui;

Client::Client(const QString &name)
    : QMainWindow() {
    setObjectName(name);
    connect(this,SIGNAL(loadFinished()),SLOT(startup()));
    QMdiArea* area = new QMdiArea();
    area->setBackground(Qt::black);
    //area->setViewport(new QGLWidget);
    setCentralWidget(area);
    centralWidget()->hide();
}

bool Client::load() {
        QSettings* settings = qApp->findChild<QSettings*>();
        settings->beginGroup(objectName());
        QDir path(settings->value("path").toString());

        game::TileData* tileData = new game::TileData(this);
        tileData->load(path.filePath(settings->value("tiledata","tiledata.mul").toString()));
        QDir::addSearchPath("arts",path.filePath("art.mul"));
        QDir::addSearchPath("textures",path.filePath("texmaps.mul"));
        QDir::addSearchPath("animations",path.filePath("anim.mul"));

        qDebug() << "Creating GUI context...";
        //centralWidget()->resize(settings->value("GamePlayWindowSize",QSize(640, 480)).toSize() );

        qDebug() << "Creating network context...";
        QPluginLoader pluginLoader("libNetwork.dylib");
        mNetwork = pluginLoader.instance();
        mNetwork->setParent(this);
        connect(mNetwork,SIGNAL(error(QString,QString)),SLOT(onError(QString,QString)));
        connect(mNetwork,SIGNAL(networkStateChanged(QString,QString)),SLOT(onMessage(QString,QString)));
        settings->beginGroup("Server");
        mNetwork->setProperty("host",settings->value("Host","127.0.0.1"));
        mNetwork->setProperty("port",settings->value("Port",2593));
        settings->endGroup();

#ifdef QT_DEBUG
        settings->beginGroup("facets");
        Q_FOREACH(QString facet, settings->childGroups()) {
                qDebug() << "Construct facet" << facet;
                settings->beginGroup(facet);
                game::Scene* scene = new game::Scene(path.filePath(settings->value("Map","map0.mul").toString()),
                    path.filePath(settings->value("Statics","statics0.mul").toString()),
                    path.filePath(settings->value("Index","staidx0.mul").toString()),
                    this,
                    settings->value("Dimension",QSize(768,512)).toSize()
                );
                scene->setObjectName(facet);
                scene->enableNetwork(mNetwork);
                settings->endGroup();
        }
        settings->endGroup();
#endif

        settings->endGroup();
        emit loadFinished();
        return true;
}

void Client::startup() {
    show();

    LoginDialog* login = new LoginDialog(this);
    CharacterListDialog* characterListDialog = new CharacterListDialog(this);

    connect(login,SIGNAL(rejected()),SLOT(close()));

    mNetwork->connect(login,SIGNAL(authenticated(QString,QString)),SLOT(connectToHost(QString,QString)));
    mNetwork->connect(characterListDialog,SIGNAL(characterChosen(QString,QString,quint32)),SLOT(sendLoginCharacter(QString,QString,quint32)));

    // The host disconnects when we're relayed to the game server!
    //login->connect(mNetwork,SIGNAL(disconnected()),SLOT(show()));
    login->connect(mNetwork,SIGNAL(error(QAbstractSocket::SocketError)),SLOT(show()));
    login->connect(mNetwork,SIGNAL(loginCompleted()),SLOT(hide()));
    login->connect(characterListDialog,SIGNAL(rejected()),SLOT(show()));


    characterListDialog->connect(mNetwork,SIGNAL(characterReceived(QString,QString)),SLOT(show()));
    characterListDialog->connect(mNetwork,SIGNAL(characterReceived(QString,QString)),SLOT(addCharacter(QString,QString)));
    characterListDialog->connect(mNetwork,SIGNAL(loginCompleted()),SLOT(hide()));

    centralWidget()->connect(mNetwork,SIGNAL(loginCompleted()),SLOT(show()));



    login->show();

}

void Client::ingame(quint32 serial, quint16 body, quint8 direction, const QVector3D &pos, const QSize& map) {
    game::Scene* scene = findChild<game::Scene*>();
        if(scene) {
            QGraphicsItem* character = scene->updateMobile(serial,body,direction,pos,0,0,0);
            QTime timer;
            timer.start();
            QRect mapRect(QPoint(0,0),QSize(32,32));
            mapRect.moveCenter(pos.toPoint());
            scene->loadMap(mapRect);
            qDebug() << "Load done in" << timer.restart() << "ms";
            ViewWidget* view = new ViewWidget(scene);
            view->centerOn(character);
            addChild(view);
    }
}

void Client::addChild(QWidget *widget,Qt::WindowFlags windowFlags) {
    QMdiArea* area = qobject_cast<QMdiArea*>(centralWidget());
    if(!area)
        qFatal("Central widget is not a QMdiArea");
    QSize size = widget->size();
    QMdiSubWindow* sub = area->addSubWindow(widget,windowFlags);
    sub->setGeometry(QRect(QPoint(0,0),size));
    widget->setAttribute(Qt::WA_DeleteOnClose);
    connect(widget,SIGNAL(destroyed()),sub,SLOT(close()));
}

void Client::onMessage(const QString& message, QString from) {
    if(from.isNull())
        from = sender()->objectName();
    statusBar()->showMessage(tr("%1 - %2").arg(message).arg(from));
}

void Client::onError(const QString &message, QString from) {
    if(from.isNull())
        from = sender()->objectName();
    QMessageBox::critical(this,tr("Error"),tr("%1 - %2").arg(message).arg(from));
}

//void Client::processPlayMusc(quint16 id) {
//    qDebug() << "Music" << id;
//}
