#include "Nordwind.hpp"
#include "gui/Client.hpp"

#include <QtDebug>
#include <QSettings>
#include <QDir>
#include <QFileDialog>
#include <QPixmapCache>
#include <QtGlobal>
#include <QMessageBox>

#ifdef QT_DEBUG
#include <QToolBar>
#include <QAction>
#include <QKeySequence>
#include <Conan.h>
#endif

int main(int argc, char *argv[]) {
    // will only run with the QT library version has been built
    // QT_REQUIRE_VERSION(argc,argv,"4.7.0")

    Nordwind nw(argc,argv);
    nw.addLibraryPath(QDir::currentPath());
    qDebug() << nw.libraryPaths();
    if(!nw.staticMetaObject.invokeMethod(&nw,"initialize",Qt::QueuedConnection))
        qFatal("Unable to invoke initialize!");
    return nw.exec();
}

Nordwind::Nordwind(int& argc, char** argv) :
    QApplication(argc,argv),
    mSplashscreen(QPixmap(":7w.png"))
{
        setObjectName("Nordwind");
        setApplicationName( QApplication::tr("Nordwind") );
        setApplicationVersion(  QApplication::tr("Alpha") );
        setOrganizationDomain(  QApplication::tr("www.siebenwind.de") );
        setOrganizationName(  QApplication::tr("Siebenwind") );
        QSettings* settings = new QSettings("config.ini",QSettings::IniFormat,this);
        settings->setObjectName("Settings");
}

bool Nordwind::initialize() {
    try {
        QSettings* settings = findChild<QSettings*>();
    mSplashscreen.show();

#ifdef QT_DEBUG
    mSplashscreen.showMessage(tr("Initialize debug tools"));
    ConanWidget* signalSlotsDebug = new ConanWidget;
    signalSlotsDebug->AddRootObject(this);
    QAction* debugAction = new QAction(QIcon(":/debug.gif"),tr("Debugging"),this);
    debugAction->setShortcut(QKeySequence::Print);
    debugAction->setShortcutContext(Qt::ApplicationShortcut);
    debugAction->setToolTip(tr("Toogle Debug tools"));
    //debugAction->setCheckable(true);
    signalSlotsDebug->connect(debugAction,SIGNAL(triggered()),SLOT(show()));
#endif

    mSplashscreen.showMessage(tr("Initializing load sequence"));

    QPixmapCache::setCacheLimit(settings->value("PixmapCache",102400).toUInt()); // Cache to 100 MB
    setProperty("drawMapLines",true);
    QStringList shards = settings->childGroups();
    qDebug() << shards;
        switch(shards.size()) {
        case 0:
                        qWarning() << "No Shard found.";
                        mSplashscreen.showMessage(tr("Creating Default Shard ..."));
                        settings->beginGroup("Default");
                        settings->setValue("Path",QFileDialog::getExistingDirectory(NULL,tr("Select UO Data folder"),QDir::currentPath()));
                        settings->endGroup();
                        shards << "Default";
        case 1: {
                        mSplashscreen.showMessage(tr("Loading shard config"));
                        QString shardName = shards.first();
                        gui::Client* singleShard = new gui::Client(shardName);
                        singleShard->load();
#ifdef QT_DEBUG
                        QToolBar* debugTools = singleShard->addToolBar(tr("Debug Tools"));
                        debugTools->addAction(debugAction);
                        debugTools->show();
#endif
                        singleShard->show();
                        mSplashscreen.finish(singleShard);
                        break;
                    }
        default:
#ifdef QT_DEBUG
            QToolBar* tools = new QToolBar(tr("Tools"));
            tools->addAction(debugAction);
#endif
                        qDebug() << "Show Shard selection here.";
        }
   return true;
} catch (...) {
    qFatal("Unhandled error");
    return false;
}
}

void Nordwind::onError(const QString& error, QString name) {
    if(name.isNull())
        name = sender()->objectName();
    QMessageBox::warning(NULL,tr("Errror - %1").arg(name),error);
}
