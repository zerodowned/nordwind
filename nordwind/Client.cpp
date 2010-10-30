#include "Client.hpp"
#include "Settings.hpp"

#include "Data.hpp"
#include "data/IndexFileHandle.hpp"
#include "data/MulFileEngine.hpp"
#include "data/TileData.hpp"

#include "game/Scene.hpp"

#include "network/LoginSocket.hpp"

#include <qmainwindow.h>
#include <qmdiarea.h>
#include <qmdisubwindow.h>
#include "gui/Login.hpp"

#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <qsplashscreen.h>
#include <qfiledialog.h>
#include <qdir.h>
#include <qdatetime.h>
#include <qdebug.h>
#include <qpixmapcache.h>

Client::Client(int& argc, char** argv)
: QApplication(argc,argv,QApplication::GuiClient) {
	setObjectName("Client");
	setApplicationName( QApplication::tr("Nordwind") );
	setApplicationVersion(  QApplication::tr("Alpha") );
	setOrganizationDomain(  QApplication::tr("idstein.info") );
	setOrganizationName(  QApplication::tr("Idstein") );
	connect(this,SIGNAL(aboutToQuit()),SLOT(cleanup()));
}

Client::~Client(void) {
}

bool Client::initialize() {
	QTime time;
	qDebug() << "Initializing load sequence";
	QSplashScreen splashscreen(QPixmap(":/7w.png"));
	splashscreen.show();

	splashscreen.showMessage("Reading configuration...",Qt::AlignCenter);
	qApp->processEvents();
	Settings::setPointer(new Settings());
	Settings& settings = Settings::instance();
	QString folderPath = settings.value("path").toString();
	if(folderPath.isEmpty())
		folderPath = QFileDialog::getExistingDirectory(NULL,tr("Select UO Data folder"),QDir::currentPath());
	QDir path(folderPath);

	splashscreen.showMessage("Initializing data...",Qt::AlignCenter);
	qApp->processEvents();
	// Increase pixmap cache
	// TODO set variable in settings!
	QPixmapCache::setCacheLimit(102400); // Cache to 100 MB
	new data::TileData(settings.value("tiledata","tiledata.mul").toString());
	settings.beginGroup("muls");
	time.start();
	Q_FOREACH(QString type, settings.childGroups()) {
		settings.beginGroup(type);
		QString mulFile = settings.value("file").toString();
		QString indexFile = settings.value("index").toString();
		if(!indexFile.isNull())
			data::IndexFileHandle::instance(mulFile,indexFile);
		if(!QFile::exists(QString("%1:%2").arg(settings.value("file").toString()).arg(0)))
			qWarning() << "Empty index 0 in" << indexFile;
		settings.endGroup();
		qDebug() << type << "loaded with MulFileEngine took aprox." << time.restart() << "ms";
	}
	settings.endGroup();
	settings.beginGroup("facets");
	Q_FOREACH(QString facet, settings.childGroups()) {
		qDebug() << "Construct facet" << facet;
		new game::Scene(facet,
		settings.value(facet +"/map","map0.mul").toString(),
		settings.value(facet +"/data","statics0.mul").toString(),
		settings.value(facet +"/index","staidx0.mul").toString(),
		settings.value(facet +"/dimension",QSize(768,512)).toSize()
		);
	}
	settings.endGroup();

	splashscreen.showMessage("Creating network context...",Qt::AlignCenter);
	qApp->processEvents();
        new network::LoginSocket(settings.value("LoginServer","localhost").toString(),settings.value("LoginPort",2593).toUInt());

	splashscreen.showMessage("Creating GUI context...",Qt::AlignCenter);
	qApp->processEvents();
	mGUI = new QMainWindow;
	mGUI->resize( 640, 480 );
	mGUI->setCentralWidget(new QMdiArea);

    gui::Login* login = new gui::Login;
    addChild(login,0);

    splashscreen.finish(mGUI);
    mGUI->show();

	return true;
}

void Client::cleanup() {
	Q_FOREACH(QGraphicsScene* scene,findChildren<QGraphicsScene*>()) {
		if(scene)
			delete scene;
	}
}

void Client::setAccount(const QString& account) {
	mAccount = account;
}

void Client::setPassword(const QString& password) {
	mPassword = password;
}

void Client::addChild(QWidget *widget,Qt::WindowFlags windowFlags) {
	QMdiArea* area = qobject_cast<QMdiArea*>(mGUI->centralWidget());
	if(!area)
		qFatal("Central widget is not a QMdiArea");
        QMdiSubWindow* sub = area->addSubWindow(widget,windowFlags);
        widget->setAttribute(Qt::WA_DeleteOnClose);
        connect(widget,SIGNAL(destroyed()),sub,SLOT(close()));
	sub->show();
}

void Client::connectionEstablished() {
    emit login(mAccount,mPassword);
}

void Client::error() {
    QAbstractSocket* socket = qobject_cast<QAbstractSocket*>(sender());
    if(socket)
        QMessageBox::warning(mGUI,tr("Network Error %1 - %2:%3").arg(socket->objectName()).arg(socket->peerName()).arg(socket->peerPort()),socket->errorString());
}

void Client::networkState(QAbstractSocket::SocketState socketState) {
    QString message;
    QAbstractSocket* socket = qobject_cast<QAbstractSocket*>(sender());
    switch(socketState) {
        case QAbstractSocket::UnconnectedState:
            message = tr("Not connected");
            break;
        case QAbstractSocket::HostLookupState:
            message = tr("Looking for host %1:%2").arg(socket->peerName()).arg(socket->peerPort());
            break;
    case QAbstractSocket::ConnectingState:
            message = tr("Connecting to host %1:%2").arg(socket->peerName()).arg(socket->peerPort());
            break;
    case QAbstractSocket::ConnectedState:
            message = tr("Connected to host %1:%2").arg(socket->peerName()).arg(socket->peerPort());
            break;
    case QAbstractSocket::ClosingState:
            message = tr("Closing connection host %1:%2").arg(socket->peerName()).arg(socket->peerPort());
            break;
    default:
            message = tr("No network activity.");
    }
    mGUI->statusBar()->showMessage(message);
}

int main(int argc, char *argv[]) {
	Client client(argc,argv);
	data::MulFileEngineHandler mulEngineHandler;
    return client.initialize() && client.exec();
}

/*
void CClient::shutdown(quint32 _msecs) {
	setState( eCS_ShutDown, true );
	deinitialize(_msecs);
}*/
//
//bool Client::load() {
//	QPixmap pix("loading.bmp");
//	QSplashScreen info( pix, Qt::WindowStaysOnTopHint );
//	info.setMask(pix.createMaskFromColor( QColor( 0,0,0 )));
//	info.show();
//	if(!IModule::load())
//		return false;
//	/*
//	setState( eCS_Gui, loadGui(_msecs));
//	setState( eCS_Sound, loadSound(_msecs));
//	setState( eCS_World, loadWorld(_msecs));
//	setState( eCS_Network, loadNetwork(_msecs));*/
//	info.showMessage( "Loading resource interface...", Qt::AlignCenter );
//	processEvents();
//	m_interfaces = new CInterfaces( (QApplication*)this);
//	Q_CHECK_PTR(m_interfaces);
//	if(!m_interfaces->load())
//		qWarning() << "Faild to initialize interface loader";
//
//	Q_CHECK_PTR(m_interfaces);
//	m_resource = qobject_cast<resource::IResource*>(m_interfaces->load( "resource" ));
//	if(!m_resource||!m_resource->load()) {
//		info.showMessage( "Resource initialization failed!", Qt::AlignCenter );
//		qFatal("Couldn't initialize resource interface.");
//	}
//
//	info.showMessage( "Interface load done.", Qt::AlignCenter );
//	processEvents();
//	m_gui = new gui::CGui();
//	Q_CHECK_PTR(m_gui);
//	if(!m_gui->load())
//		qWarning() << "Faild to initialize gui";
//	info.showMessage( "Starting...", Qt::AlignCenter );
//	processEvents();
//	return true;
//}
//
//bool Client::unload() {
//	if(m_gui)
//		delete m_gui;
//	if(m_interfaces)
//		delete m_interfaces;
//	/*
//	unloadNetwork(_msecs);
//	unloadWorld(_msecs);
//	unloadSound(_msecs);
//	unloadGui(_msecs);
//	unloadCore(_msecs);*/
//	return true;
//}
//
//bool Client::reload() {
//	return( unload() && load() );
//}
//
//resource::IResource* Client::resource() const {
//	return m_resource;
//}
/*
bool loadCore(quint32& _msecs) {
	setState( eCS_InitCore, true );
	// wait unteil _msecs is zero
	// otherwise refresh _msecs
	// on zero report error
	m_core = core::CCore::getPointer();
	return (m_core) ? true : false;
}

void unloadCore(quint32& _msecs) {
	if(isState( eCS_Core )) {
		m_core::destroyInstance();
		m_core = NULL;
		setState( eCS_Core, false );
		setState( eCS_LoadingCore, false );
		setState( eCS_InitCore, false );
	} else {
		if(isState( eCS_LoadingCore)) {
			// request status report from core
			// CStatus status_core;
			// m_core->status( CStatus& status_core );
			// wait 200 Msec
			// check result
			// core shutdown after 5000 msec without acceptable response
		} else {
			if(isState( eCS_InitCore )) {
				// failure while core initializiation. Out of Memory?
				// Process paused?
				// system overload?
			} else {
				// nothing to do, maybe report
			}
		}
	}
}

bool CClient::loadGui(quint32& _msecs) {
	setState( eCS_InitGui, true );
	// s.above. loadCore
	m_gui = gui::CGui::getPointer();
	return (m_gui) ? true : false;
}

void CClient::undloadGui(quint32& _msecs) {
	if(isState( eCS_Gui)) {
		m_gui::destroyInstance();
		m_gui = NULL;
		setState( eCS_Gui, false );
		setState( eCS_InitGui, false );
		setState( eCS_LoadingGui, false );
	} else {
		if(isState(eCS_LoadingGui)) {
		} else {
			if(isState(eCS_InitGui)) {
			} else {
				// nothing to do, maybe report
			}
		}
	}
}

bool CClient::loadSound(quint32& _msecs) {
	setState( eCS_InitSound );
	m_sound = new sound::CSoundHandler();
	setState( eCS_Sound );
}

QGLWidget* CClient::gui() {
	return reinterpret_cast<QGLWidget*>(m_gui);
}

core::CCore* CClient::core() const {
	return m_core;
}*/
//
//qint32 Client::status() {
//	return 0;
//}
//
//QString Client::statusDesc( qint32 _status ) {
//	return "No Error";
//}
//
//QString Client::statusInfo( ) {
//	return "No Error";
//}
//
//const QString& Client::version() {
//	return applicationVersion();
//}
//
//const QString Client::name() {
//	return QString("client");
//}
