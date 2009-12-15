//#include "CLog.h"
//#include <Qt/QFile.h>
//#include <Qt/QTextStream.h>
//#include "CSettings.h"
//
//using namespace core;
//
//
//Log::Log() {
//	QString date = QDateTime::currentDateTime().toString(QString("dd-MM-yyyy'__'hh-mm"));
//	QString logpath = CSettings::instance().value( "Data/logpath", "" ).toString();
//	// rediretion of cerr to errors_DD-MM-YYYY_hh-mm.txt
//	m_fileerr = new QFile( QString("%1%2%3%4").arg(logpath).arg("err").arg(date).arg(".txt") );
//	if(!m_fileerr->open( QFile::WriteOnly ) ) {
//		NOTICE(QString("Couldn't create error file %1").arg(m_fileerr->fileName()).toStdString());
//		NOTICE(qPrintable(m_fileerr->errorString()));
//	};
//	m_err.setDevice( m_fileerr );
//	// rediretion of clog to log_DD-MM-YYYY_hh-mm.txt
//	m_filelog = new QFile( QString("%1%2%3%4").arg(logpath).arg("log").arg(date).arg(".txt") );
//	if(!m_filelog->open( QFile::WriteOnly ) ) {
//		NOTICE(QString("Couldn't create log file %1").arg(m_filelog->fileName()).toStdString() );
//		NOTICE(qPrintable(m_filelog->errorString()));
//	};
//	m_log.setDevice(m_filelog);
//}
//
//Log::~Log() {
//	m_muterr.lock();
//	m_mutlog.lock();
//	m_log.flush();
//	m_err.flush();
//	if(m_fileerr) {
//		m_fileerr->close();
//		delete m_fileerr;
//	};
//	if(m_filelog) {
//		m_filelog->close();
//		delete m_filelog;
//	};
//}
//
//void Log::log( QString _str ) {
//	m_mutlog.lock();
//	m_log << _str;
//	m_log.flush();
//	m_mutlog.unlock();
//}
//
//
//void Log::error( QString _str ) {
//	m_muterr.lock();
//	m_err << _str;
//	m_err.flush();
//	m_muterr.unlock();
//}
