#include "CCursor.h"
#include "../interfaces/IResource.h"
#include <Qt/QPixmap.h>
#include "../core/CClient.h"
#include <Qt/QBitmap.h>

using namespace gui;

CCursor::CCursor() {
	load();
}

CCursor::~CCursor() {
	unload();
}

bool CCursor::load() {
	const quint32 normalCursorOffset = 0x206A;
	const quint32 warmodeCursorOffset = 0x2053;
	
	if( core::Client::root->resource() ) {
		m_cursors.resize( CURSOR_COUNT );
		m_cursorsWar.resize( CURSOR_COUNT );
		for( quint8 i = 0; i < m_cursors.size(); i ++ ) {
			qint32 hotX, hotY;
			QImage* cur = core::Client::root->resource()->getCursor( hotX, hotY, normalCursorOffset + i );
			if(cur) {
				QPixmap pix = QPixmap::fromImage( *cur );
				pix.setMask( QBitmap::fromImage( cur->createAlphaMask() ) );
				m_cursors[i] = new QCursor( pix, hotX, hotY );
				delete cur;
				cur = NULL;
			} else {
				qWarning() << QString("Couldn't load cursor %1 at id %2").arg(i).arg(normalCursorOffset + i);
			}
			cur = core::Client::root->resource()->getCursor( hotX, hotY, warmodeCursorOffset + i );
			if(cur) {
				QPixmap pix = QPixmap::fromImage( *cur );
				pix.setMask( QBitmap::fromImage( cur->createAlphaMask() ) );
				m_cursorsWar[i] = new QCursor( pix, hotX, hotY );
				delete cur;
				cur = NULL;
			} else {
				qWarning() << QString("Couldn't load cursor %1 at id %2").arg(i).arg(warmodeCursorOffset + i);
			}
		}
	}
	if( m_cursors.empty() ) {
		qCritical() << "No standart cursor loaded. Using system fallback";
		return false;
	} else {
		QApplication::setOverrideCursor( getCursor( eCursor_STANDART ) );
		return true;
	}
}

bool CCursor::unload() {
	foreach( QCursor* cur, m_cursors ) {
		if(cur)
			delete cur;
	}
	foreach( QCursor* cur, m_cursorsWar ) {
		if(cur)
			delete cur;
	}
	if( !m_cursors.empty() )
		QApplication::restoreOverrideCursor();
	return true;
}

bool CCursor::reload() {
	return (unload() &&	load());
}

QCursor& CCursor::getCursor( eCursorType _cursor, bool _warmode ) {
	QCursor* res = NULL;
	if(_warmode) {
		res = m_cursorsWar[_cursor];
	} else {
		res = m_cursors[_cursor];
	}
	if(!res)
		res = m_cursors[eCursor_STANDART];
	if( !res ) 
		return QCursor( Qt::ArrowCursor );
	return *res;
}