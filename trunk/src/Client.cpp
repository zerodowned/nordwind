#include "Client.hpp"

Client::Client( int argc, char** argv  ) 
: QApplication(argc, argv), mSettings(NULL) {
	setApplicationName( QApplication::tr("Windsegler") );
	setApplicationVersion(  QApplication::tr("Alpha") );
	setOrganizationDomain(  QApplication::tr("idstein.info") );
	setOrganizationName(  QApplication::tr("Idstein") );
}

Client::~Client(void) {
//	unload();
	//shutdown(5000);
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
