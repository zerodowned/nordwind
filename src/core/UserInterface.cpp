#include "core/UserInterface.hpp"
//#include <QGLWidget>
#include <qmdiarea.h>
#include <qmdisubwindow.h>
#include <qtextedit.h>
#include "game/View.hpp"

using namespace core;

UserInterfaces::UserInterfaces()
: QMainWindow(/*new QGLWidget()*/) {
	resize( 640, 480 );
	QMdiArea* area = new QMdiArea(this);
	area->setBackground( Qt::black );
	setCentralWidget( area );
        addUI( new game::View(this) );
	show();
}

UserInterfaces::~UserInterfaces() {
}

QPointer<QMdiArea> UserInterfaces::getUIArea() {
	return qobject_cast<QMdiArea*>(centralWidget());
}

UserInterfaces::UserInterface UserInterfaces::addUI(QWidget *_element) {
    return UserInterfaces::UserInterface(getUIArea()->addSubWindow(_element));
}

//#include "CCursor.h"
//#include <Qt/QLabel.h>
//#include <Qt/QPushButton.h>
//#include <Qt/QShortcut.h>
//#include <Qt/QGraphicsProxyWidget.h>
//#include "../interfaces/IResource.h"
//#include "CConsole.h"
////#include "CTextField.h"
//
//using namespace gui;
//
//UserInterface::UserInterface( ) : m_cursor(NULL)  {
//	qDebug() << "Created interface for " << name() << version();
//	//CCursor::instance();
//	/*QPalette p = QApplication::palette();
//	p.setColor( QPalette::Background, QColor( 0xFF,0xFF,0xFF,0x00 ) );
//	QApplication::setPalette( p );*/
//	//resize( core::CSettings::instance().value( "size", QSize( 640, 480 )).toSize() );
//	//move( core::CSettings::instance().value( "pos", QPoint( 50, 50 ) ).toPoint() );
//
//	//setAutoFillBackground(true);
//	//setWindowTitle(QString("Siebenwind %1").arg(CClient::version()) );
//	/*int id = QFontDatabase::addApplicationFont("Oltima_Unline_Solid.ttf");
//    if (id < 0) {
//        WARNING("Loading the font failed :(");
//	} else {
//		setFont(QFontDatabase::applicationFontFamilies(id).first());
//	}*/
//}
//
//UserInterface::~UserInterface() {
//	unload();
//	//CCursor::destroyInstance();
//}
//
//bool UserInterface::unload() {
//	if(m_cursor)
//		delete m_cursor;
//	if(m_scene)
//		delete m_scene;
//	return true;
//}
//
//bool UserInterface::reload() {
//	return (unload() && load());
//}
//
//bool UserInterface::load() {
//	if(!IModule::load())
//		return false;
//
//	qDebug() << "Init gui window";
//	setWindowTitle( core::Client::instance()->applicationName() );
//	move( settings()->value( "pos", QPoint(0,0) ).toPoint() );
//	resize( settings()->value( "size", QSize( 640, 480 ) ).toSize() );
//	// no scrollbars at startup
//	setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
//	setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
//
//	qDebug() << "Init gui scene";
//	// gui scene
//	m_scene = new QGraphicsScene();
//	m_scene->setBackgroundBrush( QBrush( QColor(0x00, 0x00,0x00) ) );
//	setScene( m_scene );
//	setSceneRect( 0,0, size().width(), size().height() );
//
//	qDebug() << "Init cursor";
//	m_cursor = new CCursor();
//	Q_CHECK_PTR(m_cursor);
//
//
//	//QPushButton btn( "Close me" );
//	//m_scene->addWidget( new QPushButton("Close methis ist ajfkewrjewokrjewrokerw" ));
//	if(core::Client::root->resource()) {
//		resource::ITexture* res = core::Client::root->resource()->getGump( 0x157C );
//		if(res)
//			m_scene->addPixmap(  QPixmap::fromImage( res->image() ) );
//	}
//
//	QWidget* console = new CConsole();
//	console->setGeometry(0,size().height()-console->sizeHint().height(), size().width(), console->sizeHint().height() );
//	m_scene->addWidget( console )->setZValue( 1 );
//	QShortcut* shortcut = new QShortcut( this );
//	shortcut->setKey(Qt::CTRL + Qt::Key_F1);
//	QGraphicsView::connect( shortcut, SIGNAL(activated()), console, SLOT(toggle()) );
//	//test Resource Interface
//	//QLabel* test = new QLabel( "Dies ist ein Test", this, Qt::SubWindow );
//	//test->move(0,0);
//	//QPushButton btn( "Close Me", test );
//	//setWindowOpacity( 0.5 );
//	show();
//	return true;
//}
//
//void UserInterface::closeEvent(QCloseEvent *event) {
//	settings()->setValue( "size", size() );
//	settings()->setValue( "pos", pos() );
//}
//
///*
//void CGui::initializeGL() {
//    glClearColor(0.0, 0.0, 0.0, 0.0);
//	glClearStencil(1);
//	glEnable(GL_TEXTURE_2D);
//	glDisable(GL_DITHER);
//    glShadeModel(GL_SMOOTH);
//	glEnable(GL_NORMALIZE);
//	glDisable( GL_LIGHTING );
//	glEnable( GL_BLEND );
//	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
//}
//
//void CGui::resizeGL( qint32 width, qint32 height ) {
//    glViewport(0, 0, width, height);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//
//	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
//    gluOrtho2D(0, width, 0, height );
//    glMatrixMode(GL_MODELVIEW);
//}
//
//void CGui::paintGL() {
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	++fps;
//}
//
//void CGui::refreshFps() {
//	setWindowTitle(QString("Siebenwind %1 %2 fps").arg(CClient::version()).arg(fps) );
//	fps = 0;
//}
//
//void CGui::setupGui() {
//	CPushButton* button = new CPushButton( this, width()-65, 0, "CloseButton" );
//	button->addState( eBtnStandart, 0x7DB ).addState( eBtnPressed, 0x7DA );
//	connect( button, SIGNAL(click()), this, SLOT(close()) );
//	CBorderGump* bg = new CBorderGump( this, 0,0, 400, 400, 0xA40 );
//
//	//CGumpImage* gump = new CGumpImage( this, 400,400, 0xA2C, 0, false, "test", 50,50, true, false );
//	QTimer* refresh = new QTimer(this);
//	connect( refresh, SIGNAL(timeout()), this, SLOT(update()) );
//	refresh->start( core::CSettings::instance().value( "refresh", 50 ).toUInt() );
//
//	//CTextField* text = new CTextField( this, 300, 300, QString("Zeile 1!"), "Zeile1", true, 0xBBC );
//	//CTextField* text2 = new CTextField( this, 300, 350, QString("Zeile 2!") );
//	QTextEdit* edit = new QTextEdit( this );
//	edit->move( 100, 100 );
//	edit->show();
//	if( core::CSettings::instance().value( "show_fps", true ).toBool() ) {
//		fps = 0;
//		QTimer* framerate = new QTimer(this);
//		connect( framerate, SIGNAL(timeout()), this, SLOT(refreshFps()) );
//		framerate->start(1000);
//	};
//}*/
