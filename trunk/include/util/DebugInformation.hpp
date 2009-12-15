/*
 * DebugInformation.hpp
 *
 *  Created on: 13.12.2009
 *      Author: idstein
 */

#ifndef DEBUGINFORMATION_HPP_
#define DEBUGINFORMATION_HPP_

#include <qglobal.h>

#ifdef _DEBUG
class DebugInformation {
	public:
	QDate mDate; // __DATE__
	QTime mTime; // __TIME__
	QString mFile; // __FILE__
	QString mFunction; // __PRETTY_FUNCTION__
	quint32 mLine; // __LINE__
	DebugInformation( const char* _date, const char* _time, const char* _file, const char* _func, unsigned int _line )
	: mDate( QDate::fromString(_date, "MMM DD YYYY") ),
	  mTime( QTime::fromString( _time, "hh:mm:ss") ),
	  mFile(_file),
	  mFunction(_func),
	  mLine(_line) {
	}
	DebugInformation()
	: mLine(0) {};
	~DebugInformation() {
	}
};

#define DEBUG_INFO DebugInformation( __DATE__, __TIME__, __FILE__, __PRETTY_FUNCTION__, __LINE__ )
#else
#define DEBUG_INFO DebugInformation()
#endif

#endif /* DEBUGINFORMATION_HPP_ */
